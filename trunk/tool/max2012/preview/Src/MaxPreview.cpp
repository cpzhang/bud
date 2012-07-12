#include "stdafx.h"
#include "resource.h"
#include "CameraControlThirdPerson.h"
#include "RenderEngine/ParticleSystem.h"

IRenderEngine *g_pRenderEngine = 0;
IRenderSystem *g_pRenderSystem = 0;
Camera*	g_pCamera = 0;
ModelNode*	g_pNode = 0;
Viewport* g_pViewport = 0;
HWND	g_hWnd = 0;
CameraControlThirdPerson*	g_pCameraControl = 0;
rkt::IFont*	g_pFont = 0;
bool g_bShowGround = false;
bool g_bShowBoneAxis = false;
bool g_bShowBones = false;
bool g_b25d = true;
bool g_bShowShadow = true;
Matrix4 g_mtxProjection,g_mtxView45,g_mtxView;
float g_fAnimationSpeed[] = {0.1f,0.2f,0.3f,0.4f,0.5f,0.6f,0.7f,0.8f,0.9f,1.0f,2.0f,3.0f,4.0f,5.0f,6.0f,7.0f,8.0f,9.0f,10.0f,11.0f};
int g_numSpeeds = sizeof(g_fAnimationSpeed) / sizeof(g_fAnimationSpeed[0]);
int g_currentSpeed = 9;
FillMode g_fillMode = FM_SOLID;
bool g_bShowBGImage = true;
const std::string g_bgimages[] = {"001.jpg","002.jpg","003.jpg","004.jpg"/*,"雪地.jpg"*/};
int g_currentBGImage = 0;
ImageRect*	g_pBg = 0;
bool g_bShowNormal = false;

#define WM_PREVIEW	(WM_USER + 128)

void showBones()
{
	ModelInstance *pModelInstance = g_pNode->getModelInstance();
	uint boneCount = pModelInstance->getBoneCount();
	bool bLightingEnabled = g_pRenderEngine->getRenderSystem()->isLightingEnabled();
	g_pRenderEngine->getRenderSystem()->setLightingEnabled(false);
	for(uint i = 0;i < boneCount;i++)
	{
		PosColor pc[2];
		Bone *pBone = pModelInstance->getBone(i);
		const Vector3& v = g_pNode->getFullTransform() * pBone->getTransformedPivot();
		pc[0].pos = v;
		pc[0].color = ColorValue::White.getAsARGB();
		Bone *pParent = pBone->getParent();
		if(pParent)
		{
			Bone *pBone = pModelInstance->getBone(pParent->getName());
			const Vector3& v = g_pNode->getFullTransform() * pBone->getTransformedPivot();
			pc[1].pos = v;
			pc[1].color = ColorValue::White.getAsARGB();
		}
		else
		{
			pc[1].pos = Vector3(0,0,0);
			pc[1].color = ColorValue::White.getAsARGB();
		}
		g_pRenderSystem->setVertexDeclaration(VD_POSCOLOR);
		g_pRenderSystem->drawPrimitiveUP(PT_LINES,sizeof(PosColor),2,(void*)pc);
		g_pRenderSystem->setVertexDeclaration(VD_NULL);
	}
	g_pRenderEngine->getRenderSystem()->setLightingEnabled(bLightingEnabled);
}

