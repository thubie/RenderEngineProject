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
		Vertex* vertices = nullptr;
		Face* indices = nullptr;

		Vector4D vTemp;
		Vector3D nTemp;
		TexCoord tTemp;

		int faceCount = 0;
		int vertexCount = 0;
		int prevVertexCount = 0;
		int vIndex = 0;
		int tIndex = 0;
		int nIndex = 0;
		
		//Fill the models with the mesh data.
		for(i = 0; i < modelCount; ++i)
		{
			//Create the models and allocate space for the mesh data.
			sceneObjects.push_back(Model());
			vertexCount = objectDataList[i].vertexCount;
			sceneObjects[i].vertices =	new Vertex[vertexCount];
			if(sceneObjects[i].vertices == nullptr)
				return false;
			
			faceCount = objectDataList[i].faceCount;
			sceneObjects[i].faces = new Face[faceCount];
			if(sceneObjects[i].faces == nullptr)
				return false;

			sceneObjects[i].faceCount = faceCount;

			for(j = 0; j < faceCount; ++j)
			{
				//Fill the vertex data and indices for vertex one
				vIndex = objectDataList[i].faceList[i].indices[j][0] - 1;
				tIndex = objectDataList[i].faceList[i].indices[j][1] - 1;
				nIndex = objectDataList[i].faceList[i].indices[j][2] - 1;
				
				vTemp = vertexList[vIndex];
				nTemp = normalList[nIndex];
				tTemp = texCoordList[tIndex];
				vIndex = vIndex - prevVertexCount;

				sceneObjects[i].vertices[vIndex].position.m_x = vTemp.m_x;
				sceneObjects[i].vertices[vIndex].position.m_y = vTemp.m_y;
				sceneObjects[i].vertices[vIndex].position.m_z = vTemp.m_z;

				sceneObjects[i].vertices[vIndex].uv.u = tTemp.u;
				sceneObjects[i].vertices[vIndex].uv.v = tTemp.v;

				sceneObjects[i].vertices[vIndex].normal.m_x = nTemp.m_x;
				sceneObjects[i].vertices[vIndex].normal.m_y = nTemp.m_y;
				sceneObjects[i].vertices[vIndex].normal.m_z = nTemp.m_z;

				sceneObjects[i].faces[j].indices[0] = vIndex;

				//Fill the vertex data and indices for vertex two
				vIndex = objectDataList[i].faceList[i].indices[j][1] - 1;
				tIndex = objectDataList[i].faceList[i].indices[j][1] - 1;
				nIndex = objectDataList[i].faceList[i].indices[j][1] - 1;

				vTemp = vertexList[vIndex];
				nTemp = normalList[nIndex];
				tTemp = texCoordList[tIndex];
				vIndex = vIndex - prevVertexCount;

				sceneObjects[i].vertices[vIndex].position.m_x = vTemp.m_x;
				sceneObjects[i].vertices[vIndex].position.m_y = vTemp.m_y;
				sceneObjects[i].vertices[vIndex].position.m_z = vTemp.m_z;

				sceneObjects[i].vertices[vIndex].uv.u = tTemp.u;
				sceneObjects[i].vertices[vIndex].uv.v = tTemp.v;

				sceneObjects[i].vertices[vIndex].normal.m_x = nTemp.m_x;
				sceneObjects[i].vertices[vIndex].normal.m_y = nTemp.m_y;
				sceneObjects[i].vertices[vIndex].normal.m_z = nTemp.m_z;

				sceneObjects[i].faces[j].indices[1] = vIndex;

				//Fill the vertex data and indices for vertex three
				vIndex = objectDataList[i].faceList[i].indices[j][2] - 1;
				tIndex = objectDataList[i].faceList[i].indices[j][2] - 1;
				nIndex = objectDataList[i].faceList[i].indices[j][2] - 1;

				vTemp = vertexList[vIndex];
				nTemp = normalList[nIndex];
				tTemp = texCoordList[tIndex];
				vIndex = vIndex - prevVertexCount;

				sceneObjects[i].vertices[vIndex].position.m_x = vTemp.m_x;
				sceneObjects[i].vertices[vIndex].position.m_y = vTemp.m_y;
				sceneObjects[i].vertices[vIndex].position.m_z = vTemp.m_z;

				sceneObjects[i].vertices[vIndex].uv.u = tTemp.u;
				sceneObjects[i].vertices[vIndex].uv.v = tTemp.v;

				sceneObjects[i].vertices[vIndex].normal.m_x = nTemp.m_x;
				sceneObjects[i].vertices[vIndex].normal.m_y = nTemp.m_y;
				sceneObjects[i].vertices[vIndex].normal.m_z = nTemp.m_z;

				sceneObjects[i].faces[j].indices[2] = vIndex;
			}
			prevVertexCount = vertexCount;
		}

		return true;
	}





}