//This is a project class foundation given

#pragma once

#include <CoreStructures\GUObject.h>
#include <CoreStructures\GUMatrix4.h>
#include <glew\glew.h>

class CGPrincipleAxes : public CoreStructures::GUObject {

private:
	//VBO stores the vertex, colour, and shader data
	GLuint					paVertexArrayObj;

	GLuint					paVertexBuffer;
	GLuint					paColourBuffer;

	GLuint					paIndexBuffer;

	GLuint					paShader;

public:

	CGPrincipleAxes();

	~CGPrincipleAxes();

	void render(const CoreStructures::GUMatrix4& T);
};
