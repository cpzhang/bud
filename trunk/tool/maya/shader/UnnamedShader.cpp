// Plugin: UnnamedShader.cpp
//
// Produces dependency graph node UnnamedShader
// This node is an example of a Lambert shader and how to build a
// dependency node as a surface shader in Maya. The inputs for this node
// are many, and can be found in the Maya UI on the Attribute Editor for
// the node. The output attributes for the node are "outColor" and
// "outTransparency". To use this shader, create a lambertShader with
// Shading Group or connect the outputs to a Shading Group's
// "SurfaceShader" attribute.
//
#include "StdAfx.h"
#include <maya/MIOStream.h>
#include <maya/MPxNode.h>
#include <maya/MString.h>
#include <maya/MTypeId.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h> 
#include <maya/MArrayDataHandle.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MFnLightDataAttribute.h> 
#include <maya/MFloatVector.h>
#include <maya/MFnPlugin.h>
#include <maya/MGlobal.h>
#include <maya/MFnEnumAttribute.h>

// This class will create a new shader. Shaders are custom dependency
// graph objects so we will derive this class from the basic DG node
// type MPxNode
//

//unnamedShader_rd.dll，这个_rd不能丢，maya竟然是根据文件名来定材质。去掉之后，老的材质全部不认识了

class UnnamedShader : public MPxNode
{
public:
	UnnamedShader();
	virtual			~UnnamedShader();

	static  void *	creator();
	virtual MStatus	compute( const MPlug&, MDataBlock& );
	static  MStatus	initialize();


	// postConstructor:
	// The postConstructor method allows us to call MPxNode member
	// functions during initialization. Internally maya creates two
	// objects when a user defined node is created, the internal MObject
	// and the user derived object. The association between the these
	// two objects is not made until after the MPxNode constructor is
	// called. This implies that no MPxNode member function can be called
	// from the MPxNode constructor. The postConstructor will get called
	// immediately after the constructor when it is safe to call any
	// MPxNode member function.
	//

	virtual void	postConstructor(); 

	static  MTypeId   id;  // The IFF type id

protected:

	// 材质深度测试和深度写入 - 版本号14加入
	static MObject	aNoDepthCheck;
	static MObject	aNoDepthWrite;
	// 材质是否打光 - 版本号12加入
	static MObject	aUnlit;
	// 材质颜色关键桢 - 版本号10加入
	static MObject	aUnnamedColor;

	// DontCastShadow - 版本号9加入
	static MObject	aDontCastShadow;

	// Texcoord Flow U - 版本号4加入
	static MObject	aTexcoordFlowU;

	// Texcoord Flow V - 版本号4加入
	static MObject	aTexcoordFlowV;

	// Texcoord Rotate Speed - 版本号28加入
	static MObject	aTexcoordRotate;

	// Texcoord Angle Reset - 版本号28加入
	static MObject	aTexcoordAngleReset;

	// Texcoord Position Reset - 版本号28加入
	static MObject	aTexcoordPositionReset;

	// Texcoord Flow Speed U - 版本号4加入
	static MObject	aTexcoordFlowSpeedU;

	// Texcoord Flow Speed V - 版本号4加入
	static MObject	aTexcoordFlowSpeedV;

	// Texcoord Rotate - 版本号28加入
	static MObject	aTexcoordRotateSpeed;

	// Blend Mode
	static MObject	aBlendMode;

	//版本号27加入
	static MObject  aAdditiveLayer;

	// Double Side
	static MObject	aDoubleSide;

	static MObject	aTransparency;

	// 版本号7加入
	static MObject aTextureRows;
	// 版本号7加入
	static MObject aTextureCols;
	// 版本号7加入
	static MObject aTextureChangeStyle;
	// 版本号7加入
	static MObject aTextureChangeInterval;

};


// IFF type ID
// Each node requires a unique identifier which is used by
// MFnDependencyNode::create() to identify which node to create, and by
// the Maya file format. 
//
// For local testing of nodes you can use any identifier between
// 0x00000000 and 0x0007ffff, but for any node that you plan to use for
// more permanent purposes, you should get a universally unique id from
// Alias Support. You will be assigned a unique range that you
// can manage on your own. 
//
MTypeId UnnamedShader::id( 0x82000 );


