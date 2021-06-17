#version 330

uniform sampler2D texture;

in vec3 texCoord;
in vec3 brightness;

layout (location=0) out vec4 fragColour;

void main(void) {


	//fragColour = texColor;
    //fragColour = vec4(gl_FragCoord.z, gl_FragCoord.z, gl_FragCoord.z, 1.0);
    fragColour = texture2D(texture, texCoord.xy) * vec4(brightness, 1.0);

}