void setupMatrix()
{
	if(!g_pNode)return;

	RECT rc;
	GetClientRect(g_hWnd,&rc);

	int width2 = (rc.right - rc.left) / 2;
	int height2 = (rc.bottom - rc.top) / 2;

	OffsetRect(&rc,-width2,-height2);

	g_mtxProjection = Matrix4::orthogonalProjection(rc.left,rc.right,rc.bottom,rc.top,-10000,10000);

	const Vector3& v = g_pNode->getPosition();
	Matrix4 mtx;
	mtx.makeTrans(-v);
	g_mtxView = g_mtxView45 * mtx;

}
void renderAxis()
{
	if(g_pFont)
	{
		Vector3 vx = Vector3(100,0,0);
		Vector3 vy = Vector3(0,100,0);
		Vector3 vz = Vector3(0,0,100);

		ColorValue c = g_pRenderSystem->getColor();
		bool lighting = g_pRenderSystem->isLightingEnabled();
		g_pRenderSystem->setLightingEnabled(false);
		bool depthCheck = g_pRenderSystem->isDepthBufferCheckEnabled();
		g_pRenderSystem->setDepthBufferCheckEnabled(false);
		PosColor v[6] = 
		{
			{Vector3(0,0,0),ColorValue::Red.getAsARGB()},
			{vx,ColorValue::Red.getAsARGB()},
			{Vector3(0,0,0),ColorValue::Green.getAsARGB()},
			{vy,ColorValue::Green.getAsARGB()},
			{Vector3(0,0,0),ColorValue::Blue.getAsARGB()},
			{vz,ColorValue::Blue.getAsARGB()},
		};
		g_pRenderSystem->setVertexDeclaration(VD_POSCOLOR);
		g_pRenderSystem->drawPrimitiveUP(PT_LINES,sizeof(PosColor),6,(void*)v);
		g_pRenderSystem->setVertexDeclaration(VD_NULL);
		g_pRenderSystem->setColor(c);
		g_pRenderSystem->setLightingEnabled(lighting);
		g_pRenderSystem->setDepthBufferCheckEnabled(depthCheck);

		Vector3 vxp = g_pRenderSystem->project(vx + Vector3(1,0,0));
		Vector3 vyp = g_pRenderSystem->project(vy + Vector3(0,1,0));
		Vector3 vzp = g_pRenderSystem->project(vz + Vector3(0,0,1));
		g_pRenderSystem->switchTo2D();
		g_pFont->render2d(vxp.x,vxp.y,-1.0f,ColorValue::Red,"X");
		g_pFont->render2d(vyp.x,vyp.y,-1.0f,ColorValue::Green,"Y");
		g_pFont->render2d(vzp.x,vzp.y,-1.0f,ColorValue::Blue,"Z");
		g_pRenderSystem->switchTo3D();
	}
}


void renderGrids()
{
	if(!g_pNode)return;
	const Vector3& v = g_pNode->getPosition();
#define STEPS	20
	static float step = 32 * Math::fSqrt2;
	int x = v.x / step;
	int z = v.z / step;

	for(int i = x - STEPS;i < x + STEPS;i++)
		for(int j = z - STEPS;j < z + STEPS;j++)
		{
			Vector3 v1 = Vector3(i * step,0,(z - STEPS) * step);
			Vector3 v2 = Vector3(i * step,0,(z + STEPS) * step);
			Vector3 v3 = Vector3((x - STEPS) * step,0,j * step);
			Vector3 v4 = Vector3((x + STEPS) * step,0,j * step);
			PosColor v[4] = 
			{
				{v1,ColorValue::White.getAsARGB()},
				{v2,ColorValue::White.getAsARGB()},
				{v3,ColorValue::White.getAsARGB()},
				{v4,ColorValue::White.getAsARGB()}
			};
			g_pRenderSystem->setVertexDeclaration(VD_POSCOLOR);
			g_pRenderSystem->drawPrimitiveUP(PT_LINES,sizeof(PosColor),4,(void*)v);
			g_pRenderSystem->setVertexDeclaration(VD_NULL);
		}
}

void renderAxis3D()
{
	if(g_pFont)
	{
		Vector3 v0 = Vector3(0,0,0);
		Vector3 vx = Vector3(10,0,0);
		Vector3 vy = Vector3(0,10,0);
		Vector3 vz = Vector3(0,0,10);

		ColorValue c = g_pRenderSystem->getColor();
		bool lighting = g_pRenderSystem->isLightingEnabled();
		g_pRenderSystem->setLightingEnabled(false);
		bool depthCheck = g_pRenderSystem->isDepthBufferCheckEnabled();
		g_pRenderSystem->setDepthBufferCheckEnabled(false);
		g_pRenderSystem->setLineWidth(2.0f);
		PosColor v[6] = 
		{
			{v0,ColorValue::Red.getAsARGB()},
			{vx,ColorValue::Red.getAsARGB()},
			{v0,ColorValue::Green.getAsARGB()},
			{vy,ColorValue::Green.getAsARGB()},
			{v0,ColorValue::Blue.getAsARGB()},
			{vz,ColorValue::Blue.getAsARGB()},
		};
		g_pRenderSystem->setVertexDeclaration(VD_POSCOLOR);
		g_pRenderSystem->drawPrimitiveUP(PT_LINES,sizeof(PosColor),6,(void*)v);
		g_pRenderSystem->setVertexDeclaration(VD_NULL);
		g_pRenderSystem->setColor(c);
		g_pRenderSystem->setLightingEnabled(lighting);
		g_pRenderSystem->setDepthBufferCheckEnabled(depthCheck);

		Vector3 vxp = g_pRenderSystem->project(vx + Vector3(1,0,0));
		Vector3 vyp = g_pRenderSystem->project(vy + Vector3(0,1,0));
		Vector3 vzp = g_pRenderSystem->project(vz + Vector3(0,0,1));
		g_pRenderSystem->switchTo2D();
		g_pFont->render2d(vxp.x,vxp.y,-1.0f,ColorValue::Red,"X");
		g_pFont->render2d(vyp.x,vyp.y,-1.0f,ColorValue::Green,"Y");
		g_pFont->render2d(vzp.x,vzp.y,-1.0f,ColorValue::Blue,"Z");
		g_pRenderSystem->switchTo3D();
	}
}

