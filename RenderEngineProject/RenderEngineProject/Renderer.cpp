#include"Renderer.h"

namespace RenderEngine
{
	Renderer::Renderer(HWND* hWindow, int frameWidth,int frameHeight) : m_hwnd(hWindow), m_frameWidth(frameWidth), m_frameHeight(frameHeight)
	{
		m_rasterizer = nullptr;
		m_renderTarget = nullptr;
		m_camera = nullptr;
	}

	Renderer::Renderer(const Renderer& other) : m_hwnd(other.m_hwnd), m_frameWidth(other.m_frameWidth), m_frameHeight(other.m_frameHeight) 
	{

	}

	Renderer::~Renderer()
	{}

	bool Renderer::Initialize()
	{
		m_renderTarget = new GDIRenderTarget(m_hwnd,m_frameWidth,m_frameHeight);
		if(m_renderTarget == nullptr)
		{
			return false;
		}
		m_renderTarget->Initialize();

		Color* renderTargetBuffer = m_renderTarget->GetColorBuffer();
		int stride = m_renderTarget->GetWidth();

		m_rasterizer = new Rasterizer(renderTargetBuffer,stride);
		if(m_rasterizer == nullptr)
		{
			return false;
		}


		//camera data and initialization  encapsulate and put in windowapp
		Vector3D viewTarget(0.0f,0.0f,-3.0f);
		Vector3D upDirection(0.0f,1.0f,0.0f); 
		Vector3D cameraPosition(0.0,1.0,4.5);

		float fov = PI / 2.0f;
		float aspectRatio = 16.0f / 9.0f;
		float nearPlane = -0.1f;
		float farPlane = -1000.0f;

		m_camera = new Camera(cameraPosition,viewTarget,upDirection,fov,aspectRatio,nearPlane,farPlane,m_frameWidth,m_frameHeight);
		if(m_camera == nullptr)
		{
			return false;
		}
		m_camera->Initialize();
		m_viewTransMatrix = m_camera->ComputeViewTransformMatrix();

		MakeTestScene();

		return true;
	}

	void Renderer::MakeTestScene()
	{
		m_testGeometry = new Vertex[3];
		m_testGeometry[0].m_position = Vector4D(-2.0f,0.2f,3.0f,1);
		m_testGeometry[1].m_position = Vector4D( 2.0f,0.2f,3.0f,1);
		m_testGeometry[2].m_position = Vector4D( 0.0f,1.2f,3.0f,1);

		m_testGeometry[0].m_diffuse.SetColor(250,0,0,255); 
		m_testGeometry[1].m_diffuse.SetColor(0,250,0,255);
		m_testGeometry[2].m_diffuse.SetColor(0,0,250,255);

	}

	void Renderer::Shutdown()
	{
		if(m_rasterizer != nullptr)
		{
			//m_rasterizer->Shutdown();
			delete m_rasterizer;
			m_rasterizer = nullptr;
		}

		if(m_renderTarget != nullptr)
		{
			m_renderTarget->Shutdown();
			delete m_renderTarget;
			m_renderTarget = nullptr;
		}
	}

	bool Renderer::NextFrame()
	{
		//Get current BackBuffer
		ProcessGeometry();
		return true;
	}
	
	void Renderer::ProcessGeometry()
	{
		/*Color* renderTargetBuffer = m_renderTarget->GetColorBuffer();
		m_rasterizer->SetRenderTargetBuffer(renderTargetBuffer);
		m_rasterizer->DrawLine(10,10,300,10,Color(0,255,0,255));
		m_rasterizer->DrawLine(10,10,10,300,Color(0,0,255,255));
		m_rasterizer->DrawLine(300,10,300,300,Color(255,0,0,255));
		m_rasterizer->DrawLine(10,300,300,300,Color(255,255,0,255));
		m_renderTarget->Flip();*/

		Matrix4x4* viewTransform = m_camera->ComputeViewTransformMatrix();
		Matrix4x4* viewportMatrix = m_camera->GetViewportMatrix();


		Vertex endpoints[3];
		endpoints[0] = m_testGeometry[0];
		endpoints[1] = m_testGeometry[1];
		endpoints[2] = m_testGeometry[2];

		endpoints[0].m_position = Vec4MultiMat4x4(*viewTransform,m_testGeometry[0].m_position);
		endpoints[1].m_position = Vec4MultiMat4x4(*viewTransform,m_testGeometry[1].m_position);
		endpoints[2].m_position = Vec4MultiMat4x4(*viewTransform,m_testGeometry[2].m_position);


		for(int i = 0; i < 3; ++i)
		{
			endpoints[i].m_position = endpoints[i].m_position / endpoints[i].m_position.m_w;
			endpoints[i].m_position = Vec4MultiMat4x4(*viewportMatrix,endpoints[i].m_position);
		}

		Color* renderTargetBuffer = m_renderTarget->GetColorBuffer();
		m_rasterizer->SetRenderTargetBuffer(renderTargetBuffer);
		m_rasterizer->DrawTriangle(&endpoints[0],&endpoints[1],&endpoints[2]);
		m_renderTarget->Flip();
	}
}