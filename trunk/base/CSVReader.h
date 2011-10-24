//==========================================================================
/**
* @file	  : CSVReader.h
* @author : cpzhang <chengpengzhang@gmail.com>
* created : 2010-6-10   21:22
* purpose : 
*/
//==========================================================================

#ifndef __CSVReader_h__
#define __CSVReader_h__

#include "Common.h"

namespace Buddha
{
	template<class T> 
	class /*_BUHHDA_EXPORT_*/ CSVReader
	{
		enum eType
		{
			eType_Bool,
			eType_Int,
			eType_Float,
			eType_String,
		};
	public:
		CSVReader(const std::string& fileName);
		~CSVReader();

	public:
		bool create();
		bool destroy();

	public:
		std::string		_fileName;
		std::vector<T*> _elements;
		std::map<std::string, eType> _typeMap;
		std::vector<eType> _types;
	};

	template<class T>
	Buddha::CSVReader<T>::~CSVReader()
	{

	}
	template<class T>
	CSVReader<T>::CSVReader( const std::string& fileName )
		:	_fileName(fileName)
	{

	}

	template<class T>
	bool CSVReader<T>::create()
	{
		_typeMap["bool"] = eType_Bool;
		_typeMap["int"] = eType_Int;
		_typeMap["float"] = eType_Float;
		_typeMap["string"] = eType_String;
		
		std::ifstream f(_fileName.c_str());
		//std::ifstream f("e:\\20100423Refracture\\bin\\data\\scp\\blooding.csv", std::ios::in);
		//
		if (!f.good())
		{
			Error("Failed to open CSV");
			return false;
		}

		/*static std::string line;
		static const int Max_Line = 1024;
		line.reserve(Max_Line);

		f.getline((char*)line.c_str(), Max_Line);*/

		std::string field;
		// specification
		f>>field;

		// type
		f>>field;
		for (int i = 0; i < field.size(); ++i)
		{
			if (field[i] == ',')
			{
				field[i] = ' ';
			}
		}
		std::istringstream stream(field);
		while(stream.good())
		{
			stream>>field;
			_types.push_back(_typeMap[field]);
		}

		f>>field;
		while (f.good())
		{
			if (field[0] == '#')
			{
				if (f.good())
				{
					f>>field;
				}
				continue;
			}

			for (int i = 0; i < field.size(); ++i)
			{
				if (field[i] == ',')
				{
					field[i] = ' ';
				}
			}
			std::istringstream stream(field);
			DWORD cursor = 0;
			T* el = new T;

			memset(el, 0, sizeof(T));

			for (int k = 0; k < _types.size(); ++k)
			{
				switch (_types[k])
				{
				case eType_Bool:
					{
						bool b;
						stream>>b;
						memcpy((char*)(el) + cursor, &b, sizeof(bool));
						cursor += sizeof(bool);
					}
					break;
				case eType_Int:
					{
						int i;
						stream>>i;
						memcpy((char*)(el) + cursor, &i, sizeof(int));
						cursor += sizeof(int);
					}
					break;

				case eType_Float:
					{
						float f;
						stream>>f;
						memcpy((char*)(el) + cursor, &f, sizeof(float));
						cursor += sizeof(float);
					}
					break;

				case eType_String:
					{
						char s[64];
						stream>>s;
						DWORD add = (DWORD)memcpy((char*)(el) + cursor, s, 64);
						cursor += 0x00000040;
					}
					break;
				default:
					break;
				}
			}

			_elements.push_back(el);

			if (f.good())
			{
				f>>field;
			}
		}

		//
		f.close();

		return true;
	}



	template<class T>
	bool CSVReader<T>::destroy()
	{
		for(int i = 0; i < _elements.size(); ++i)
		{
			if (_elements[i])
			{
				delete _elements[i];
				_elements[i] = 0;
			}
		}
		_elements.clear();

		return true;
	}
}

#endif // __CSVReader_h__
 
