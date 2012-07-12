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
	
	virtual void decode(const std::string& fileName)
	{
		//
		setMesh(fileName);
		//
		setMaterial("shader/aP.fx", Euclid::eMaterialType_Vertex);
	}
	virtual void render()
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
	}
private:
	void _clear()
	{
		mName.clear();
		mMeshFile.clear();
		mMesh = NULL;
		mMaterial = NULL;
	}
private:
	std::string mName;
	std::string mMeshFile;
	Euclid::Mesh* mMesh;
	Euclid::IMaterial* mMaterial;
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
		mGeometries.clear();
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
