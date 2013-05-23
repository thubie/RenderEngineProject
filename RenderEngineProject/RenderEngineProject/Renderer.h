#pragma once

//#include<Windows.h>
#include"GDIRenderTarget.h"
#include"Rasterizer.h"
#include"Camera.h"
#include"ImporterOBJ.h"

namespace RenderEngine
{
	class Renderer
	{
	public:
		Renderer(HWND* hWindow,int frameWidth,int frameHeight);
		Renderer(const Renderer& other);
		~Renderer();

		bool Initialize();
		void Shutdown();
		bool NextFrame();

	private:
		void ProcessGeometry();
		void MakeTestScene();

	private:
		int m_frameWidth;
		int m_frameHeight;

		HWND *m_hwnd;
		Camera *m_camera;
		GDIRenderTarget *m_renderTarget;
		std::vector<Model>* m_scene;
		Rasterizer *m_rasterizer;
		Matrix4x4* m_viewTransMatrix;
		ImporterOBJ* m_importer;
	};
}