// the postConstructor() function is called immediately after the objects
// constructor. It is not safe to call MPxNode member functions from the
// constructor, instead they should be called here.
//
void UnnamedShader::postConstructor( )
{
	// setMPSafe indicates that this shader can be used for multiprocessor
	// rendering. For a shading node to be MP safe, it cannot access any
	// shared global data and should only use attributes in the datablock
	// to get input data and store output data.
	//
	setMPSafe( true );
}


///////////////////////////////////////////////////////
// DESCRIPTION: attribute information
///////////////////////////////////////////////////////
//
MObject	 UnnamedShader::aNoDepthCheck;
MObject	 UnnamedShader::aNoDepthWrite;
MObject  UnnamedShader::aUnlit;
MObject  UnnamedShader::aUnnamedColor;
MObject  UnnamedShader::aDontCastShadow;
MObject	 UnnamedShader::aTexcoordFlowU;
MObject	 UnnamedShader::aTexcoordFlowV;
MObject	 UnnamedShader::aTexcoordRotate;
MObject	 UnnamedShader::aTexcoordAngleReset;
MObject	 UnnamedShader::aTexcoordPositionReset;
MObject	 UnnamedShader::aTexcoordFlowSpeedU;
MObject	 UnnamedShader::aTexcoordFlowSpeedV;
MObject	 UnnamedShader::aTexcoordRotateSpeed;
MObject  UnnamedShader::aBlendMode;
MObject	 UnnamedShader::aAdditiveLayer;
MObject  UnnamedShader::aDoubleSide;
MObject	 UnnamedShader::aTransparency;
MObject  UnnamedShader::aTextureRows;
MObject  UnnamedShader::aTextureCols;
MObject  UnnamedShader::aTextureChangeStyle;
MObject  UnnamedShader::aTextureChangeInterval;


// This node does not need to perform any special actions on creation or
// destruction
//

UnnamedShader::UnnamedShader() { }
UnnamedShader::~UnnamedShader() { }


// The creator() method allows Maya to instantiate instances of this node.
// It is called every time a new instance of the node is requested by
// either the createNode command or the MFnDependencyNode::create()
// method. 
//
// In this case creator simply returns a new UnnamedShader object.
//

void* UnnamedShader::creator()
{
	return new UnnamedShader();
}


// The initialize method is called only once when the node is first
// registered with Maya. In this method you define the attributes of the
// node, what data comes in and goes out of the node that other nodes may
// want to connect to. 
//

