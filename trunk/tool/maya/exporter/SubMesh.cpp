#include "common.h"
#include ".\submesh.h"
#include "Math.h"

SubMesh::SubMesh(void)
{
	m_bAnimated = false;
	clear();
}

SubMesh::~SubMesh(void)
{
	clear();
}

void SubMesh::clear()
{
	m_pMaterial = NULL;
	m_vertices.clear();
	m_faces.clear();
}

MStatus SubMesh::loadMaterial(MObject& shader,MStringArray& uvsets)
{
	MPlug plug;
	MPlugArray srcplugarray;
	bool foundShader = false;
	MStatus stat;
	//get shader from shading group
	MFnDependencyNode shadingGroup(shader);
	plug = shadingGroup.findPlug("surfaceShader");
	plug.connectedTo(srcplugarray,true,false,&stat);
	MFnDependencyNode *pShaderNode = 0;
	for (int i=0; i<srcplugarray.length() && !foundShader; i++)
	{
		MObject node = srcplugarray[i].node();
		pShaderNode = new MFnDependencyNode(node);
		foundShader = true;
	}

	if(!pShaderNode)return MS::kFailure;

	//check if this material has already been created
	const char *name = pShaderNode->name().asChar();
	material* pMaterial = MaterialSet::getSingleton().getMaterial(pShaderNode->name());
	//if the material has already been created, update the pointer
	if (pMaterial)
	{
		m_pMaterial = pMaterial;
	}
	//else create it and add it to the material set
	else
	{
		pMaterial = new material();
		pMaterial->load(pShaderNode,uvsets);
		m_pMaterial = pMaterial;
		MaterialSet::getSingleton().addMaterial(pMaterial);
	}
	//safeDelete(pShaderNode);

	return MS::kSuccess;
}

struct vindex
{
	int pidx;
	int nidx;
	std::vector<texcoords> texcoords;
};

namespace stdext
{
	inline size_t hash_value(const vindex&f)
	{
		int value = f.pidx + 256 * f.nidx;
		for(size_t n = 0;n < f.texcoords.size();n++)
		{
			value += f.texcoords[n].u + f.texcoords[n].v;
		}

		return value;
	};
}

bool operator==(const vindex& f1,const vindex& f2)
{
	bool b = (f1.nidx == f2.nidx && f1.pidx == f1.pidx);
	if(b)
	{
		if(f1.texcoords.size() != f2.texcoords.size())
		{
			return false;
		}
		for(size_t n = 0;n < f1.texcoords.size();n++)
		{
			if(fabs(f1.texcoords[n].u - f2.texcoords[n].u) > 0.001 || fabs(f1.texcoords[n].v - f2.texcoords[n].v) > 0.001)
			{
				return false;
			}
		}
		return true;
	}
	return false;
}

template<> struct less<vindex>
{
	bool operator()(const vindex& v1,const vindex& v2) const
	{
		return !(v1 == v2);
	}
};

struct vinfo
{
	vertex v;
	int index;
};

MStatus SubMesh::load_(std::vector<face>& faces, std::vector<vertexInfo>& vertInfo, MFloatPointArray& points, 
					  MFloatVectorArray& normals,bool opposite)
{
	//iterate over faces array, to retrieve vertices info
	for (int i=0; i<faces.size(); i++)
	{
		face newFace;
		for (int j=0; j<3; j++)
		{
			vertex v;
			vertexInfo vInfo = vertInfo[faces[i].v[j]];
			// save vertex coordinates
			v.x = points[vInfo.pointIdx].x;
			v.y = points[vInfo.pointIdx].y;
			v.z = points[vInfo.pointIdx].z;
			// save vertex normal
			if (opposite)
			{
				v.n.x = -normals[vInfo.normalIdx].x;
				v.n.y = -normals[vInfo.normalIdx].y;
				v.n.z = -normals[vInfo.normalIdx].z;
			}
			else
			{
				v.n.x = normals[vInfo.normalIdx].x;
				v.n.y = normals[vInfo.normalIdx].y;
				v.n.z = normals[vInfo.normalIdx].z;
			}
			v.n.normalize();
			// save vertex color
			v.r = vInfo.r;
			v.g = vInfo.g;
			v.b = vInfo.b;
			v.a = vInfo.a;
			// save vertex bone assignements
			for (int k=0; k<vInfo.vba.size(); k++)
			{
				vba newVba;
				newVba.jointIdx = vInfo.jointIds[k];
				newVba.weight = vInfo.vba[k];
				v.vbas.push_back(newVba);
			}
			// save texture coordinates
			for (int k=0; k<vInfo.u.size(); k++)
			{
				texcoords newTexCoords;
				newTexCoords.u = vInfo.u[k];
				newTexCoords.v = vInfo.v[k];
				newTexCoords.w = 0;
				v.texcoords.push_back(newTexCoords);
			}
			// add newly created vertex to vertex list
			m_vertices.push_back(v);
			if (opposite)	// reverse order of face vertices to get correct culling
				newFace.v[2-j] = m_vertices.size() - 1;
			else
				newFace.v[j] = m_vertices.size() - 1;
		}
		m_faces.push_back(newFace);
	}

	return MS::kSuccess;
}

