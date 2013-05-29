#include"Camera.h"

namespace RenderEngine
{
	

	//Default constructor will create a camera at position (0,0,0) with a 90 degrees field of view and 16:9 aspect ratio
	Camera::Camera() : m_position(Vector3D(0,0,0)), m_viewTarget(Vector3D(0,0,-5)), m_upVector(Vector3D(0,1,0)), 
		m_fieldOfView(PI/2.0f),m_aspectRatio(1.7777f),m_nearPlane(0.1f),m_farPlane(1000.0f),m_frameWidth(1280),m_frameHeight(720),
		m_viewportMatrix(), m_cameraMatrix(), m_perspectiveMatrix(),m_viewTransformMatrix()
	{}

	//copy constructor
	Camera::Camera(Camera& other) : m_position(other.m_position), m_viewTarget(other.m_viewTarget), m_upVector(other.m_upVector), 
		m_fieldOfView(other.m_fieldOfView),m_aspectRatio(other.m_aspectRatio),m_nearPlane(other.m_nearPlane),m_farPlane(other.m_farPlane), 
		m_frameWidth(other.m_frameWidth),m_frameHeight(other.m_frameHeight), 
		m_viewportMatrix(other.m_viewportMatrix), m_cameraMatrix(other.m_cameraMatrix), m_perspectiveMatrix(other.m_perspectiveMatrix),m_viewTransformMatrix(other.m_viewTransformMatrix)
	{}

	//constructor
	Camera::Camera(Vector3D position,Vector3D viewTarget,Vector3D upVector,float fieldOfView,float aspectRatio,float nearPlane,float farPlane,unsigned int frameWidth,unsigned int frameHeight) :
		m_position(position), m_viewTarget(viewTarget), m_upVector(upVector), m_fieldOfView(fieldOfView),m_aspectRatio(aspectRatio),
		m_nearPlane(nearPlane),m_farPlane(farPlane),m_frameWidth(frameWidth),m_frameHeight(frameHeight),m_viewportMatrix(), m_cameraMatrix(), m_perspectiveMatrix(),m_viewTransformMatrix()
	{}


	//Intializes the camera
	bool Camera::Initialize()
	{
		ComputeViewvolumeData();
		ComputeViewTransformMatrix();
		return true;
	}

	//functions to compute the matrices needed for the view transform.
	Matrix4x4* Camera::ComputeViewTransformMatrix()
	{

		ComputeViewPortMatrix();
		ComputePerspectiveMatrix();
		ComputeCameraMatrix();
	
		MultiMat4x4(m_viewTransformMatrix,m_perspectiveMatrix, m_cameraMatrix);
		return &m_viewTransformMatrix;
	}


	Matrix4x4* Camera::GetViewportMatrix()
	{
		return &m_viewportMatrix;
	}

	void Camera::SetCameraPosition(Vector3D& position)
	{
		m_position = position;
	}

	void Camera::ComputeViewvolumeData()
	{
		m_top = abs(m_nearPlane) * tanf((m_fieldOfView / 2));
		m_bottom = -1.0f * m_top;
		m_right = m_aspectRatio * m_top;
		m_left =  -1.0f * m_right;
	}

	void Camera::ComputeViewPortMatrix()
	{
		float nx = m_frameWidth;
		float ny = m_frameHeight;

		m_viewportMatrix.SetColumn(1, (nx/2), 0, 0, 0);
		m_viewportMatrix.SetColumn(2, 0, (ny/2), 0, 0);
		m_viewportMatrix.SetColumn(3, 0, 0 , 1, 0);
		m_viewportMatrix.SetColumn(4, (nx - 1) / 2, (ny - 1) / 2, 0, 1);
	}

	void Camera::ComputeCameraMatrix()
	{
		Matrix4x4 camTransformMatrix;
		Matrix4x4 camAxisMatrix;
		if(m_position.m_z == 0.0)
			m_position.m_z += EPSILON;
		//Compute canonical axis
		LookAtToAxes(m_position,m_viewTarget,m_upVector,m_uAxis,m_vAxis,m_wAxis);

		//The camera transform matrix
		TranslationMatrix(camTransformMatrix,(-1 * m_position.m_x),(-1 * m_position.m_y),(-1 * m_position.m_z));

		camAxisMatrix.SetColumn(1, m_uAxis.m_x, m_vAxis.m_x, m_wAxis.m_x,0);
		camAxisMatrix.SetColumn(2, m_uAxis.m_y, m_vAxis.m_y, m_wAxis.m_y,0);
		camAxisMatrix.SetColumn(3, m_uAxis.m_z, m_vAxis.m_z, m_wAxis.m_z,0);
		camAxisMatrix.SetColumn(4, 0, 0, 0,1);

		//Calculate the Camera Matrix
		MultiMat4x4(m_cameraMatrix, camAxisMatrix, camTransformMatrix); 
	}

	void Camera::ComputePerspectiveMatrix()
	{
		m_perspectiveMatrix.SetColumn(1, ((2 * m_nearPlane) / (m_right - m_left)), 0, 0, 0);
		m_perspectiveMatrix.SetColumn(2, 0,((2 * m_nearPlane) / (m_top - m_bottom)) , 0, 0);
		m_perspectiveMatrix.SetColumn(3, ((m_left + m_right)/(m_left - m_right)), ((m_bottom + m_top) / (m_bottom - m_top)),((m_nearPlane + m_farPlane) / (m_farPlane - m_nearPlane)),1);
		m_perspectiveMatrix.SetColumn(4, 0, 0, 1 * (2 * m_farPlane * m_nearPlane) / (m_farPlane - m_nearPlane), 0);
	}

}