MStatus UnnamedShader::initialize()
{
	MFnNumericAttribute nAttr; 
	MFnLightDataAttribute lAttr;

	MStatus status; // Status will be used to hold the MStatus value
	// returned by each api function call. It is important
	// to check the status returned by a call to aid in
	// debugging. Failed API calls can result in subtle 
	// errors that can be difficult to track down, you may
	// wish to use the CHECK_MSTATUS macro for any API
	// call where you do not need to provide your own
	// error handling.
	//

	// Attribute Initialization:
	//
	// create      - The create function creates a new attribute for the
	//				 node, it takes a long name for the attribute, a short
	//				 name for the attribute, the type of the attribute,
	//				 and a status object to determine if the api call was
	//				 successful.
	//
	// setKeyable  - Sets whether this attribute should accept keyframe
	//				 data, Attributes are not keyable by default.
	//
	// setStorable - Sets whether this attribute should be storable. If an
	//				 attribute is storable, then it will be writen out
	//				 when the node is stored to a file. Attributes are 
	//               storable by default.
	//
	// setDefault  - Sets the default value for this attribute.
	//
	// setUsedAsColor - Sets whether this attribute should be presented as
	//				 a color in the UI.
	//
	// setHidden   - Sets whether this attribute should be hidden from the
	//				 UI. This is useful if the attribute is being used for
	//				 blind data, or if it is being used as scratch space
	//				 for a geometry calculation (should also be marked
	//				 non-connectable in that case). Attributes are not
	//				 hidden by default.
	//
	// setReadable - Sets whether this attribute should be readable. If an
	//				 attribute is readable, then it can be used as the
	//				 source in a dependency graph connection. Attributes
	//				 are readable by default.
	//
	// setWritable - Sets whether this attribute should be readable. If an
	//				 attribute is writable, then it can be used as the
	//				 destination in a dependency graph connection. If an
	//			     attribute is not writable then setAttr commands will
	//				 fail to change the attribute. If both keyable and
	//				 writable for an attribute are set to true it will be
	//				 displayed in the channel box when the node is
	//				 selected. Attributes are writable by default.
	//
	// setArray    - Sets whether this attribute should have an array of
	//				 data. This should be set to true if the attribute
	//				 needs to accept multiple incoming connections.
	//				 Attributes are single elements by default.
	//


	// Input Attributes
	//

	aNoDepthCheck = nAttr.create("noDepthCheck","ndc",MFnNumericData::kBoolean,false,&status);
	nAttr.setStorable(true);

	aNoDepthWrite = nAttr.create("noDepthWrite","ndw",MFnNumericData::kBoolean,false,&status);
	nAttr.setStorable(true);

	aUnlit = nAttr.create("unlit","ul",MFnNumericData::kBoolean,false,&status);
	nAttr.setStorable(true);

	aUnnamedColor = nAttr.createColor("unnamedColor","uc");
	nAttr.setStorable(true);
	nAttr.setKeyable(true);
	nAttr.setDefault(1.0f,1.0f,1.0f);

	aDontCastShadow = nAttr.create("dontCastShadow","dontcs",MFnNumericData::kBoolean,false,&status);
	CHECK_MSTATUS( status );
	nAttr.setStorable(true);

	aTexcoordFlowU = nAttr.create("tcFlowU","flowU",MFnNumericData::kBoolean,false,&status);
	CHECK_MSTATUS( status );
	nAttr.setStorable(true);

	aTexcoordFlowV = nAttr.create("tcFlowV","flowV",MFnNumericData::kBoolean,false,&status);
	CHECK_MSTATUS( status );
	nAttr.setStorable(true);

	aTexcoordRotate = nAttr.create("tcRotate","rotate",MFnNumericData::kBoolean,false,&status);
	CHECK_MSTATUS( status );
	nAttr.setStorable(true);

	aTexcoordAngleReset = nAttr.create("tcAngleReset","angleReset",MFnNumericData::kBoolean,false,&status);
	CHECK_MSTATUS( status );
	nAttr.setStorable(true);

	aTexcoordPositionReset = nAttr.create("tcPositionReset","positionReset",MFnNumericData::kBoolean,false,&status);
	CHECK_MSTATUS( status );
	nAttr.setStorable(true);

	aTexcoordFlowSpeedU = nAttr.create("tcFlowSpeedU","speedU",MFnNumericData::kFloat,1.0f);
	nAttr.setStorable(true);
	nAttr.setMin(-32.0f);
	nAttr.setMax(32.0f);
	nAttr.setKeyable(true);	//版本号26加入关键桢

	aTexcoordFlowSpeedV = nAttr.create("tcFlowSpeedV","speedV",MFnNumericData::kFloat,1.0f);
	nAttr.setStorable(true);
	nAttr.setMin(-32.0f);
	nAttr.setMax(32.0f);
	nAttr.setKeyable(true);	//版本号26加入关键桢

	aTexcoordRotateSpeed = nAttr.create("tcRotateSpeed","rotateSpeed",MFnNumericData::kFloat,1.0f);
	nAttr.setStorable(true);
	nAttr.setMin(0.0f);
	nAttr.setMax(100.0f);
	nAttr.setKeyable(true);	//版本号28加入关键桢

	aDoubleSide = nAttr.create("twoSide","ts",MFnNumericData::kBoolean,false,&status);
	CHECK_MSTATUS( status );
	nAttr.setStorable(true);

	aTransparency = nAttr.create("unnamedTransparency","ut",MFnNumericData::kFloat,1.0f);
	CHECK_MSTATUS( status );
	nAttr.setStorable(true);
	nAttr.setMin(0.0f);
	nAttr.setMax(1.0f);
	nAttr.setKeyable(true);

	MFnEnumAttribute eAttr;
	aBlendMode = eAttr.create("blendMode","bm",0);
	eAttr.setStorable(true);
	eAttr.addField("Opaque",0);
	eAttr.addField("Transparent",1);
	eAttr.addField("AlphaBlend",2);
	eAttr.addField("Additive",3);
	eAttr.addField("AdditiveAlpha",4);
	eAttr.addField("Modulate",5);

 	aAdditiveLayer = nAttr.create("additiveLayer","al",MFnNumericData::kBoolean,false,&status);
 	CHECK_MSTATUS( status );
 	nAttr.setStorable(true);

	MFnNumericAttribute attr;
	aTextureRows = attr.create("textureRows","texRows",MFnNumericData::kShort,1);
	attr.setStorable(true);
	attr.setMin(1);
	attr.setMax(16);

	aTextureCols = attr.create("textureCols","texCols",MFnNumericData::kShort,1);
	attr.setStorable(true);
	attr.setMin(1);
	attr.setMax(16);

	aTextureChangeStyle = eAttr.create("textureChangeStyle","texChangeStyle",0);
	eAttr.setStorable(true);
	eAttr.addField("Sequence",0);
	eAttr.addField("Random",1);
	eAttr.addField("Stop",2);
	eAttr.setKeyable(true);	//版本号26加入关键桢

	aTextureChangeInterval = attr.create("textureChangeInterval","texChangeInterval",MFnNumericData::kShort,30);
	attr.setStorable(true);
	attr.setMin(1);
	attr.setMax(5000);


	// Next we will add the attributes we have defined to the node
	//
	CHECK_MSTATUS( addAttribute( aNoDepthCheck) );
	CHECK_MSTATUS( addAttribute( aNoDepthWrite) );
	CHECK_MSTATUS( addAttribute( aUnlit) );
	CHECK_MSTATUS( addAttribute( aUnnamedColor) );
	CHECK_MSTATUS( addAttribute( aDontCastShadow ) );
	CHECK_MSTATUS( addAttribute( aTexcoordFlowU ) );
	CHECK_MSTATUS( addAttribute( aTexcoordFlowV ) );
	CHECK_MSTATUS( addAttribute( aTexcoordRotate ) );
	CHECK_MSTATUS( addAttribute( aTexcoordAngleReset ) );
	CHECK_MSTATUS( addAttribute( aTexcoordPositionReset ) );
	CHECK_MSTATUS( addAttribute( aTexcoordFlowSpeedU ) );
	CHECK_MSTATUS( addAttribute( aTexcoordFlowSpeedV ) );
	CHECK_MSTATUS( addAttribute( aTexcoordRotateSpeed ) );
	CHECK_MSTATUS( addAttribute( aDoubleSide ) );
	CHECK_MSTATUS( addAttribute( aTransparency ) );
	CHECK_MSTATUS( addAttribute( aTextureCols ) );
	CHECK_MSTATUS( addAttribute( aTextureRows ) );
	CHECK_MSTATUS( addAttribute( aTextureChangeStyle ) );
	CHECK_MSTATUS( addAttribute( aTextureChangeInterval ) );
	CHECK_MSTATUS( addAttribute( aBlendMode ) );
	CHECK_MSTATUS( addAttribute( aAdditiveLayer ) );


	return( MS::kSuccess );
}