void renderGrids3D()
{
	bool bLightingEnabled = g_pRenderSystem->isLightingEnabled();
	g_pRenderSystem->setLightingEnabled(false);

	ColorValue c = g_pRenderSystem->getColor();

	Vector3 vx = Vector3(10,0,0);
	Vector3 vy = Vector3(0,10,0);
	Vector3 vz = Vector3(0,0,10);
	Vector3 v0 = Vector3(0,0,0);
#define L	40
	for(int i = 0;i < L;i++)
	{
		if(i == 0)
		{
			if(1)
			{
				PosColor v[8] = 
				{
					{Vector3(-L,0,0),ColorValue(0.6f,0.6f,0.6f).getAsARGB()},
					{Vector3(0,0,0),ColorValue(0.6f,0.6f,0.6f).getAsARGB()},
					{Vector3(vx.x + 1,0,0),ColorValue(0.6f,0.6f,0.6f).getAsARGB()},
					{Vector3(L,0,0),ColorValue(0.6f,0.6f,0.6f).getAsARGB()},
					{Vector3(0,0,-L),ColorValue(0.6f,0.6f,0.6f).getAsARGB()},
					{Vector3(0,0,0),ColorValue(0.6f,0.6f,0.6f).getAsARGB()},
					{Vector3(0,0,vz.z + 1),ColorValue(0.6f,0.6f,0.6f).getAsARGB()},
					{Vector3(0,0,L),ColorValue(0.6f,0.6f,0.6f).getAsARGB()}
				};
				g_pRenderSystem->setVertexDeclaration(VD_POSCOLOR);
				g_pRenderSystem->drawPrimitiveUP(PT_LINES,sizeof(PosColor),8,(void*)v);
				g_pRenderSystem->setVertexDeclaration(VD_NULL);
			}
			else
			{
				PosColor v[4] = 
				{
					{Vector3(-L,0,0),ColorValue(0.6f,0.6f,0.6f).getAsARGB()},
					{Vector3(L,0,0),ColorValue(0.6f,0.6f,0.6f).getAsARGB()},
					{Vector3(0,0,-L),ColorValue(0.6f,0.6f,0.6f).getAsARGB()},
					{Vector3(0,0,L),ColorValue(0.6f,0.6f,0.6f).getAsARGB()},
				};
				g_pRenderSystem->setVertexDeclaration(VD_POSCOLOR);
				g_pRenderSystem->drawPrimitiveUP(PT_LINES,sizeof(PosColor),4,(void*)v);
				g_pRenderSystem->setVertexDeclaration(VD_NULL);
			}
		}
		else
		{
			PosColor v[8] = 
			{
				{Vector3(-L,0,i),ColorValue(0.4f,0.4f,0.4f).getAsARGB()},
				{Vector3(L,0,i),ColorValue(0.4f,0.4f,0.4f).getAsARGB()},
				{Vector3(-L,0,-i),ColorValue(0.4f,0.4f,0.4f).getAsARGB()},
				{Vector3(L,0,-i),ColorValue(0.4f,0.4f,0.4f).getAsARGB()},
				{Vector3(i,0,-L),ColorValue(0.4f,0.4f,0.4f).getAsARGB()},
				{Vector3(i,0,L),ColorValue(0.4f,0.4f,0.4f).getAsARGB()},
				{Vector3(-i,0,-L),ColorValue(0.4f,0.4f,0.4f).getAsARGB()},
				{Vector3(-i,0,L),ColorValue(0.4f,0.4f,0.4f).getAsARGB()}
			};
			g_pRenderSystem->setVertexDeclaration(VD_POSCOLOR);
			g_pRenderSystem->drawPrimitiveUP(PT_LINES,sizeof(PosColor),8,(void*)v);
			g_pRenderSystem->setVertexDeclaration(VD_NULL);
		}
	}
	float lineWidth = g_pRenderSystem->getLineWidth();
	g_pRenderSystem->setLineWidth(2.0f);
	PosColor v[6] = 
	{
		{v0,ColorValue::Red.getAsARGB()},
		{vx,ColorValue::Red.getAsARGB()},
		{v0,ColorValue::Green.getAsARGB()},
		{vy,ColorValue::Green.getAsARGB()},
		{v0,ColorValue::Blue.getAsARGB()},
		{vz,ColorValue::Blue.getAsARGB()},
	};
	g_pRenderSystem->setVertexDeclaration(VD_POSCOLOR);
	g_pRenderSystem->drawPrimitiveUP(PT_LINES,sizeof(PosColor),6,(void*)v);
	g_pRenderSystem->setVertexDeclaration(VD_NULL);
	g_pRenderSystem->setLineWidth(lineWidth);

	g_pRenderSystem->setColor(c);
	g_pRenderSystem->setLightingEnabled(bLightingEnabled);
}

