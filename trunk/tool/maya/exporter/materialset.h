#pragma		once
#include "singleton.h"

class MaterialSet : public Singleton<MaterialSet>
{
public:
	//constructor
	MaterialSet(){};
	//destructor
	~MaterialSet(){	clear();}
	//clear
	void clear()
	{
		for (int i=0; i < (int)m_materials.size(); i++)
			delete m_materials[i];
		m_materials.clear();
	}
	//add material
	void addMaterial(material* pMat)
	{
		const char *szName = pMat->m_name.asChar();
		bool found = false;
		for (int i=0; i < (int)m_materials.size() && !found; i++)
		{
			if (m_materials[i]->m_name == pMat->m_name)
			{
				found = true;
				delete pMat;
			}
		}
		if (!found)
		{
			m_materials.push_back(pMat);
		}
	}
	//get material
	material* getMaterial(const MString& name)
	{
		for (int i=0; i < (int)m_materials.size(); i++)
		{
			if (m_materials[i]->m_name == name)
			{
				return m_materials[i];
			}
		}
		return NULL;
	};
	//get material set
	static MaterialSet& getSingleton()
	{
		assert(ms_Singleton);  
		return (*ms_Singleton);
	};
	static MaterialSet* getSingletonPtr()
	{
		return ms_Singleton;
	};

public:
	std::vector<material*> m_materials;
};

template<> MaterialSet* Singleton<MaterialSet>::ms_Singleton = 0;