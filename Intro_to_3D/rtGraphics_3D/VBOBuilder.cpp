#include "VBOBuilder.h"

VBOBuilder::VBOBuilder(const aiScene* scene) 
{
	numModels = scene->mNumMeshes; //The certain number of models that can be rendered in the scene

	modelArray = (meshVBO**)malloc(numModels * sizeof(meshVBO*)); //Allocates the memory into VBO

	for (int i = 0; i < numModels; i++) 
	{
		modelArray[i] = new meshVBO(scene, i);
	}

}

void VBOBuilder::render() 
{
	for (int i = 0; i < numModels; i++)
	{
		modelArray[i]->render(); //Renders the model array
	}
}