// The compute() method does the actual work of the node using the inputs
// of the node to generate its output. 
//
// Compute takes two parameters: plug and data.
// - Plug is the the data value that needs to be recomputed
// - Data provides handles to all of the nodes attributes, only these
//   handles should be used when performing computations.
//
MStatus UnnamedShader::compute( const MPlug& plug, MDataBlock& block ) 
{ 
	// The plug parameter will allow us to determine which output attribute
	// needs to be calculated.

	return( MS::kSuccess );
}


// These methods load and unload the plugin, registerNode registers the
// new node type with maya
//

extern "C" __declspec(dllexport) MStatus initializePlugin( MObject obj )
{ 
	const MString	UserClassify( "shader/surface" );

	MString command( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );

	MFnPlugin plugin( obj, "Alias", "4.5", "Any" );

	CHECK_MSTATUS( plugin.registerNode( "UnnamedShader", UnnamedShader::id, 
		UnnamedShader::creator, UnnamedShader::initialize, MPxNode::kDependNode,
		&UserClassify ) );

	command += UserClassify;

	command += "\");}\n";

	CHECK_MSTATUS( MGlobal::executeCommand( command ) );

	return( MS::kSuccess );
}

extern "C" __declspec(dllexport) MStatus uninitializePlugin( MObject obj )
{
	const MString UserClassify( "shader/surface" );

	MString command( "if( `window -exists createRenderNodeWindow` ) {refreshCreateRenderNodeWindow(\"" );

	MFnPlugin plugin( obj );

	CHECK_MSTATUS( plugin.deregisterNode( UnnamedShader::id ) );

	command += UserClassify;
	command += "\");}\n";

	CHECK_MSTATUS( MGlobal::executeCommand( command ) );

	return MS::kSuccess;
}
