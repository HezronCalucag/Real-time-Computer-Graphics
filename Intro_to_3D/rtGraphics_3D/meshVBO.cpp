#include "meshVBO.h"

meshVBO::meshVBO(const aiScene* scene, int meshIndex)
{
	//Setup VAO For principle axes object
	glGenVertexArrays(1, &meshVAO);
	glBindVertexArray(meshVAO);

	//Setup VBO for position variables
	glGenBuffers(1, &meshPosBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, meshPosBuffer);
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[meshIndex]->mNumVertices * sizeof(aiVector3D), scene->mMeshes[meshIndex]->mVertices, GL_STATIC_DRAW);

	//Vertex Shader setup
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);

	//Setup VBO for normals
	glGenBuffers(1, &meshNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, meshNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[meshIndex]->mNumVertices * sizeof(aiVector3D), scene->mMeshes[meshIndex]->mNormals, GL_STATIC_DRAW);

	//Setup vertex shader variable bindings via connecting colour to the buffer
	glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)0);

	//Setup VBO for colour variables
	glGenBuffers(1, &meshTexCoordBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, meshTexCoordBuffer);
	glBufferData(GL_ARRAY_BUFFER, scene->mMeshes[meshIndex]->mNumVertices * sizeof(aiVector3D), scene->mMeshes[meshIndex]->mTextureCoords[0], GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 0, (const GLvoid*)0);

	//Setup VBO for principle axis(PA) iNDEX Buffer
	glGenBuffers(1, &MeshFaceVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MeshFaceVBO); //Must use glDrawElements to render 3D objects for face index array

	unsigned int numBytes = scene->mMeshes[meshIndex]->mNumFaces * 3 * sizeof(unsigned int);

	unsigned int* FaceIndexArray = (unsigned int*)malloc(numBytes);

	for (int f = 0, dstIndex = 0; f < scene->mMeshes[meshIndex]->mNumFaces; f++)
	{

		unsigned int* I = scene->mMeshes[meshIndex]->mFaces[f].mIndices;

		FaceIndexArray[dstIndex] = I[0];
		dstIndex += 1;
		FaceIndexArray[dstIndex] = I[1];
		dstIndex += 1;
		FaceIndexArray[dstIndex] = I[2];
		dstIndex += 1;
	}

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numBytes, FaceIndexArray, GL_STATIC_DRAW);

	//Enable Vertex Buffer

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(6);

	//Unbind Principle axes VAO
	glBindVertexArray(0);

	//Unbind Principle VAO

	numfaces = scene->mMeshes[meshIndex]->mNumFaces;
}


void meshVBO::render()
{
	glBindVertexArray(meshVAO);
	glDrawElements(GL_TRIANGLES, numfaces * 3, GL_UNSIGNED_INT, (void*)0);
}