MStatus SubMesh::load(std::vector<face>& faces, std::vector<vertexInfo>& vertInfo, MFloatPointArray& points, 
					  MFloatVectorArray& normals,bool opposite)
{
	int n1 = faces.size();
	int n2 = vertInfo.size();
	int n3 = points.length();
	int n4 = normals.length();
	//iterate over faces array, to retrieve vertices info

	typedef stdext::hash_map<vindex,vinfo,stdext::hash_compare<vindex,less<vindex> > > IndexMap;
	IndexMap vv;
	for (int i=0; i<faces.size(); i++)
	{
		face newFace;
		for (int j=0; j<3; j++)
		{
			vertex v;
			vertexInfo vInfo = vertInfo[faces[i].v[j]];

			vindex vi;
			vi.pidx = vInfo.pointIdx;
			vi.nidx = vInfo.normalIdx;
			for (int k=0; k<vInfo.u.size(); k++)
			{
				texcoords newTexCoords;
				newTexCoords.u = vInfo.u[k];
				newTexCoords.v = vInfo.v[k];
				newTexCoords.w = 0;
				vi.texcoords.push_back(newTexCoords);
			}

			IndexMap::iterator it = vv.find(vi);
			if(it != vv.end())
			{
				if (opposite)	// reverse order of face vertices to get correct culling
					newFace.v[2-j] = (*it).second.index;
				else
					newFace.v[j] = (*it).second.index;

				/*char str[256];
				sprintf(str,"Found:%d,%d ",(*it).first.pidx,(*it).first.nidx);
				OutputDebugString(str);
				for(size_t i = 0;i < (*it).first.texcoords.size();i++)
				{
					sprintf(str,"%f,%f,",(*it).first.texcoords[i].u,(*it).first.texcoords[i].v);
					OutputDebugString(str);
				}
				OutputDebugString("     ");*/

				for (int k=0; k<vInfo.u.size(); k++)
				{
					texcoords newTexCoords;
					newTexCoords.u = vInfo.u[k];
					newTexCoords.v = vInfo.v[k];
					newTexCoords.w = 0;
					//sprintf(str,"%f,%f,",newTexCoords.u,newTexCoords.v);
					//OutputDebugString(str);
				}
				//OutputDebugString("\n");
			}
			else
			{
				// save vertex coordinates
				v.x = points[vInfo.pointIdx].x;
				v.y = points[vInfo.pointIdx].y;
				v.z = points[vInfo.pointIdx].z;
				if(*(unsigned int*)&v.x == 0x7fc00000)
				{
					v.x = v.x;
				}
				// save vertex normal
				if (opposite)
				{
					v.n.x = -normals[vInfo.normalIdx].x;
					v.n.y = -normals[vInfo.normalIdx].y;
					v.n.z = -normals[vInfo.normalIdx].z;
				}
				else
				{
					v.n.x = normals[vInfo.normalIdx].x;
					v.n.y = normals[vInfo.normalIdx].y;
					v.n.z = normals[vInfo.normalIdx].z;
				}
				v.n.normalize();
				// save vertex color
				v.r = vInfo.r;
				v.g = vInfo.g;
				v.b = vInfo.b;
				v.a = vInfo.a;
				// save vertex bone assignements
				for (int k=0; k<vInfo.vba.size(); k++)
				{
					vba newVba;
					newVba.jointIdx = vInfo.jointIds[k];
					newVba.weight = vInfo.vba[k];
					v.vbas.push_back(newVba);

					m_bAnimated = true;
				}
				// save texture coordinates
				for (int k=0; k<vInfo.u.size(); k++)
				{
					texcoords newTexCoords;
					newTexCoords.u = vInfo.u[k];
					newTexCoords.v = vInfo.v[k];
					newTexCoords.w = 0;
					v.texcoords.push_back(newTexCoords);

				}
				vi.texcoords = v.texcoords;
				// add newly created vertex to vertex list
				m_vertices.push_back(v);

				if (opposite)	// reverse order of face vertices to get correct culling
					newFace.v[2-j] = m_vertices.size() - 1;
				else
					newFace.v[j] = m_vertices.size() - 1;

				vinfo vin;
				vin.index = m_vertices.size() - 1;
				vin.v = v;
				vv[vi] = vin;
			}
		}
		m_faces.push_back(newFace);
	}

	return MS::kSuccess;
}

