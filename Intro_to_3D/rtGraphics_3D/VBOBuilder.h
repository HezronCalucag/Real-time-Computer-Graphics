#pragma once

#include <glew/glew.h>
#include <CoreStructures/GUObject.h>
#include <CoreStructures/GUMatrix4.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "meshVBO.h"
#include "src/shader_setup.h"

class VBOBuilder : public CoreStructures::GUObject
{
	//Stores the model array by using Vertex Buffer Array
	GLuint numModels;
	meshVBO** modelArray;

public:

	VBOBuilder(const aiScene* scene);

	void render();
};