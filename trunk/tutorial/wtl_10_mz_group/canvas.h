#pragma once

#include "render/Euclid.h"
#include "tinyxml2.h"
#include "global.h"
class Group
{
public:
	class Entity
	{
	public:
		Entity()
		{
			_clear();
		}
		~Entity()
		{

		}
		void decode(const std::string& fileName)
		{
			std::string name;
			name = Buddha::FileSystem::getInstancePtr()->getDataDirectory();
			name += fileName;

			tinyxml2::XMLDocument doc;
			if (tinyxml2::XML_SUCCESS != doc.LoadFile(name.c_str()))
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
			mMeshFile = mes->Attribute("file");
			mMesh = new Euclid::Mesh;
			mMesh->create(mMeshFile);
			//
			{
				tinyxml2::XMLElement* mat = r->FirstChildElement("material");
				if (mat)
				{
					mMaterial = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(Euclid::eMaterialType_VertexTexture);
					mMaterial->setEffect(mat->Attribute("shader"));
					tinyxml2::XMLElement* tex = mat->FirstChildElement("texture");
					if (tex)
					{
						mMaterial->setTexture(tex->Attribute("name"), tex->Attribute("value"));
					}
				}
			}
		}
		void render()
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
		void destroy()
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
		void setMatrix( const std::string& name, const Mat4& pMatrix )
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
	Group()
	{
		_clear();
	}
	~Group()
	{
		
	}
	void decode(const std::string& fileName)
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
			e->decode(mes->Attribute("file"));
			mEntities.push_back(e);
			mes = mes->NextSiblingElement();
		}
	}
	void render()
	{
		for(size_t i = 0; i != mEntities.size(); ++i)
		{
			mEntities[i]->render();
		}
	}
	void destroy()
	{
		for(size_t i = 0; i != mEntities.size(); ++i)
		{
			mEntities[i]->destroy();
			delete mEntities[i];
			mEntities[i] = NULL;
		}
		_clear();
	}
	void setMatrix( const std::string& name, const Mat4& pMatrix )
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
	Euclid::FreeType*		_font;
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
	Group* _group;
};