void setBGImage(int index)
{
	safeDelete(g_pBg);
	g_pBg = new ImageRect;
	g_pBg->initialize(g_pRenderSystem,g_bgimages[index],true);
}

INT_PTR CALLBACK PreviewProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
	case WM_INITDIALOG:
		{
			RenderEngineCreationParameters param;
			param.colorDepth = 32;
			param.fullscreen = false;
			param.hwnd = (void*)hWnd;
			param.refreshRate = 85;
			param.rst = RS_D3D9;
			param.stencilBuffer = true;
			RECT rc;
			GetClientRect(hWnd,&rc);
			param.w = rc.right - rc.left;
			param.h = rc.bottom - rc.top;
			g_pRenderEngine = createRenderEngine(&param);

			g_pViewport = new Viewport;
			g_pViewport->setValues(0,0,param.w,param.h);

			g_pRenderSystem = g_pRenderEngine->getRenderSystem();

			g_pCamera = new Camera(g_pViewport);
			g_pCamera->setPosition(Vector3(128,128,128));
			g_pCamera->lookAt(Vector3(0,0,0));
			g_pCamera->setFarDistance(99999999);

			SetTimer(hWnd,0,16,0);

			getFileSystem()->addFindPath("");

			g_pCameraControl = new CameraControlThirdPerson(g_pCamera,g_pRenderEngine->getRenderSystem());

			g_pFont = g_pRenderEngine->getFontManager()->createFont(g_pRenderEngine->getRenderSystem(),FONT_TYPE_WINDOWS,"宋体",12,FONT_PROP_OUTLINE);

			rkt::ParticleSystem::set25dMode(g_b25d);

			getIoRequestManager()->setForceSynchronous(true);
			//不缓存，因为修改了Max.mz文件后，不会更新
			getCacheManager()->setMaxPoolSize(0);

			g_pRenderEngine->getRenderSystem()->setClearColor(ColorValue(0.223f,0.427f,0.647f));
			g_pRenderEngine->getRenderSystem()->setLightingEnabled(true);
			g_pRenderEngine->getRenderSystem()->setAmbientLight(ColorValue(1,1,1,1));
			g_pRenderEngine->getRenderSystem()->setViewport(0,0,param.w,param.h);
			g_pRenderSystem->switchTo2D();
			g_pRenderSystem->setSceneBlending(SBF_SOURCE_ALPHA,SBF_ONE_MINUS_SOURCE_ALPHA);
			g_pRenderSystem->switchTo3D();

			SurfaceParam sp;
			sp.ambient = ColorValue(1,1,1,1);
			sp.diffuse = ColorValue(1,1,1,1);
			sp.specular = ColorValue(0,0,0);
			sp.emissive = ColorValue(0,0,0);
			sp.shininess = 0;
			g_pRenderSystem->setSurfaceParam(sp);
			g_pRenderSystem->setAmbientLight(ColorValue(0.85f,0.85f,0.85f));

			//
			Matrix4 mtxView;
			mtxView.rotationX(30);
			Matrix4 mtxView1;
			mtxView1.rotationY(45);
			mtxView = mtxView * mtxView1;
			g_mtxView45 = mtxView;
			g_mtxView = g_mtxView45;

			setBGImage(g_currentBGImage);
		}
		return TRUE;
	case WM_DESTROY:
		{
			safeDelete(g_pBg);
			safeRelease(g_pFont);
			safeDelete(g_pCameraControl);
			KillTimer(hWnd,0);
			safeRelease(g_pNode);
			safeRelease(g_pViewport);
			safeRelease(g_pCamera);
			ICacheManager *pCacheManager = getCacheManager();
			pCacheManager->release();
			safeRelease(g_pRenderEngine);
			g_hWnd = 0;
			PostQuitMessage(0);
		}
		break;
	case WM_CLOSE:
		DestroyWindow(hWnd);
		return TRUE;
	case WM_TIMER:
		{
			if(g_b25d)
			{
				setupMatrix();
				g_pRenderEngine->getRenderSystem()->setProjectionMatrix(g_mtxProjection);
				g_pRenderEngine->getRenderSystem()->setViewMatrix(g_mtxView);
			}
			else
			{
				g_pCamera->update();
				g_pRenderEngine->getRenderSystem()->setProjectionMatrix(g_pCamera->getProjectionMatrix());
				g_pRenderEngine->getRenderSystem()->setViewMatrix(g_pCamera->getViewMatrix() * g_pCameraControl->getWorldMatrix());
			}
			g_pRenderEngine->getRenderSystem()->setWorldMatrix(Matrix4::IDENTITY);
			float tick = GetTickCount();
			static float tickOrigin = tick;
			tick = tickOrigin + (tick - tickOrigin) * g_fAnimationSpeed[g_currentSpeed];
			
			g_pRenderEngine->getRenderSystem()->beginFrame(true,true,true);
			if(g_bShowBGImage)
			{
				g_pRenderSystem->switchTo2D();
				int left,top,width,height;
				g_pRenderSystem->getViewport(left,top,width,height);
				int x = (width - (int)g_pBg->getWidth()) / 2;
				int y = (height - (int)g_pBg->getHeight()) / 2;
				g_pBg->render(x,y);
				g_pRenderSystem->switchTo3D();
			}
			if(g_bShowGround)
			{
				g_b25d ? renderGrids() : renderGrids3D();
			}
			if(g_pNode)
			{
				g_pNode->update(tick,(tick - tickOrigin),g_pRenderEngine->getRenderSystem());
				if(g_bShowShadow)
				{
					bool depthCheck = g_pRenderSystem->isDepthBufferCheckEnabled();
					bool depthWrite = g_pRenderSystem->isDepthBufferWriteEnabled();
					bool lighting = g_pRenderSystem->isLightingEnabled();
					SceneBlendFactor src,dst;
					g_pRenderSystem->getSceneBlending(src,dst);

					g_pRenderSystem->setSceneBlending(SBF_SOURCE_ALPHA,SBF_ONE_MINUS_SOURCE_ALPHA);
					g_pRenderSystem->setDepthBufferCheckEnabled(false);
					g_pRenderSystem->setDepthBufferWriteEnabled(false);
					//g_pRenderSystem->setLightingEnabled(false);

					static Vector3 gLD(-1.0f,2.5f,0.0f);
					Vector3 lightDirection = gLD;
					Matrix4 mtxWorld = g_pRenderSystem->getWorldMatrix();
					Matrix4 mtxShadow = Matrix4::IDENTITY;
					mtxShadow[1][0] = -lightDirection.x / lightDirection.y;
					mtxShadow[1][1] = 0;
					mtxShadow[1][2] = -lightDirection.z / lightDirection.y;

					ColorValue c = g_pRenderSystem->getColor();
					g_pRenderSystem->setColor(ColorValue(0.0,0.0,0.0,0.3f));
					SurfaceParam sp;
					g_pRenderSystem->getSurfaceParam(&sp);
					SurfaceParam spNew = sp;
					spNew.diffuse = ColorValue(0.0,0.0,0.0,0.3f);
					spNew.ambient = ColorValue::Black;
					g_pRenderSystem->setSurfaceParam(spNew);
					//
					g_pRenderSystem->setStencilCheckEnabled(true);
					g_pRenderSystem->setStencilBufferParams(CMPF_EQUAL,0,0xFFFFFFFF,SOP_KEEP,SOP_KEEP,SOP_INCREMENT);

					g_pRenderSystem->setWorldMatrix(mtxWorld * mtxShadow);
					g_pNode->render(g_pRenderSystem,false);
					g_pRenderSystem->setWorldMatrix(mtxWorld);
					g_pRenderSystem->setStencilCheckEnabled(false);
					g_pRenderSystem->setStencilBufferParams();
					g_pRenderSystem->setSurfaceParam(sp);
					g_pRenderSystem->setColor(c);

					g_pRenderSystem->setSceneBlending(src,dst);
					g_pRenderSystem->setLightingEnabled(lighting);
					g_pRenderSystem->setDepthBufferCheckEnabled(depthCheck);
					g_pRenderSystem->setDepthBufferWriteEnabled(depthWrite);
				}
				g_pRenderEngine->getRenderSystem()->clearBatches();
				g_pRenderEngine->getRenderSystem()->setBatchesAvailable(true);
				g_pNode->render(g_pRenderEngine->getRenderSystem());
				g_pRenderEngine->getRenderSystem()->setBatchesAvailable(false);
				if(g_bShowBones)showBones();
			}
			if(g_bShowGround)
			{
				g_b25d ? renderAxis() : renderAxis3D();
			}
			if(g_pNode && g_bShowBoneAxis)
			{
				Matrix4 mtxWorld = g_pRenderSystem->getWorldMatrix();
				for(uint i = 0;i < g_pNode->getModelInstance()->getBoneCount();i++)
				{
					Matrix4 mtxTranslation = Matrix4::IDENTITY;
					mtxTranslation.setTrans(g_pNode->getModelInstance()->getBone(i)->getTransformedPivot());
					Matrix4 mtxRotation = g_pNode->getModelInstance()->getBone(i)->getFullRotation();
					mtxRotation = mtxTranslation * mtxRotation;
					g_pRenderSystem->setWorldMatrix(mtxRotation);
					g_b25d ? renderAxis() : renderAxis3D();
				}
				g_pRenderSystem->setWorldMatrix(mtxWorld);
			}
			if(g_bShowNormal && g_pNode)
			{
				bool depthWrite = g_pRenderSystem->isDepthBufferWriteEnabled();
				bool depthCheck = g_pRenderSystem->isDepthBufferCheckEnabled();
				g_pRenderSystem->setDepthBufferCheckEnabled(true);
				g_pRenderSystem->setDepthBufferWriteEnabled(true);
				bool lighting = g_pRenderSystem->isLightingEnabled();
				g_pRenderSystem->setLightingEnabled(false);
				Matrix4 mtxWorld = g_pRenderSystem->getWorldMatrix();
				g_pRenderSystem->setWorldMatrix(g_pNode->getFullTransform());
				IVertexBuffer *pVB = g_pNode->getModelInstance()->getVertexBuffer();
				if (pVB)
				{
					Vector3 *pVector3 = (Vector3*)pVB->lock(0,0,BL_NORMAL);
					for(uint i = 0;i < g_pNode->getModelInstance()->getNumVertices();i++)
					{
						PosColor v[2] = 
						{
							{*(pVector3 + i * 2),0xFFFFFFFF},
							{*(pVector3 + i * 2) + 8 * *(pVector3 + i * 2 + 1),0xFFFFFFFF}
						};
						g_pRenderSystem->setVertexDeclaration(VD_POSCOLOR);
						g_pRenderSystem->drawPrimitiveUP(PT_LINES,16,2,(void*)v);
						g_pRenderSystem->setVertexDeclaration(VD_NULL);
					}
					pVB->unlock();
				}
				g_pRenderSystem->setWorldMatrix(mtxWorld);
				g_pRenderSystem->setLightingEnabled(lighting);
				g_pRenderSystem->setDepthBufferCheckEnabled(depthCheck);
				g_pRenderSystem->setDepthBufferWriteEnabled(depthWrite);
			}
			g_pRenderSystem->switchTo2D();
			g_pFont->render2d(0,0,0,ColorValue::Green,L"G 显示坐标系和地表    M 显示背景图片    A~D 旋转    PageDown~Up 换背景    N 法线");
			g_pFont->render2d(0,16,0,ColorValue::Green,L"B 显示骨骼    L 显示骨骼局部坐标系");
			g_pFont->render2d(0,32,0,ColorValue::Green,L"C 切换2.5D和3D渲染模式    Z 显示网格模式");
			if(g_pNode)
			{
				autostring wstr;
				char str[256];
				sprintf(str,"  骨骼数=%d",g_pNode->getModelInstance()->getBoneCount());
				wstr = str;
				g_pFont->render2d(0,60,0,ColorValue::White,wstr.c_wstr());
				sprintf(str,"  顶点数=%d",g_pNode->getModelInstance()->getNumVertices());
				wstr = str;
				g_pFont->render2d(0,76,0,ColorValue::White,wstr.c_wstr());
				sprintf(str,"  面数=%d",g_pNode->getModelInstance()->getNumFaces());
				wstr = str;
				g_pFont->render2d(0,92,0,ColorValue::White,wstr.c_wstr());
				sprintf(str,"  子模型数=%d",g_pNode->getModelInstance()->getNumSubModels());
				wstr = str;
				g_pFont->render2d(0,108,0,ColorValue::White,wstr.c_wstr());
				sprintf(str,"  版本号=%d",g_pNode->getModelInstance()->getVer());
				wstr = str;
				g_pFont->render2d(0,124,0,ColorValue::White,wstr.c_wstr());
				sprintf(str,"  粒子系统数=%d",g_pNode->getModelInstance()->getModel()->numParticleSystem());
				wstr = str;
				g_pFont->render2d(0,140,0,ColorValue::White,wstr.c_wstr());
				sprintf(str,"  带子系统数=%d",g_pNode->getModelInstance()->getModel()->numRibbonSystem());
				wstr = str;
				g_pFont->render2d(0,156,0,ColorValue::White,wstr.c_wstr());

				{
					sprintf(str,"  batches = %d",g_pRenderSystem->getBatches());
					wstr = str;
					g_pFont->render2d(0,172,0,ColorValue::White,wstr.c_wstr());
				}
				if(g_b25d)
				{
					sprintf(str,"A 向左旋转");
					wstr = str;
					g_pFont->render2d(0,188,0,ColorValue::Green,wstr.c_wstr());
					sprintf(str,"D 向右旋转");
					wstr = str;
					g_pFont->render2d(0,204,0,ColorValue::Green,wstr.c_wstr());
				}
			}
			g_pRenderSystem->switchTo3D();
			g_pRenderEngine->getRenderSystem()->endFrame();
			tickOrigin = tick;
		}
		break;
	case WM_SIZE:
		{
			short width = LOWORD(lParam);
			short height = HIWORD(lParam);
			if(width == 0 || height == 0)return TRUE;

			g_pRenderEngine->invalidateDeviceObjects();
			g_pViewport->setValues(0,0,width,height);
			RenderEngineCreationParameters param = g_pRenderEngine->getCreationParams();
			param.w = width;
			param.h = height;
			g_pRenderEngine->switchDisplayMode(&param);
			g_pRenderEngine->getRenderSystem()->setViewport(0,0,width,height);
			g_pRenderEngine->getRenderSystem()->setClearColor(ColorValue(0.223f,0.427f,0.647f));
			g_pRenderEngine->getRenderSystem()->setLightingEnabled(true);
			g_pRenderEngine->getRenderSystem()->setAmbientLight(ColorValue(1,1,1,1));
			g_pRenderSystem->switchTo2D();
			g_pRenderSystem->setSceneBlending(SBF_SOURCE_ALPHA,SBF_ONE_MINUS_SOURCE_ALPHA);
			g_pRenderSystem->switchTo3D();
			SurfaceParam sp;
			sp.ambient = ColorValue(1,1,1,1);
			sp.diffuse = ColorValue(1,1,1,1);
			sp.specular = ColorValue(0,0,0);
			sp.emissive = ColorValue(0,0,0);
			sp.shininess = 0;
			g_pRenderSystem->setSurfaceParam(sp);
			g_pRenderSystem->setAmbientLight(ColorValue::White);
			g_pRenderEngine->restoreDeviceObjects();
		}
		break;
	case WM_PREVIEW:
		{
			safeRelease(g_pNode);

			char tempPath[MAX_PATH];
			GetTempPath(MAX_PATH, tempPath);
			rkt::CPath tempFileName(tempPath);
			tempFileName.addTailSlash();
			tempFileName += "max.mz";

			g_pNode = ModelNodeCreater::create(tempFileName.c_str(),0,0);
			if(g_pNode)
			{
				g_pNode->getModelInstance()->setCurrentAnimation("Animation");
			}
			SetForegroundWindow(g_hWnd);
			BringWindowToTop(g_hWnd);
		}
		break;
	case WM_KEYDOWN:
		{
			static float angle = 0;
			Quaternion q;
			g_pCameraControl->onKeyDown(wParam);
			switch(wParam)
			{
			case 'S':
				g_bShowShadow = !g_bShowShadow;
				break;
			case 'M':
				g_bShowBGImage = !g_bShowBGImage;
				break;
			case 'G':
				g_bShowGround = !g_bShowGround;
				break;
			case 'B':
				g_bShowBones = !g_bShowBones;
				break;
			case 'C':
				g_b25d = !g_b25d;
				rkt::ParticleSystem::set25dMode(g_b25d);
				break;
			case 'Z':
				if(g_fillMode == FM_SOLID)
					g_fillMode = FM_WIREFRAME;
				else if(g_fillMode == FM_WIREFRAME)
					g_fillMode = FM_SOLID;
				g_pRenderSystem->setFillMode(g_fillMode);
				break;
			case 'A':
				{
					if (g_pNode)
					{
						angle -= 45;
						q.FromAngleAxis(angle, Vector3(0,1,0));
						g_pNode->setOrientation(q);
					}
				}
				break;
			case 'D':
				{
					if (g_pNode)
					{
						angle += 45;
						q.FromAngleAxis(angle, Vector3(0,1,0));
						g_pNode->setOrientation(q);
					}
				}
				break;
			case 'L':
				g_bShowBoneAxis = !g_bShowBoneAxis;
				break;
			case 'T':
				::SendMessage(g_hWnd,WM_PREVIEW,0,0);
				break;
			case VK_PRIOR:
				{
					g_currentBGImage--;
					if(g_currentBGImage < 0)g_currentBGImage = sizeof(g_bgimages) / sizeof(g_bgimages[0]) - 1;
					setBGImage(g_currentBGImage);
				}
				break;
			case VK_NEXT:
				{
					g_currentBGImage++;
					if(g_currentBGImage >= sizeof(g_bgimages) / sizeof(g_bgimages[0]))g_currentBGImage = 0;
					setBGImage(g_currentBGImage);
				}
				break;
			case 'N':
				g_bShowNormal = !g_bShowNormal;
				break;
			/*case VK_LEFT:
				{
					g_currentSpeed--;
					if(g_currentSpeed < 0)g_currentSpeed = g_numSpeeds - 1;
				}
				break;
			case VK_RIGHT:
				{
					g_currentSpeed++;
					if(g_currentSpeed == g_numSpeeds)g_currentSpeed = 0;
				}
				break;*/
			}
		}
		break;
	case WM_LBUTTONDOWN:
		{
			SetCapture(hWnd);
			ushort x = LOWORD(lParam);
			ushort y = HIWORD(lParam);
			g_pCameraControl->onLeftButtonDown(x,y);
		}
		break;
	case WM_LBUTTONUP:
		{
			ReleaseCapture();
			ushort x = LOWORD(lParam);
			ushort y = HIWORD(lParam);
			g_pCameraControl->onLeftButtonUp(x,y);
		}
		break;
	case WM_MOUSEMOVE:
		{
			ushort x = LOWORD(lParam);
			ushort y = HIWORD(lParam);
			g_pCameraControl->onMouseMove(x,y);
		}
		break;
	case WM_RBUTTONDOWN:
		{
			SetCapture(hWnd);
			ushort x = LOWORD(lParam);
			ushort y = HIWORD(lParam);
			g_pCameraControl->onRightButtonDown(x,y);
		}
		break;
	case WM_RBUTTONUP:
		{
			ReleaseCapture();
			ushort x = LOWORD(lParam);
			ushort y = HIWORD(lParam);
			g_pCameraControl->onRightButtonUp(x,y);
		}
		break;
	case WM_MBUTTONDOWN:
		{
			SetCapture(hWnd);
			ushort x = LOWORD(lParam);
			ushort y = HIWORD(lParam);
			g_pCameraControl->onMButtonDown(x,y);
		}
		break;
	case WM_MBUTTONUP:
		{
			ReleaseCapture();
			ushort x = LOWORD(lParam);
			ushort y = HIWORD(lParam);
			g_pCameraControl->onMButtonUp(x,y);
		}
		break;
	case WM_MOUSEWHEEL:
		{
			short delta = HIWORD(wParam);
			g_pCameraControl->onMouseWheel(delta);
		}
		break;
	}

	return FALSE;
}

#ifdef RELEASEDEBUG
	#define DLL_NAME "MaxPreview_rd.exe"
#elif _DEBUG
	#define DLL_NAME "MaxPreview_d.exe"
#else
	#define DLL_NAME "MaxPreview.exe"
#endif

int __stdcall WinMain(__in HINSTANCE hInstance, __in_opt HINSTANCE hPrevInstance, __in_opt LPSTR lpCmdLine, __in int nShowCmd)
{
	g_hWnd = CreateDialogParamA(GetModuleHandle(DLL_NAME),MAKEINTRESOURCE(IDD_PREVIEW),GetDesktopWindow(),PreviewProc,0);
	ShowWindow(g_hWnd,SW_SHOW);
	//SendMessage(g_hWnd,WM_PREVIEW,(WPARAM)"D:\\Rocket\\Bin\\Client\\Test\\tta.Mz",0);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}