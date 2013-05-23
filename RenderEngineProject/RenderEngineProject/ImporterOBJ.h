#pragma once

#include<vector>
#include"LinearMath.h"
#include"Vertex.h"
#include"Model.h"
#include<fstream>
#include<sstream>
#include<string>

namespace RenderEngine
{
	struct FaceData
	{
		int indices[3][3];
	};

	struct ObjectData
	{		
		std::vector<FaceData> faceList;
		int vertexCount;
		int normalCount;
		int texCoordCount;
		int faceCount;
	};

	class ImporterOBJ
	{
	public:
		ImporterOBJ();
		ImporterOBJ(char* filename);
		void setFilename(char* filename);
		bool ProcessFile();

		std::vector<Model>* GetScene();
		
	private:
		void GetDataCount();
		bool ReadInfileAndElementCounts();
		bool FillMeshData();
		bool MakeScene();
	private:
		char* filename;
		std::vector<ObjectData> objectDataList;
		std::vector<int> objectLineStart;  //container containing the line number a new object starts at.
		std::vector<std::string> fileLines; 
		//Object data off the mesh
		std::vector<Vector4D> vertexList;
		std::vector<Vector3D> normalList;
		std::vector<TexCoord> texCoordList;
		std::vector<Model> sceneObjects;
		Vector4D* meshData;
		friend class Model;
	};
}