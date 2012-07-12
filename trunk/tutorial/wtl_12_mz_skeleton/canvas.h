#pragma once

#include "render/Euclid.h"
#include "tinyxml2.h"
#include "global.h"

class IPrimitive
{
public:
	virtual ~IPrimitive(){};
	//È«Â·¾¶
	virtual void decode(const std::string& fileName) = 0;
	virtual void render() = 0;
	virtual void destroy() = 0;
	virtual void setMatrix( const std::string& name, const Mat4& pMatrix ) = 0;
	virtual void saveFile(const std::string& fileName){};
	virtual void renderSkeleton(){};
};

class Geometry: public IPrimitive
{
public:
	Geometry()
	{
		_clear();
	}
	~Geometry()
	{

	}
	void setMaterial(const std::string& fileName, Euclid::eMaterialType mt)
	{
		mMaterial = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(mt);
		mMaterial->setEffect(fileName);
	}
	void setSkeletonMaterial(const std::string& fileName, Euclid::eMaterialType mt)
	{
		mSkeletonMaterial = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(mt);
		mSkeletonMaterial->setEffect(fileName);
	}
	Euclid::IMaterial* getMaterial()
	{
		return mMaterial;
	}
	void setMesh(const std::string& fileName)
	{
		mMeshFile = fileName;
		mMesh = new Euclid::Mesh;
		mMesh->create(mMeshFile);
	}
	void setSkeleton(const std::string& fileName)
	{
		mSkeleton = new Euclid::Skeleton;
		mSkeleton->create(fileName);
	}
	
	virtual void decode(const std::string& fileName)
	{
		//
		setMesh(fileName);
		//
		setMaterial("shader/aP.fx", Euclid::eMaterialType_Vertex);
	}
	virtual void render()
	{
		//
		renderSkeleton();
		return;
		if (NULL == mMesh)
		{
			return;
		}
		if (NULL == mMaterial)
		{
			return;
		}
		Euclid::Effect* fx = mMaterial->getEffect();
		if (NULL == fx)
		{
			return;
		}
		//
		mMaterial->apply();
//		fx->setMatrixArray("MatrixPalette", ); 
		u32 passes = 0;
		fx->begin(&passes);
		for (u32 i = 0; i != passes; ++i)
		{
			fx->beginPass(i);
			mMesh->render();
			fx->endPass();
		}
		fx->end();
	}
	virtual void renderSkeleton()
	{
		if (NULL == mSkeleton)
		{
			return;
		}
		if (NULL == mSkeletonMaterial)
		{
			return;
		}
		Euclid::Effect* fx = mSkeletonMaterial->getEffect();
		if (NULL == fx)
		{
			return;
		}
		//
		mSkeletonMaterial->apply();
		
		Euclid::BoneNodeMapIterator it = mSkeleton->getCommandMapIterator();
		while(!it.isAtEnd())
		{
			Euclid::BoneNode* n = it.getCurrentValue();
			Euclid::BoneNode* p = n->getParentNode();
			if (NULL != p)
			{
				Euclid::Bone* b = n->getBone();
				Euclid::Bone* f = p->getBone();
				//
				Euclid::sPosition p[2];
				p[0].position = b->initialMatrix * p[0].position;
				p[1].position = f->initialMatrix * p[1].position;
				
				//
				{
					u32 passes = 0;
					fx->begin(&passes);
					for (u32 i = 0; i != passes; ++i)
					{
						fx->beginPass(i);

						RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawPrimitiveUP(Euclid::ePrimitive_LineList, 1, p, sizeof(Euclid::sPosition));
						//
						RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->drawPrimitiveUP(Euclid::ePrimitive_PointList, 2, p, sizeof(Euclid::sPosition));

						fx->endPass();
					}
					fx->end();
				}
			}
			//
			++it;
		}
		//
		it.toStart();
		//
		Mat4 vm, pm;
		fx->getMatrix("gView", &vm);
		fx->getMatrix("gProjection", &pm);
		while(!it.isAtEnd())
		{
			Euclid::BoneNode* n = it.getCurrentValue();
			{
				Euclid::Bone* b = n->getBone();
				//
				Vec3 p;
				RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->objectSpace2ScreenSpace(&p, &p, NULL, &pm, &vm, &b->initialMatrix);
				//
				{
					RenderEngineImp::getInstancePtr()->getRenderEngine()->getFontManager()->getFont("freeNormal")->render(p, Vec3(1, 0, 0), Euclid::Color::Red, b->name);
				}
			}
			//
			++it;
		}
	}
	virtual void destroy()
	{
		if (mMesh)
		{
			mMesh->destroy();
			delete mMesh;
		}
		if (mMaterial)
		{
			mMaterial->destroy();
			delete mMaterial;
		}
		if (mSkeletonMaterial)
		{
			mSkeletonMaterial->destroy();
			delete mSkeletonMaterial;
		}
		if (mSkeleton)
		{
			mSkeleton->destroy();
			delete mSkeleton;
		}
		_clear();
	}
	virtual void setMatrix( const std::string& name, const Mat4& pMatrix )
	{
		if (NULL == mMesh)
		{
			return;
		}
		if (NULL == mMaterial)
		{
			return;
		}
		Euclid::Effect* fx = mMaterial->getEffect();
		if (NULL == fx)
		{
			return;
		}
		fx->setMatrix(name, pMatrix);
		//
		{
			if (NULL == mSkeletonMaterial)
			{
				return;
			}
			Euclid::Effect* fx = mSkeletonMaterial->getEffect();
			if (NULL == fx)
			{
				return;
			}
			fx->setMatrix(name, pMatrix);
		}
	}
private:
	void _clear()
	{
		mName.clear();
		mMeshFile.clear();
		mMesh = NULL;
		mMaterial = NULL;
		mSkeleton = NULL;
		mSkeletonMaterial = NULL;
	}
private:
	std::string mName;
	std::string mMeshFile;
	Euclid::Mesh* mMesh;
	Euclid::IMaterial* mMaterial;
	Euclid::Skeleton* mSkeleton;
	Euclid::IMaterial* mSkeletonMaterial;
};

