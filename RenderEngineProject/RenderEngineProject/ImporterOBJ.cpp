#include"ImporterOBJ.h"



namespace RenderEngine
{
	ImporterOBJ::ImporterOBJ() : objectDataList(std::vector<ObjectData>()),objectLineStart(std::vector<int>()), fileLines(std::vector<std::string>())  
	{
		filename = nullptr;
	}

	ImporterOBJ::ImporterOBJ(char *inputFile) : filename(inputFile), objectDataList(std::vector<ObjectData>()), objectLineStart(std::vector<int>()), fileLines(std::vector<std::string>()) 
	{}

	void ImporterOBJ::setFilename(char *inputFile)
	{
		this->filename = inputFile;
	}

	bool ImporterOBJ::ProcessFile()
	{
		bool result;
		result = ReadInfileAndElementCounts();
		if(!result)
			return false;

		result = FillMeshData();
		if(!result)
			return false;

		result = MakeScene();
		if(!result)
			return false;

		return true;
	}

	bool ImporterOBJ::ReadInfileAndElementCounts()
	{
		std::ifstream fin;
		int objectCount = 0;
		bool result;
		char input1;
		char input2;
		int currentPos = 0;
		int startline = 0;
		int endline = 0;  
		int i,j,k,objectListSize,lineSize;
		std::string line;

		//Open file
		fin.open(filename,std::ios::in);
		result = fin.fail();
		//Check if we openend file right.
		if(fin.fail() == true)
			return false;

		std::getline(fin,line);
		fileLines.push_back(line);
		startline += 1;
		//see how many objects there are and push the string info fileLines
		while(!fin.eof())
		{
			if(line[0] == 'o')
			{
				objectDataList.push_back(ObjectData());
				objectLineStart.push_back(startline);
			}
			std::getline(fin,line);
			fileLines.push_back(line);
			startline += 1;	
		}
		fin.close();
		objectLineStart.push_back(startline);
		objectListSize = objectDataList.size();		
		return true;
	}

	bool ImporterOBJ::FillMeshData()
	{
		int i,j,k,l,objectListSize;
		int startline = 0;
		int endline = 0;
		int count = 0;
		bool result;
		
		Vector4D tempVertex;
		Vector3D tempNormal;
		TexCoord tempTex;
		FaceData tempFace;

		std::string line,input,input2;
		int lineSize;
		int vertexIndex, normalIndex, texCoordIndex, faceIndex;
		int vertexCount, normalCount, texCoordCount, faceCount;
		vertexIndex = normalIndex = texCoordIndex = faceIndex = 0;

		objectListSize = objectDataList.size();
		
		//Fill the arrays with mesh data
		for(i = 0; i < objectListSize; ++i)
		{
			startline = objectLineStart[i];
			endline = objectLineStart[i + 1] - 1;
			vertexCount = 0;
			normalCount = 0;
			texCoordCount = 0; 

			for(j = startline; j < endline; ++j)
			{
				line = fileLines[j];	
				lineSize = line.size();
				std::istringstream iss(line.c_str());
				
				
				while(iss >> input)
				{
							
					if(input == "v")
					{
						iss >> tempVertex.m_x;
						iss >> tempVertex.m_y;
						iss >> tempVertex.m_z;
						vertexList.push_back(tempVertex);
						vertexCount++;
						vertexIndex++;
					}

					if(input == "vt")
					{
						iss >> tempTex.u;
						iss >> tempTex.v;
						texCoordList.push_back(tempTex);
						texCoordCount++;
						texCoordIndex++;
					}

					if(input == "vn")
					{
						iss >> tempNormal.m_x;
						iss >> tempNormal.m_y;
						iss >> tempNormal.m_z;
						normalList.push_back(tempNormal);
						normalCount++;
						normalIndex++;
					}
					
					if(input == "f")
					{
						tempFace;						
						for(k = 0; k < 3; ++k)
						{
							iss >> input2;
							int length = input2.size();
							for(l = 0; l < length ; ++l)
							{
								char test = input2[l];
								if(test == '/')
									input2[l] = ' ';
							}
							std::istringstream iss2(input2);
							iss2 >> tempFace.indices[k][0];
							iss2 >> tempFace.indices[k][1];
							iss2 >> tempFace.indices[k][2]; 
						}
						objectDataList[i].faceList.push_back(tempFace);
						faceIndex++;
					}					
				}
			}
			objectDataList[i].vertexCount = vertexCount;
			objectDataList[i].normalCount = normalCount++;
			objectDataList[i].texCoordCount = texCoordCount;
			objectDataList[i].faceCount = objectDataList[i].faceList.size();

		}//end loop i



		return true;
	}

