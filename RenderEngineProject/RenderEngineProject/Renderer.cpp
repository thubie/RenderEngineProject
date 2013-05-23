#include"Renderer.h"

namespace RenderEngine
{
	Renderer::Renderer(HWND* hWindow, int frameWidth,int frameHeight) : m_hwnd(hWindow), m_frameWidth(frameWidth), m_frameHeight(frameHeight)
	{
		m_rasterizer = nullptr;
		m_renderTarget = nullptr;
		m_camera = nullptr;
		m_importer = nullptr;
		m_scene = nullptr;
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
		Vector3D viewTarget(0.0f,1.5f,0.0f);
		Vector3D upDirection(0.0f,1.0f,0.0f); 
		Vector3D cameraPosition(0.0,2.0,2.01);

		float fov = PI / 2.0f;
		float aspectRatio = 16.0f / 9.0f;
		float nearPlane = -0.1f;
		float farPlane = -400.0f;

		m_camera = new Camera(cameraPosition,viewTarget,upDirection,fov,aspectRatio,nearPlane,farPlane,m_frameWidth,m_frameHeight);
		if(m_camera == nullptr)
		{
			return false;
		}
		m_camera->Initialize();
		m_viewTransMatrix = m_camera->ComputeViewTransformMatrix();

		m_importer = new ImporterOBJ("head.obj");
		if(m_importer == nullptr)
		{
			return false;
		}
		if(!m_importer->ProcessFile())
			return false;

		m_scene = m_importer->GetScene();

		return true;
	}

	void Renderer::Shutdown()
	{
		if(m_scene != nullptr)
		{
			m_scene->clear();
			m_scene = nullptr;
		}
		
		if(m_importer != nullptr)
		{
			delete m_importer;
			m_importer = nullptr;
		}
		
		if(m_camera != nullptr)
		{
			delete m_camera;
			m_camera = nullptr;
		}

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
		ProcessGeometry();
		return true;
	}
	
	void Renderer::ProcessGeometry()
	{
		int i,j,k,l;
		int objects = m_scene->size();
		int faceCount = 0;
		int index1,index2,index3;
		Matrix4x4* viewTransform = m_camera->ComputeViewTransformMatrix();
		Matrix4x4* viewportMatrix = m_camera->GetViewportMatrix();
		Vertex endpoints[3];
		Vertex* objectVertexData;
		Face* objectIndices;
		Color* renderTargetBuffer = m_renderTarget->GetColorBuffer();
		m_rasterizer->SetRenderTargetBuffer(renderTargetBuffer);
		Color red(255,0,0,0);
		ObjectData tempObjData;

		for(i = 0; i < objects; ++i)
		{
			objectVertexData = m_scene->at(i).vertices;
			objectIndices = m_scene->at(i).faces;
			faceCount = m_scene->at(i).faceCount;

			for(j = 0; j < faceCount; ++j)
			{
				endpoints[0] = objectVertexData[objectIndices[j].indices[0]];
				endpoints[1] = objectVertexData[objectIndices[j].indices[1]];
				endpoints[2] = objectVertexData[objectIndices[j].indices[2]];

				endpoints[0].position = Vec4MultiMat4x4(*viewTransform,endpoints[0].position);
				endpoints[1].position = Vec4MultiMat4x4(*viewTransform,endpoints[1].position);
				endpoints[2].position = Vec4MultiMat4x4(*viewTransform,endpoints[2].position);

				for(k = 0; k < 3; ++k)
				{
					//endpoints[k].perW /= endpoints[k].position.m_w;
					endpoints[k].position = endpoints[k].position / endpoints[k].position.m_w;
					endpoints[k].position = Vec4MultiMat4x4(*viewportMatrix,endpoints[k].position);
					endpoints[k].diffuse = red;
				}				
				m_rasterizer->DrawTriangleLine(&endpoints[0],&endpoints[1],&endpoints[2]);
				m_rasterizer->DrawTriangle(&endpoints[0],&endpoints[1],&endpoints[2]);
			}
		}
		m_renderTarget->Flip();
	}
}