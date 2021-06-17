#version 330

uniform sampler2D texture;

//Renders 3D vector that includes X, Y, Z positions and Red, Green, Blue
in vec3 texCoord;
in vec3 brightness;

//Vertex attributes
layout (location=0) out vec4 fragColour;

void main(void) {

	//vec4 texColor = texture2D(texture, texCoord);
	//fragColour = texColor;

    //fragColour = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0);
    fragColour = texture2D(texture, texCoord.xy) * vec4(brightness, 1.0); //Coordinates the texture into models

}
