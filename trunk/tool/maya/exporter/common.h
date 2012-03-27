// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>
#include "windowsx.h"
#include "Commdlg.h"
#include "process.h"
#include "shlwapi.h"
#include "comdef.h"

#include "atlstr.h"
#include <atlcoll.h>
#include <bitset>

#include "stdio.h"

// TODO: 在此处引用程序要求的附加头文件
//
#define _BOOL

#include <maya/MIOStream.h>
#include "maya/MPxNode.h"
#include "maya/MPxFileTranslator.h"
#include "maya/MPxTransform.h"
#include "maya/MPxCommand.h"
#include "maya/MArgList.h"
#include "maya/MStatus.h"
#include "maya/MFn.h"
#include "maya/M3dView.h"
#include "maya/MGlobal.h"
#include "maya/MAnimControl.h"
#include "maya/MSelectionList.h"
#include "maya/MItSelectionList.h"
#include "maya/MPlugArray.h"
#include "maya/MPlug.h"
#include "maya/MObject.h"
#include "maya/MObjectArray.h"
#include "maya/MFnDependencyNode.h"
#include "maya/MFnDagNode.h"
#include "maya/MDagPath.h"
#include "maya/MDagPathArray.h"
#include "maya/MObjectHandle.h"
#include "maya/MFnMesh.h"
#include "maya/MFnCamera.h"
#include "maya/MFnTransform.h"
#include "maya/MFnSkinCluster.h"
#include "maya/MFnSingleIndexedComponent.h"
#include "maya/MTypeId.h"
#include "maya/MDataHandle.h"
#include "maya/MDataBlock.h"
#include "maya/MFileObject.h"
#include "maya/MTime.h"
#include "maya/MIntArray.h"
#include "maya/MPoint.h"
#include "maya/MPointArray.h"
#include "maya/MVector.h"
#include "maya/MFloatVector.h"
#include "maya/mdoublearray.h"
#include "maya/MString.h"
#include "maya/MMatrix.h"
#include "maya/MFloatMatrix.h"
#include "maya/MFnNumericAttribute.h"
#include "maya/MFnTypedAttribute.h"
#include "maya/MFnCompoundAttribute.h"
#include "maya/mfndata.h"
#include "maya/MFnDoubleArrayData.h"
#include "maya/MFnNumericData.h"
#include "maya/MFnStringData.h"
#include "maya/MFnMatrixData.h"
#include "maya/MFnMeshData.h"
#include "maya/MItDependencyGraph.h"
#include "maya/MItDependencyNodes.h"
#include "maya/MItDag.h"
#include "maya/MItMeshPolygon.h"
#include "maya/MItMeshVertex.h"
#include "maya/MFnIkJoint.h"
#include "maya/MSceneMessage.h"
#include "maya/MDGMessage.h"
#include "maya/MMessage.h"
#include "maya/MNodeMessage.h"
#include "maya/MDagMessage.h"
#include "maya/MEventMessage.h"
#include <maya/MAngle.h>
#include <maya/MStringArray.h>
#include <maya/MFnIntArrayData.h>
#include <maya/MFnVectorArrayData.h>
#include <maya/MFnLambertShader.h>
#include <maya/MFnPhongShader.h>
#include <maya/MFnBlinnShader.h>
#include <maya/MFloatPointArray.h>
#include <maya/MFloatVectorArray.h>
#include <maya/MFloatArray.h>
#include <maya/MItGeometry.h>
#include <maya/MAnimUtil.h>
#include <maya/MRenderUtil.h>
#include <maya/MQuaternion.h>

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <hash_map>
#include <fstream>
using namespace std;

// #define USE_SYSTEM_MEMORY
// #define BASE_STATIC_LIB
// #include "Base.h"
//
#define safeDestroy(x) if((x)!=NULL){ (x)->destroy(); delete (x); (x) = NULL;}
#define safeDelete(x) if((x)!=NULL){delete (x); (x) = NULL;}

#define safeDeleteArray(x) if((x)!=NULL){delete[] (x); (x) = NULL;}
#define STL_SAFE_DELETE(x)	{if(x){delete (x);(x)=0;}}
#define STL_SAFE_RELEASE(x)	{if(x){(x)->release();(x)=0;}}
template<class T>void STLDeleteSequence(T &t)
{
	typename T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		STL_SAFE_DELETE(*i);
	}
	t.clear();
}

