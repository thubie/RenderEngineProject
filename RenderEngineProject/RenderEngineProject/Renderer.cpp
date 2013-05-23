#include"Renderer.h"

namespace RenderEngine
{
	Renderer::Renderer(HWND* hWindow, int frameWidth,int frameHeight) : m_hwnd(hWindow), m_frameWidth(frameWidth), m_frameHeight(frameHeight)
	{
		m_rasterizer = nullptr;
		m_renderTarget = nullptr;
		m_camera = nullptr;
		m_importer = nullptr;
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
		Vector3D viewTarget(0.0f,0.0f,0.0f);
		Vector3D upDirection(0.0f,1.0f,0.0f); 
		Vector3D cameraPosition(0.0,0.0,3.0);

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

		m_importer = new ImporterOBJ("testScene2.obj");
		if(m_importer == nullptr)
		{
			return false;
		}
		m_importer->ProcessFile();

		MakeTestScene();

		return true;
	}

	void Renderer::MakeTestScene()
	{
		m_testGeometry = new Vertex[4];
		m_testGeometry[0].position = Vector4D(-1.0f, 1.0f, 1.5f, 1);
		m_testGeometry[1].position = Vector4D(-1.0f, -1.0f, 1.5f, 1);
		m_testGeometry[2].position = Vector4D( 1.0f, -1.0f, 1.5f, 1);
		m_testGeometry[3].position = Vector4D( 1.0f, 1.0f, 1.5f, 1);

		m_testGeometry[0].diffuse.SetColor(255,0,0,0); 
		m_testGeometry[1].diffuse.SetColor(0,255,0,0);
		m_testGeometry[2].diffuse.SetColor(0,0,255,0);
		m_testGeometry[3].diffuse.SetColor(255,255,0,0);

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
		Matrix4x4* viewTransform = m_camera->ComputeViewTransformMatrix();
		Matrix4x4* viewportMatrix = m_camera->GetViewportMatrix();

		Vertex endpoints[4];
		endpoints[0] = m_testGeometry[0];
		endpoints[1] = m_testGeometry[1];
		endpoints[2] = m_testGeometry[2];
		endpoints[3] = m_testGeometry[3];

		endpoints[0].position = Vec4MultiMat4x4(*viewTransform,m_testGeometry[0].position);
		endpoints[1].position = Vec4MultiMat4x4(*viewTransform,m_testGeometry[1].position);
		endpoints[2].position = Vec4MultiMat4x4(*viewTransform,m_testGeometry[2].position);
		endpoints[3].position = Vec4MultiMat4x4(*viewTransform,m_testGeometry[3].position);

		for(int i = 0; i < 4; ++i)
		{
			endpoints[i].perW /= endpoints[i].position.m_w;
			endpoints[i].position = endpoints[i].position / endpoints[i].position.m_w;
			//endpoints[i].diffuse.m_r /= endpoints[i].m_perW;
			//endpoints[i].diffuse.m_g /= endpoints[i].m_perW;
			//endpoints[i].diffuse.m_b /= endpoints[i].m_perW;
			endpoints[i].position = Vec4MultiMat4x4(*viewportMatrix,endpoints[i].position);
		}

		Color* renderTargetBuffer = m_renderTarget->GetColorBuffer();
		m_rasterizer->SetRenderTargetBuffer(renderTargetBuffer);
		m_rasterizer->DrawTriangle(&endpoints[2],&endpoints[0],&endpoints[1]);
		m_rasterizer->DrawTriangle(&endpoints[0],&endpoints[2],&endpoints[3]);
		m_renderTarget->Flip();
	}
}