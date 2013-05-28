#include"Renderer.h"

namespace RenderEngine
{
	Renderer::Renderer(HWND* hWindow, int frameWidth,int frameHeight) : m_hwnd(hWindow), m_frameWidth(frameWidth), m_frameHeight(frameHeight),m_yDegrees(0), m_yDegreesIncre(30.0)
	{
		m_rasterizer = nullptr;
		m_renderTarget = nullptr;
		m_camera = nullptr;
		m_importer = nullptr;
		m_scene = nullptr;
		m_HRTimer = nullptr;
	}

	Renderer::Renderer(const Renderer& other) : m_hwnd(other.m_hwnd), m_frameWidth(other.m_frameWidth), m_frameHeight(other.m_frameHeight), m_yDegreesIncre(2.0)
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
		Vector3D viewTarget(0.0f,0.0,0.0f);
		Vector3D upDirection(0.0f,1.0f,0.0f); 
		Vector3D cameraPosition(0.0,3.0f,0.000001f);

		float fov = PI / 2.0f;
		float aspectRatio = 16.0f / 9.0f;
		float nearPlane = -0.01f;
		float farPlane = -100.0f;

		m_camera = new Camera(cameraPosition,viewTarget,upDirection,fov,aspectRatio,nearPlane,farPlane,m_frameWidth,m_frameHeight);
		if(m_camera == nullptr)
		{
			return false;
		}
		m_camera->Initialize();
		m_viewTransMatrix = m_camera->ComputeViewTransformMatrix();

		m_importer = new ImporterOBJ("plane.obj");
		if(m_importer == nullptr)
		{
			return false;
		}
		if(!m_importer->ProcessFile())
			return false;

		m_scene = m_importer->GetScene();

		m_HRTimer = new HRTimer();
		if(m_HRTimer == nullptr)
			return false;

		return true;
	}

	void Renderer::Shutdown()
	{

		if(m_HRTimer != nullptr)
		{
			delete m_HRTimer;
			m_HRTimer = nullptr;
		}

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
		m_HRTimer->Start();
		ProcessGeometry();		
		m_HRTimer->Stop();
		m_elapsedSeconds = m_HRTimer->GetElapsedAsSeconds(); 
		m_yDegrees += m_yDegreesIncre * m_elapsedSeconds;
		return true;
	}
	
	void Renderer::ProcessGeometry()
	{
		int i,j,k,l;
		int objects = m_scene->size();
		int faceCount = 0;
		int vertexCount = 0;
		int index1,index2,index3;
		
		Matrix4x4* viewTransform = m_camera->ComputeViewTransformMatrix();
		Matrix4x4* viewportMatrix = m_camera->GetViewportMatrix();
		Matrix4x4 ModelTransform;
		RotationYMatrix(ModelTransform,m_yDegrees);
		Vertex endpoints[3];
		Vertex* objectVertexData;
		Face* objectIndices;
		m_rasterizer->SetRenderTargetBuffer( m_renderTarget->GetColorBuffer());
		m_rasterizer->SetZBuffer(m_renderTarget->GetDepthBuffer());

		for(i = 0; i < objects; ++i)
		{
			objectVertexData = m_scene->at(i).vertices;
			objectIndices = m_scene->at(i).faces;
			faceCount = m_scene->at(i).faceCount;
			vertexCount = m_scene->at(i).faceCount;

			for(j = 0; j < faceCount; ++j)
			{
				
				endpoints[0] = objectVertexData[objectIndices[j].indices[0]];
				endpoints[1] = objectVertexData[objectIndices[j].indices[1]];
				endpoints[2] = objectVertexData[objectIndices[j].indices[2]];
				
				endpoints[0].position = Vec4MultiMat4x4(ModelTransform,endpoints[0].position);
				endpoints[1].position = Vec4MultiMat4x4(ModelTransform,endpoints[1].position);
				endpoints[2].position = Vec4MultiMat4x4(ModelTransform,endpoints[2].position);
				
				endpoints[0].position = Vec4MultiMat4x4(*viewTransform,endpoints[0].position);
				endpoints[1].position = Vec4MultiMat4x4(*viewTransform,endpoints[1].position);
				endpoints[2].position = Vec4MultiMat4x4(*viewTransform,endpoints[2].position);

				for(k = 0; k < 3; ++k)
				{
					endpoints[k].normal = endpoints[k].normal / endpoints[k].position.m_w;
					//endpoints[k].uv.u /= endpoints[k].position.m_w;
					//endpoints[k].uv.v /= endpoints[k].position.m_w;
					endpoints[k].diffuse = endpoints[k].diffuse / endpoints[k].position.m_w;
					endpoints[k].perW /= endpoints[k].position.m_w;
					endpoints[k].position = endpoints[k].position / endpoints[k].position.m_w;
					endpoints[k].position = Vec4MultiMat4x4(*viewportMatrix,endpoints[k].position);	
				}
				//backface Culling
				float area = 0.5f * ((endpoints[2].position.m_x - endpoints[1].position.m_x) * 
					(endpoints[0].position.m_y - endpoints[2].position.m_y) - (endpoints[2].position.m_y - endpoints[1].position.m_y) * 
					(endpoints[0].position.m_x - endpoints[2].position.m_x));
				if(area >= 0)
					continue;

				m_rasterizer->DrawTriangle(endpoints[0],endpoints[1],endpoints[2]);
				//m_rasterizer->DrawTriangleLine(endpoints[0],endpoints[1],endpoints[2]);
			}
		}
		m_renderTarget->Flip();		
	}
}