class MzModel: public IPrimitive
{
public:
	MzModel()
	{
		_clear();
	}
	~MzModel()
	{

	}
	virtual void saveFile(const std::string& fileName)
	{
		Euclid::MZ::saveAnimation(fileName);
	}
	virtual void decode(const std::string& fileName)
	{
		Euclid::MZ::load(fileName);
		Euclid::Mesh m;
		for (int i = 0; i < Euclid::MZ::getSubMeshNumber(); ++i)
		{
			Euclid::MZ::create(i, &m);
			std::string path = Buddha::FileSystem::getInstancePtr()->getDataDirectory() + "mesh/" + Euclid::MZ::getSubMeshName(i) + ".mesh";
			m.save(path);
			m.destroy();
			Geometry* g = new Geometry;
			g->decode(path);
			mGeometries.push_back(g);
			//
			path = Buddha::FileSystem::getInstancePtr()->getDataDirectory() + "skin/" + Euclid::MZ::getSubMeshName(i) + ".skeleton";
			Euclid::MZ::saveSkeleton(path);
			g->setSkeleton(path);
			//
			g->setSkeletonMaterial("shader/aP.fx", Euclid::eMaterialType_Vertex);
		}
	}
	virtual void render()
	{
		for (size_t i = 0; i != mGeometries.size(); ++i)
		{
			mGeometries[i]->render();
		}
	}
	virtual void destroy()
	{
		for (size_t i = 0; i != mGeometries.size(); ++i)
		{
			mGeometries[i]->destroy();
			delete mGeometries[i];
		}
		_clear();
	}
	virtual void setMatrix( const std::string& name, const Mat4& pMatrix )
	{
		for (size_t i = 0; i != mGeometries.size(); ++i)
		{
			mGeometries[i]->setMatrix(name, pMatrix);
		}
	}
private:
	void _clear()
	{
		mGeometries.clear();
	}
private:
	typedef std::vector<Geometry*> GeoVec;
	GeoVec mGeometries;
};

