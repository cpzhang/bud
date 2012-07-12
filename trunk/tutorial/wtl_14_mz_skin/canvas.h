#pragma once

#include "render/Euclid.h"
#include "tinyxml2.h"
#include "global.h"

struct sAnimation
{
	std::string name;
	int duration;
	std::string skinFile;
};
typedef std::map<std::string, sAnimation> AniMap;
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
	virtual void update(u32 delta){};
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
		mMaterial->setVertexDeclaration(Euclid::eVertexDeclarationType_PositionTextureBoneWeight);
	}
	void setSkeletonMaterial(const std::string& fileName, Euclid::eMaterialType mt)
	{
		mSkeletonMaterial = RenderEngineImp::getInstancePtr()->getRenderEngine()->getMaterialManager()->createMaterial(mt);
		mSkeletonMaterial->setEffect(fileName);
	}
	void setSkin(const std::string& fileName)
	{
		if (mSkin)
		{
			delete mSkin;
			mSkin = NULL;
		}
		mSkin = new Euclid::Skin;
		mSkin->create(fileName);
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
	virtual void update(u32 delta)
	{
		static Euclid::AnimationTime at;
		at.start = 0;
		at.end = 1999;
		at.current += delta * 1.f;
		while(at.current >= at.end)
		{
			at.current -= at.end;
		}
		if (mSkin && mSkeleton)
		{
			mSkeleton->update(at, *mSkin);
		}
	}
	virtual void decode(const std::string& fileName)
	{
		//
		setMesh(fileName);
		//
		setMaterial("shader/aPSkin.fx", Euclid::eMaterialType_Vertex);
	}
	void _renderMesh()
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
		//
		fx->setMatrixArray("gMatrixPalette", &mSkeleton->_matrices[0], mSkeleton->_matrices.size()); 
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
	virtual void render()
	{
		_renderMesh();
		//
		renderSkeleton();
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
				p[0].position = mSkeleton->_matricesFull[b->id] * p[0].position;
				p[1].position = mSkeleton->_matricesFull[f->id] * p[1].position;
				
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
				RenderEngineImp::getInstancePtr()->getRenderEngine()->getRenderSystem()->objectSpace2ScreenSpace(&p, &p, NULL, &pm, &vm, &mSkeleton->_matricesFull[b->id]);
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
		if (mSkin)
		{
			delete mSkin;
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
		mSkin = NULL;
	}
private:
	std::string mName;
	std::string mMeshFile;
	Euclid::Mesh* mMesh;
	Euclid::IMaterial* mMaterial;
	Euclid::Skeleton* mSkeleton;
	Euclid::IMaterial* mSkeletonMaterial;
	Euclid::Skin* mSkin;
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
			//
			path = Buddha::FileSystem::getInstancePtr()->getDataDirectory() + "skin/";
			Euclid::MZ::saveSkin(path);
			//
			g->setSkin(path + "run.skin");
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
	virtual void update(u32 delta)
	{
		for (size_t i = 0; i != mGeometries.size(); ++i)
		{
			mGeometries[i]->update(delta);
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
		//
		{
			tinyxml2::XMLElement* mat = r->FirstChildElement("animations");
			if (mat)
			{
				tinyxml2::XMLElement* tex = mat->FirstChildElement("animation");
				if (tex)
				{
					sAnimation an;
					an.name = tex->Attribute("name");
					an.skinFile = tex->Attribute("skin");
					an.duration = tex->IntAttribute("duration");
					mAnimations[an.name] = an;
				}
			}
			//
			mGeometry->setSkin(Buddha::FileSystem::getInstancePtr()->getDataDirectory() + mAnimations.begin()->second.skinFile);
		}
		//
		{
			tinyxml2::XMLElement* mat = r->FirstChildElement("skeleton");
			if (mat)
			{
				std::string skeleton = mat->Attribute("file");
				mGeometry->setSkeleton(Buddha::FileSystem::getInstancePtr()->getDataDirectory() + skeleton);
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
	virtual void update(u32 delta)
	{
		mGeometry->update(delta);
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
		mAnimations.clear();
	}
private:
	std::string mName;
	Geometry* mGeometry;
	AniMap mAnimations;
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
	void update(u32 delta);
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
	void changeModelFile(const std::string& fileName);
	void saveFile(const std::string& fileName);
private:
	void _renderGeometry();
	void _clear();
	void _clearModel();
	bool _create();
	bool _createModel();
	bool _isInitialized();
	void _calcFPS();
	void _destroymodel();
	//
private:
	HWND					_hwnd;
	Euclid::FreeType*		_font[Euclid::eFontProperty_Size];
	float					_fps;
	std::string				_modelFileName;
	std::string				_modelMeshName;
	std::string				_modelMaterialName;
	std::string				_shaderTexture;
	Euclid::Camera			_camera;
	Euclid::CameraControllerThirdPerson _cameraController;
	bool					_isModelOK;
	bool					_isOK;
	typedef std::pair<std::string, std::string> TextureFilePair;
	typedef std::vector<TextureFilePair> TextureFilePairVec;
	TextureFilePairVec		_textureFiles;
	IPrimitive* _model;
};
