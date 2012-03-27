#include "common.h"
#include ".\mesh.h"

Mesh::Mesh(void)
{
	m_pSkinCluster = NULL;
	m_pSkeleton = NULL;
}

Mesh::~Mesh(void)
{
	clear();
}

void Mesh::clear()
{
	for (int i=0; i<m_submeshes.size(); i++)
		delete m_submeshes[i];

	m_uvsets.clear();
	m_submeshes.clear();
	if (m_pSkinCluster)
		delete m_pSkinCluster;
	if (m_pSkeleton)
		delete m_pSkeleton;
}

MStatus Mesh::load(MDagPath& meshDag,MDagPath *pDagPath)
{
	MStatus stat;
	std::vector<MFloatArray> weights;
	std::vector<MIntArray> Ids;
	std::vector<MIntArray> jointIds;
	unsigned int numJoints = 0;
	std::vector<vertexInfo> vertices;
	MFloatPointArray points;
	MFloatVectorArray normals;

	if (!meshDag.hasFn(MFn::kMesh))
		return MS::kFailure;

	MFnMesh mesh(meshDag);
	/*{
		mesh.getPoints(points,MSpace::kWorld);
		MPoint pos;
		mesh.getPoint(1,pos,MSpace::kWorld);
		for(unsigned i = 0;i < points.length();i++)
		{
			MFloatPoint fp = points[i];
			float x = fp.x;
			float y = fp.y;
			float z = fp.z;
			fp = fp;
		}
	}*/
	int numVertices = mesh.numVertices();
	vertices.resize(numVertices);
	weights.resize(numVertices);
	Ids.resize(numVertices);
	jointIds.resize(numVertices);

	// 获取UV坐标集的名称
	MStringArray uvsets;
	if (mesh.numUVSets() > 0)
	{
		stat = mesh.getUVSetNames(uvsets);
		if (MS::kSuccess != stat)
		{
			std::cout << "Error retrieving UV sets names\n";
			return MS::kFailure;
		}
	}
	// 保存UV集信息
	for (int i=m_uvsets.size(); i<uvsets.length(); i++)
	{
		uvset uv;
		uv.size = 2;
		m_uvsets.push_back(uv);
	}

	// 检查法线是否反
	bool opposite = false;
	mesh.findPlug("opposite",true).getValue(opposite);

	// get connected skin cluster (if present)
	bool foundSkinCluster = false;
	MItDependencyNodes kDepNodeIt( MFn::kSkinClusterFilter );            
	for( ;!kDepNodeIt.isDone() && !foundSkinCluster; kDepNodeIt.next()) 
	{            
		MObject kObject = kDepNodeIt.item();
		m_pSkinCluster = new MFnSkinCluster(kObject);
		unsigned int uiNumGeometries = m_pSkinCluster->numOutputConnections();
		for(unsigned int uiGeometry = 0; uiGeometry < uiNumGeometries; ++uiGeometry ) 
		{
			unsigned int uiIndex = m_pSkinCluster->indexForOutputConnection(uiGeometry);
			MObject kOutputObject = m_pSkinCluster->outputShapeAtIndex(uiIndex);
			if(kOutputObject == mesh.object()) 
			{
				foundSkinCluster = true;
			}
			else
			{
				delete m_pSkinCluster;
				m_pSkinCluster = NULL;
			}
		}

		// load connected skeleton (if present)
		if (m_pSkinCluster)
		{
			if (!m_pSkeleton)
				m_pSkeleton = new skeleton();
			stat = m_pSkeleton->load(m_pSkinCluster);
			if (MS::kSuccess != stat)
				std::cout << "Error loading skeleton data\n";
		}
		else
		{
//			breakable;
		}
	}
	// get connected shaders
	MObjectArray shaders;
	MIntArray shaderPolygonMapping;
	stat = mesh.getConnectedShaders(0,shaders,shaderPolygonMapping);
	if (MS::kSuccess != stat)
	{
		std::cout << "Error getting connected shaders\n";
		return MS::kFailure;
	}

	if (shaders.length() <= 0)
	{
		std::cout << "No connected shaders, skipping mesh\n";
		return MS::kFailure;
	}

	// create a series of arrays of faces for each different submesh
	std::vector<faceArray> polygonSets;
	polygonSets.resize(shaders.length());

	// Get faces data
	// prepare vertex table
	for (int i=0; i<vertices.size(); i++)
		vertices[i].next = -2;
	//get vertex positions from mesh data

	mesh.getPoints(points,MSpace::kWorld);
	mesh.getNormals(normals,MSpace::kWorld);

	//get list of vertex weights
	if (m_pSkinCluster)
	{
		MItGeometry iterGeom(meshDag);
		for (int i=0; !iterGeom.isDone(); iterGeom.next(), i++)
		{
			weights[i].clear();
			Ids[i].clear();
			MObject component = iterGeom.component();
			MFloatArray vertexWeights;
			stat=m_pSkinCluster->getWeights(meshDag,component,vertexWeights,numJoints);
			int nWeights = vertexWeights.length();
			for(int j = 0;j<nWeights;j++)
			{
				if(vertexWeights[j] >= 0.00001f || vertexWeights[j] <= -0.00001f  )
				{
					// 记录该节点j
					Ids[i].append(j);
					weights[i].append(vertexWeights[j]);
				}
			}
		
			//weights[i]= vertexWeights;

			if (MS::kSuccess != stat)
			{
				std::cout << "Error retrieving vertex weights\n";
			}
			// get ids for the joints
			if (m_pSkeleton)
			{
				jointIds[i].clear();
				if(weights[i].length() > 0 )
				{
					MDagPathArray influenceObjs;
					m_pSkinCluster->influenceObjects(influenceObjs,&stat);
					if (MS::kSuccess != stat)
					{
						std::cout << "Error retrieving influence objects for given skin cluster\n";
					}
					jointIds[i].setLength(weights[i].length());
					for (int j=0; j<jointIds[i].length(); j++)
					{
						bool foundJoint = false;
						for (int k=0; k<m_pSkeleton->getJoints().size() && !foundJoint; k++)
						{
							if (influenceObjs[Ids[i][j]].partialPathName() == m_pSkeleton->getJoints()[k].name)
							{
								foundJoint=true;
								jointIds[i][j] = m_pSkeleton->getJoints()[k].id;
							}
						}
					}
				}
				
			}
		}
	}
	// create an iterator to go through mesh polygons
	if (mesh.numPolygons() > 0)
	{
		const char *name = mesh.name().asChar();
		MItMeshPolygon faceIter(mesh.object(),&stat);
		if (MS::kSuccess != stat)
		{
			std::cout << "Error accessing mesh polygons\n";
			return MS::kFailure;
		}

		// iterate over mesh polygons
		for (; !faceIter.isDone(); faceIter.next())
		{
			int numTris=0;
			faceIter.numTriangles(numTris);
			// for every triangle composing current polygon extract triangle info
			for (int iTris=0; iTris<numTris; iTris++)
			{
				MPointArray triPoints;
				MIntArray tempTriVertexIdx,triVertexIdx;
				int idx;
				// create a new face to store triangle info
				face newFace;
				// extract triangle vertex indices
				faceIter.getTriangle(iTris,triPoints,tempTriVertexIdx);
				// convert indices to face-relative indices
				MIntArray polyIndices;
				faceIter.getVertices(polyIndices);
				unsigned int iPoly, iObj;
				for (iObj=0; iObj < tempTriVertexIdx.length(); ++iObj)
				{
					// iPoly is face-relative vertex index
					for (iPoly=0; iPoly < polyIndices.length(); ++iPoly)
					{
						if (tempTriVertexIdx[iObj] == polyIndices[iPoly]) 
						{
							triVertexIdx.append(iPoly);
							break;
						}
					}
				}
				// iterate over triangle's vertices
				for (int i=0; i<3; i++)
				{
					bool different = true;
					int vtxIdx = faceIter.vertexIndex(triVertexIdx[i]);
					int nrmIdx = faceIter.normalIndex(triVertexIdx[i]);

					// get vertex color
					MColor color;
					if (faceIter.hasColor(triVertexIdx[i]))
					{
						stat = faceIter.getColor(color,triVertexIdx[i]);
						if (MS::kSuccess != stat)
						{
							color = MColor(1,1,1,1);
						}
						if (color.r > 1)
							color.r = 1;
						if (color.g > 1)
							color.g = 1;
						if (color.b > 1)
							color.b = 1;
						if (color.a > 1)
							color.a = 1;
					}
					else
					{
						color = MColor(1,1,1,1);
					}
					if (vertices[vtxIdx].next == -2)	// first time we encounter a vertex in this position
					{
						// save vertex position
						points[vtxIdx].cartesianize();
						vertices[vtxIdx].pointIdx = vtxIdx;
						// save vertex normal
						vertices[vtxIdx].normalIdx = nrmIdx;
						// save vertex colour
						vertices[vtxIdx].r = color.r;
						vertices[vtxIdx].g = color.g;
						vertices[vtxIdx].b = color.b;
						vertices[vtxIdx].a = color.a;
						// save vertex texture coordinates
						vertices[vtxIdx].u.resize(uvsets.length());
						vertices[vtxIdx].v.resize(uvsets.length());
						// save vbas
						vertices[vtxIdx].vba.resize(weights[vtxIdx].length());
						for (int j=0; j<weights[vtxIdx].length(); j++)
						{
							vertices[vtxIdx].vba[j] = (weights[vtxIdx])[j];
						}
						// save joint ids
						vertices[vtxIdx].jointIds.resize(jointIds[vtxIdx].length());
						for (int j=0; j<jointIds[vtxIdx].length(); j++)
						{
							vertices[vtxIdx].jointIds[j] = (jointIds[vtxIdx])[j];
						}
						// save uv sets data
						for (int j=0; j<uvsets.length(); j++)
						{
							float2 uv;
							stat = faceIter.getUV(triVertexIdx[i],uv,&uvsets[j]);
							if (MS::kSuccess != stat)
							{
								uv[0] = 0;
								uv[1] = 0;
							}
							vertices[vtxIdx].u[j] = uv[0];
							vertices[vtxIdx].v[j] = (-1)*(uv[1]-1);
						}
						// save vertex index in face info
						newFace.v[i] = vtxIdx;
						// update value of index to next vertex info (-1 means nothing next)
						vertices[vtxIdx].next = -1;
					}
					else	// already found at least 1 vertex in this position
					{
						// check if a vertex with same attributes has been saved already
						for (int k=vtxIdx; k!=-1 && different; k=vertices[k].next)
						{
							different = false;

							MFloatVector n1 = normals[vertices[k].normalIdx];
							MFloatVector n2 = normals[nrmIdx];
							if (n1.x!=n2.x || n1.y!=n2.y || n1.z!=n2.z)
							{
								different = true;
							}


							if (vertices[k].r!=color.r || vertices[k].g!=color.g || vertices[k].b!= color.b || vertices[k].a!=color.a)
							{
								different = true;
							}

							for (int j=0; j<uvsets.length(); j++)
							{
								float2 uv;
								stat = faceIter.getUV(triVertexIdx[i],uv,&uvsets[j]);
								if (MS::kSuccess != stat)
								{
									uv[0] = 0;
									uv[1] = 0;
								}
								uv[1] = (-1)*(uv[1]-1);
								if (vertices[k].u[j]!=uv[0] || vertices[k].v[j]!=uv[1])
								{
									different = true;
								}
							}

							idx = k;
						}
						// if no identical vertex has been saved, then save the vertex info
						if (different)
						{
							vertexInfo vtx;
							// save vertex position
							vtx.pointIdx = vtxIdx;
							// save vertex normal
							vtx.normalIdx = nrmIdx;
							// save vertex colour
							vtx.r = color.r;
							vtx.g = color.g;
							vtx.b = color.b;
							vtx.a = color.a;
							// save vertex vba
							vtx.vba.resize(weights[vtxIdx].length());
							for (int j=0; j<weights[vtxIdx].length(); j++)
							{
								vtx.vba[j] = (weights[vtxIdx])[j];
							}
							// save joint ids
							vtx.jointIds.resize(jointIds[vtxIdx].length());
							for (int j=0; j<jointIds[vtxIdx].length(); j++)
							{
								vtx.jointIds[j] = (jointIds[vtxIdx])[j];
							}
							// save vertex texture coordinates
							vtx.u.resize(uvsets.length());
							vtx.v.resize(uvsets.length());
							for (int j=0; j<uvsets.length(); j++)
							{
								float2 uv;
								stat = faceIter.getUV(triVertexIdx[i],uv,&uvsets[j]);
								if (MS::kSuccess != stat)
								{
									uv[0] = 0;
									uv[1] = 0;
								}
								vtx.u[j] = uv[0];
								vtx.v[j] = (-1)*(uv[1]-1);
							}
							vtx.next = -1;
							vertices.push_back(vtx);
							// save vertex index in face info
							newFace.v[i] = vertices.size()-1;
							vertices[idx].next = vertices.size()-1;
						}
						else
						{
							newFace.v[i] = idx;
						}
					}
				} // end iteration of triangle vertices
				// add face info to the array corresponding to the submesh it belongs
				// skip faces with no shaders assigned
				if (shaderPolygonMapping[faceIter.index()] >= 0)
					polygonSets[shaderPolygonMapping[faceIter.index()]].push_back(newFace);
			} // end iteration of triangles
		}
	}
	std::cout << "done reading mesh triangles\n";

	// create a submesh for every different shader linked to the mesh
	unsigned shaderLength = shaders.length();
	for (int i=0; i<shaderLength; i++)
	{
		// check if the submesh has at least 1 triangle
		if (polygonSets[i].size() > 0)
		{
			//create new submesh
			SubMesh* pSubmesh = new SubMesh();
const char *nm = mesh.name().asChar();
const char *nm1 = mesh.partialPathName().asChar();
const char *nm2 = mesh.parentNamespace().asChar();
const char *nm3 = mesh.fullPathName().asChar();
const char *nm4 = meshDag.fullPathName().asChar();

			pSubmesh->m_name = meshDag.partialPathName();
			if(pDagPath)
				pSubmesh->m_name = pDagPath->partialPathName();
			const char *szName = pSubmesh->m_name.asChar();

			if(shaderLength > 1)
			{
				char a[256];
				sprintf(a,"%d",i);
				pSubmesh->m_name += a;
			}
			OutputDebugString(pSubmesh->m_name.asChar());
			OutputDebugString("\n");

			//OutputDebugString(szName);
			//OutputDebugString("\n");

			//load linked shader
			stat = pSubmesh->loadMaterial(shaders[i],uvsets);
			if (stat != MS::kSuccess)
			{
				MFnDependencyNode shadingGroup(shaders[i]);
				std::cout << "Error loading submesh linked to shader " << shadingGroup.name().asChar() << "\n";
				return MS::kFailure;
			}

			//load vertex and face data
			stat = pSubmesh->load(polygonSets[i],vertices,points,normals,opposite);

			//add submesh to current mesh
			m_submeshes.push_back(pSubmesh);
		}
	}

	return MS::kSuccess;
}