class Entity : public IPrimitive
{
public:
	Entity()
	{
		_clear();
	}
	~Entity()
	{

	}
	virtual void decode(const std::string& fileName)
	{
		tinyxml2::XMLDocument doc;
		if (tinyxml2::XML_SUCCESS != doc.LoadFile(fileName.c_str()))
		{
			return;
		}
		tinyxml2::XMLElement* r = doc.RootElement();
		if (NULL == r)
		{
			return;
		}
		mName = r->Attribute("name");
		tinyxml2::XMLElement* mes = r->FirstChildElement("mesh");
		if (NULL == mes)
		{
			return;
		}

		mGeometry = new Geometry;
		mGeometry->setMesh(Buddha::FileSystem::getInstancePtr()->getDataDirectory() + mes->Attribute("file"));
		//
		{
			tinyxml2::XMLElement* mat = r->FirstChildElement("material");
			if (mat)
			{
				mGeometry->setMaterial(mat->Attribute("shader"), Euclid::eMaterialType_VertexTexture);
				tinyxml2::XMLElement* tex = mat->FirstChildElement("texture");
				if (tex)
				{
					mGeometry->getMaterial()->setTexture(tex->Attribute("name"), tex->Attribute("value"));
				}
			}
		}
	}
	virtual void render()
	{
		if (mGeometry)
		{
			mGeometry->render();
		}
	}
	virtual void destroy()
	{
		if (mGeometry)
		{
			mGeometry->destroy();
			delete mGeometry;
		}
		_clear();
	}
	virtual void setMatrix( const std::string& name, const Mat4& pMatrix )
	{
		if (mGeometry)
		{
			mGeometry->setMatrix(name, pMatrix);
		}
	}
private:
	void _clear()
	{
		mName.clear();
		mGeometry = NULL;
	}
private:
	std::string mName;
	Geometry* mGeometry;
};
class Group: public IPrimitive
{
public:
	Group()
	{
		_clear();
	}
	~Group()
	{
		
	}
	virtual void decode(const std::string& fileName)
	{
		tinyxml2::XMLDocument doc;
		if (tinyxml2::XML_SUCCESS != doc.LoadFile(fileName.c_str()))
		{
			return;
		}
		tinyxml2::XMLElement* r = doc.RootElement();
		if (NULL == r)
		{
			return;
		}
		mName = r->Attribute("name");
		tinyxml2::XMLElement* mes = r->FirstChildElement("entity");
		if (NULL == mes)
		{
			return;
		}
		while(mes)
		{
			Entity* e = new Entity;
			e->decode(Buddha::FileSystem::getInstancePtr()->getDataDirectory() + mes->Attribute("file"));
			mEntities.push_back(e);
			mes = mes->NextSiblingElement();
		}
	}
	virtual void render()
	{
		for(size_t i = 0; i != mEntities.size(); ++i)
		{
			mEntities[i]->render();
		}
	}
	virtual void destroy()
	{
		for(size_t i = 0; i != mEntities.size(); ++i)
		{
			mEntities[i]->destroy();
			delete mEntities[i];
			mEntities[i] = NULL;
		}
		_clear();
	}
	virtual void setMatrix( const std::string& name, const Mat4& pMatrix )
	{
		for(size_t i = 0; i != mEntities.size(); ++i)
		{
			if (NULL == mEntities[i])
			{
				continue;
			}
			mEntities[i]->setMatrix(name, pMatrix);
		}
	}
private:
	void _clear()
	{
		mName.clear();
		mEntities.clear();
	}
private:
	std::string mName;
	std::vector<Entity*> mEntities;
};

class Canvas
{
public:
	Canvas(HWND h);

public:
	bool create();
	void destroy();
	void render();
	void update();
	void onSize(int w, int h);
	LRESULT OnKeyDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//
	LRESULT OnLeftButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnLeftButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//
	LRESULT OnRightButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnRightButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//
	LRESULT OnMiddleButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMiddleButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	//
	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnMouseWheel(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

public:
	void changeTerrainFile(const std::string& fileName);
	void saveFile(const std::string& fileName);
private:
	void _renderGeometry();
	void _clear();
	void _clearTerrain();
	bool _create();
	bool _createTerrain();
	bool _isInitialized();
	void _calcFPS();
	void _destroyTerrain();
	//
private:
	HWND					_hwnd;
	Euclid::IMaterial*		_material;
	Euclid::IBuffer*		_vb;
	Euclid::Effect*			_fx;
	Euclid::FreeType*		_font[Euclid::eFontProperty_Size];
	float					_fps;
	std::string				_terrainFileName;
	std::string				_terrainMeshName;
	std::string				_terrainMaterialName;
	std::string				_shaderTexture;
	Euclid::Mesh*			_terrain;
	Euclid::Camera			_camera;
	Euclid::CameraControllerThirdPerson _cameraController;
	Euclid::Effect*			_fxTerrain;
	Euclid::ITexture*		_texTerrain;
	bool					_isTerrainOK;
	bool					_isOK;
	typedef std::pair<std::string, std::string> TextureFilePair;
	typedef std::vector<TextureFilePair> TextureFilePairVec;
	TextureFilePairVec		_textureFiles;
	IPrimitive* _group;
};
