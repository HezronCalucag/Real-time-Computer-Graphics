#pragma once
#include <glew/glew.h>
#include <CoreStructures/GUObject.h>
#include <CoreStructures/GUMatrix4.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "src/shader_setup.h"

class meshVBO : public CoreStructures::GUObject
{
	//Stores Vertex Buffer Objects within the class
private:

	GLuint meshVAO; //Vertex Array Object stored within Vertex Buffer Object
	GLuint meshPosBuffer;
	GLuint meshTexCoordBuffer;
	GLuint meshNormalBuffer;
	GLuint MeshFaceVBO;

	GLuint numfaces;

public:

	meshVBO(const aiScene* scene, int meshIndex);

	void render();
};