#pragma   pack(1)
template<class T> struct keyframeT
{
	int time;
	T data;
};
#pragma   pack()

class MemoryFileWriter;
template<class T>void writeSequence(MemoryFileWriter *pDataStream,T &t)
{
	T::iterator i = t.begin();
	for(; i != t.end(); ++i)
	{
		pDataStream->write(&(*i),sizeof(*i));
	}
}
//
class MemoryFileWriter
{
public:
	MemoryFileWriter()
	{
		_cursor = 0;
		_length = 0;
		_buffer = NULL;
	}
	~MemoryFileWriter()
	{
		if (_buffer)
		{
			delete _buffer;
			_buffer = NULL;
		}

		_cursor = 0;
		_length = 0;
	}
public:
	bool isEnd()
	{
		return _cursor >= _length;
	}

	bool isOverFlow(size_t s)
	{
		return s + _cursor >= _length;
	}

	bool reallocate(size_t s)
	{
		if (s == 0)
		{
			return false;
		}

		size_t newSize = _cursor + s;

		void* b = new char[newSize];
		if (b == NULL)
		{
			return false;
		}
		memset(b, 0, newSize);
		if (_cursor > 0)
		{
			memcpy(b, _buffer, _cursor);
		}

		safeDelete(_buffer);
		_buffer = b;

		_length = newSize;

		return true;
	}

	bool write(void* b, size_t s)
	{
		if (isOverFlow(s))
		{
			if (!reallocate(s * 2))
			{
				return false;
			}

		}
		_writeImp(b, s);

		return true;
	}

	bool _writeImp(void* b, size_t s)
	{
		if (b == NULL)
		{
			return false;
		}
		memcpy((char*)_buffer + _cursor, (char*)b, s);
		_cursor += s;
		return true;
	}

	size_t getSize()
	{
		if (_cursor == 0)
		{
			return 0;
		}

		return _cursor;
	}

	void* getBuffer()
	{
		return _buffer;
	}
private:
	void*			_buffer;
	unsigned int	_cursor;
	unsigned int	_length;
};

struct sChunk
{
public:
	sChunk()
	{
	}
	~sChunk()
	{
	}
	std::string name;
	MemoryFileWriter buffer;
};

class ChunkSet
{
public:
	std::vector<sChunk*> _chunkSet;
	sChunk*			_currentChunk;

public:
	ChunkSet()
	{
		_currentChunk = NULL;
	}
	~ChunkSet()
	{
		_currentChunk = NULL;
		STLDeleteSequence(_chunkSet);
	}
public:
	bool beginChunk(const std::string& name)
	{
		if (name.size() != 4)
		{
			return false;
		}

		sChunk* c = new sChunk;
		c->name = name;
		_chunkSet.push_back(c);
		_currentChunk = c;

		return true;
	}

	void endChunk()
	{
		_currentChunk = NULL;
	}

	bool write(const char* b, size_t s)
	{
		return write((void*)b, s);
	}

	bool write(void* b, size_t s)
	{
		if (_currentChunk == NULL)
		{
			return false;
		}

		return _currentChunk->buffer.write(b, s);
	}

	void save(std::string& fileName)
	{
		std::ofstream o(fileName.c_str(), std::ios::binary);

		for (size_t i = 0; i != _chunkSet.size(); ++i)
		{
			size_t s = _chunkSet[i]->buffer.getSize();

			std::string name = _chunkSet[i]->name;
			char c[4];
			c[0] = name[3];
			c[1] = name[2];
			c[2] = name[1];
			c[3] = name[0];

			o.write(c, 4);

			o.write((char*)&s, sizeof(size_t));

			o.write((char*)_chunkSet[i]->buffer.getBuffer(), s);
		}

		o.close();
	}

	MemoryFileWriter* getCurrentMemoryFileWriter()
	{
		return &_currentChunk->buffer;
	}
};
typedef unsigned int uint;
typedef unsigned char uchar;
typedef unsigned short ushort;