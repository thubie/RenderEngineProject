#pragma once

#include"LinearMath.h"

namespace RenderEngine
{
	extern const float PI;

	class Camera
	{
	public:
		Camera();
		Camera(Camera& other);
		Camera(Vector3D position,Vector3D viewTarget,Vector3D upVector,float fieldOfView,float aspectRatio,float nearPlane,float farPlane,unsigned int m_frameWidth,unsigned int m_frameHeight);

		bool Initialize();
		Matrix4x4* ComputeViewTransformMatrix();
		Matrix4x4* GetViewportMatrix();
		void SetCameraPosition(Vector3D& position);
		

	private:
		//functions to compute the matrices needed for the view transform.
		void ComputeViewvolumeData();
		void ComputeViewPortMatrix();
		void ComputeCameraMatrix();
		void ComputePerspectiveMatrix();
	
	private:
		//View Volume data
		float m_top;
		float m_bottom;
		float m_left;
		float m_right;

		float m_fieldOfView; //in Radians
		float m_aspectRatio;
		float m_nearPlane;
		float m_farPlane;
		unsigned int m_frameWidth;
		unsigned int m_frameHeight;

		//Camera placement and direction in World coordinate system  
		Vector3D m_position;
		Vector3D m_viewTarget;
		Vector3D m_upVector;
	

		//The canonical coordinate system
		Vector3D m_uAxis;
		Vector3D m_vAxis;
		Vector3D m_wAxis;

		//View transform matrices
		Matrix4x4 m_viewportMatrix;
		Matrix4x4 m_cameraMatrix;
		Matrix4x4 m_perspectiveMatrix;
		Matrix4x4 m_viewTransformMatrix;
	};
}