MStatus SubMesh::load__(std::vector<face>& faces, std::vector<vertexInfo>& vertInfo, MFloatPointArray& points, 
					  MFloatVectorArray& normals,bool opposite)
{
	int n1 = faces.size();
	int n2 = vertInfo.size();
	int n3 = points.length();
	int n4 = normals.length();
	//iterate over faces array, to retrieve vertices info

	stdext::hash_map<vindex,vinfo> vv;
	for (int i=0; i<faces.size(); i++)
	{
		face newFace;
		for (int j=0; j<3; j++)
		{
			vertex v;
			vertexInfo vInfo = vertInfo[faces[i].v[j]];

			vindex vi;
			vi.pidx = vInfo.pointIdx;
			vi.nidx = vInfo.normalIdx;
			for (int k=0; k<vInfo.u.size(); k++)
			{
				texcoords newTexCoords;
				newTexCoords.u = vInfo.u[k];
				newTexCoords.v = vInfo.v[k];
				newTexCoords.w = 0;
				vi.texcoords.push_back(newTexCoords);
			}

			stdext::hash_map<vindex,vinfo>::iterator it = vv.find(vi);
			if(it != vv.end())
			{
				if (opposite)	// reverse order of face vertices to get correct culling
					newFace.v[2-j] = (*it).second.index;
				else
					newFace.v[j] = (*it).second.index;

				char str[256];
				sprintf(str,"Found:%d,%d ",(*it).first.pidx,(*it).first.nidx);
				OutputDebugString(str);
				for(size_t i = 0;i < (*it).first.texcoords.size();i++)
				{
					sprintf(str,"%f,%f,",(*it).first.texcoords[i].u,(*it).first.texcoords[i].v);
					OutputDebugString(str);
				}
				OutputDebugString("     ");

				for (int k=0; k<vInfo.u.size(); k++)
				{
					texcoords newTexCoords;
					newTexCoords.u = vInfo.u[k];
					newTexCoords.v = vInfo.v[k];
					newTexCoords.w = 0;
					sprintf(str,"%f,%f,",newTexCoords.u,newTexCoords.v);
					OutputDebugString(str);
				}
				OutputDebugString("\n");
			}
			else
			{
				// save vertex coordinates
				v.x = points[vInfo.pointIdx].x;
				v.y = points[vInfo.pointIdx].y;
				v.z = points[vInfo.pointIdx].z;
				// save vertex normal
				if (opposite)
				{
					v.n.x = -normals[vInfo.normalIdx].x;
					v.n.y = -normals[vInfo.normalIdx].y;
					v.n.z = -normals[vInfo.normalIdx].z;
				}
				else
				{
					v.n.x = normals[vInfo.normalIdx].x;
					v.n.y = normals[vInfo.normalIdx].y;
					v.n.z = normals[vInfo.normalIdx].z;
				}
				v.n.normalize();
				// save vertex color
				v.r = vInfo.r;
				v.g = vInfo.g;
				v.b = vInfo.b;
				v.a = vInfo.a;
				// save vertex bone assignements
				for (int k=0; k<vInfo.vba.size(); k++)
				{
					vba newVba;
					newVba.jointIdx = vInfo.jointIds[k];
					newVba.weight = vInfo.vba[k];
					v.vbas.push_back(newVba);
				}
				// save texture coordinates
				for (int k=0; k<vInfo.u.size(); k++)
				{
					texcoords newTexCoords;
					newTexCoords.u = vInfo.u[k];
					newTexCoords.v = vInfo.v[k];
					newTexCoords.w = 0;
					v.texcoords.push_back(newTexCoords);

				}
				vi.texcoords = v.texcoords;
				// add newly created vertex to vertex list
				m_vertices.push_back(v);

				if (opposite)	// reverse order of face vertices to get correct culling
					newFace.v[2-j] = m_vertices.size() - 1;
				else
					newFace.v[j] = m_vertices.size() - 1;

				vinfo vin;
				vin.index = m_vertices.size() - 1;
				vin.v = v;
				vv[vi] = vin;
			}
		}
		m_faces.push_back(newFace);
	}

	return MS::kSuccess;
}