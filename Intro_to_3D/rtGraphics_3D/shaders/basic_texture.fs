#version 330

uniform sampler2D texture;

//Renders 2D vector that includes X, Y, positions and Red, Green
in vec2 texCoord;

//Vertex attributes
layout (location=0) out vec4 fragColour;

void main(void) {

	vec4 texColor = texture2D(texture, texCoord);
	fragColour = texColor;

    //fragColour = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0);
    
}