	bool ImporterOBJ::MakeScene()
	{
		int modelCount = objectDataList.size();
		int i,j,k;		
		Vector4D* vPos = nullptr;
		Vector3D* nVec = nullptr;
		TexCoord* texCoord = nullptr;

		Vertex* vertices = nullptr;
		Face* indices = nullptr;

		Vertex verTemp;
		Vector4D vTemp;
		Vector3D nTemp;
		TexCoord tTemp;
		Face fTemp;     
		ObjectData obTemp; // debug
		int faceCount = 0;
		int vertexCount = 0;
		int normalCount = 0;
		int texCoordCount = 0;
		int lastVertexIndex = 0;
		int lastNormalIndex = 0;
		int lastTexCoordIndex = 0;

		int indexPos1;
		int indexTCoord1;
		int indexNorm1;

		int indexPos2;
		int indexTCoord2;
		int indexNorm2;

		int indexPos3;
		int indexTCoord3;
		int indexNorm3;
		
		//Allocate space for objects temp mesh data
		vertexCount = vertexList.size();
		texCoordCount = texCoordList.size();
		normalCount = normalList.size();
		
		vPos = new Vector4D[vertexCount];
		if(vPos == nullptr)
			return false;
		nVec = new Vector3D[normalCount];
		if(nVec == nullptr)
			return false;
		texCoord = new TexCoord[texCoordCount];
		if(texCoord == nullptr)
			return false;
		
		//Fill the arrays with data.
		for(j = 0; j < vertexCount; ++j)
		{
			vPos[j] = vertexList[j];
		}

		for(j = 0; j < texCoordCount; ++j)
		{
			texCoord[j] = texCoordList[j];
		}
			
		for(j = 0; j < normalCount; ++j)
		{
			nVec[j] = normalList[j];
		}

		for(i = 0; i < modelCount; ++i)
		{
			obTemp = objectDataList[i];
			sceneObjects.push_back(Model());
			faceCount = obTemp.faceCount;
			vertexCount = obTemp.vertexCount;

			indices = new Face[faceCount];
			if(indices == nullptr)
				return false;
			vertices =	new Vertex[vertexCount];
			if(vertices == nullptr)
				return false;

			for(j = 0; j < faceCount; ++j)
			{
				//Fill the mesh data of the first vertex
				indexPos1 = obTemp.faceList[j].indices[0][0] - 1;
				indexTCoord1 = obTemp.faceList[j].indices[0][1] - 1;
				indexNorm1 = obTemp.faceList[j].indices[0][2] - 1;
				
				verTemp.position = vertexList[indexPos1];
				verTemp.normal = normalList[indexNorm1];
				verTemp.uv = texCoordList[indexTCoord1];
				vertices[indexPos1 - lastVertexIndex] = verTemp;

				//Fill the mesh data of the second vertex
				indexPos2 = obTemp.faceList[j].indices[1][0] - 1;
				indexTCoord2 = obTemp.faceList[j].indices[1][1] - 1;
				indexNorm2 = obTemp.faceList[j].indices[1][2] - 1;
				
				
				verTemp.position = vertexList[indexPos2];
				verTemp.normal = normalList[indexNorm2];
				verTemp.uv = texCoordList[indexTCoord2];
				vertices[indexPos2 - lastVertexIndex] = verTemp;

				//Fill the mesh data of the third vertex
				indexPos3 = obTemp.faceList[j].indices[2][0]  - 1;
				indexTCoord3 = obTemp.faceList[j].indices[2][1] - 1;
				indexNorm3 = obTemp.faceList[j].indices[2][2] - 1;
				

				verTemp.position = vertexList[indexPos3];
				verTemp.normal = normalList[indexNorm3];
				verTemp.uv = texCoordList[indexTCoord3];
				vertices[indexPos3 - lastVertexIndex] = verTemp;
			
				indices[j].indices[0] = indexPos1;
				indices[j].indices[1] = indexPos2;
				indices[j].indices[2] = indexPos3;
			}
			
			sceneObjects[i].faceCount = faceCount;
			sceneObjects[i].vertexCount = vertexCount;
			sceneObjects[i].faces = indices;
			sceneObjects[i].vertices = vertices;
			
			
			lastVertexIndex += vertexCount;
			lastNormalIndex += normalCount;
			lastTexCoordIndex += texCoordCount;
		}
		
		//release mesh data
		if(vPos != nullptr)
				delete[] vPos;

		if(nVec != nullptr)
			delete[] nVec;

		if(texCoord != nullptr)
			delete[] texCoord;
		
		objectDataList.clear();
		objectLineStart.clear();  
		fileLines.clear(); 
		
		vertexList.clear();
		normalList.clear();
		texCoordList.clear();
		return true;
	}

	std::vector<Model>* ImporterOBJ::GetScene()
	{
		return &sceneObjects;
	}


}