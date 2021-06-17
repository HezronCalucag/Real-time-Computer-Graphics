#version 330

//Renders matrix of order 4x4
uniform mat4 mvpMatrix;

//Defines the variables of lights, colours, light direction and light display
uniform vec3 L[3];
uniform vec3 Lk[3];
uniform vec3 Lcolour[3];
uniform vec3 Ldirect[2];
uniform float Ldisp[2];

uniform int Ltype[2];

//Holds a simple data about meshes that are rendered in the scene
layout (location=0) in vec3 vertexPos;
layout (location=1) in vec3 vertexTexCoord;
layout (location=6) in vec3 mNormal;

//Calculates how the light positions in the game scene
out vec3 texCoord;
out vec3 brightness;


vec3 calcLightContributionForPointLight(vec3 vertexPos, vec3 mNormal, vec3 L, vec3 Lcolour, vec3 Lk)
{
	//distance vector
	vec3 D = L - vertexPos;

	//GLSL's length function
	float lengthD = length(D);

	//normalise distance vector
	vec3 D_ = normalize(D);

	// calculate for attentuation
	float A = 1.0 / (Lk.x + Lk.y * lengthD + Lk.z * lengthD * lengthD);

	//Lambertian
	float lambertian = clamp(dot(D_, mNormal), 0.0, 1.0);

	return lambertian * A * Lcolour;
}


vec3 calcLightContributionForDirectionalLight(vec3 vertexPos, vec3 mNormal, vec3 L, vec3 Lcolour, vec3 Ldirect )
{

	//distance vector
	//Ldirect = L - vertexPos;

	//GLSL's length function
	//float lengthD = length(D);

	//normalise distance vector
	vec3 D_ = normalize(Ldirect);

	// calculate for attentuation
	//float A = 1.0 / (Lk.x + Lk.y * lengthD + Lk.z * lengthD * lengthD);

	//Lambertian
	float lambertian = clamp(dot(D_, mNormal), 0.0, 1.0);

	//float acos = dot(-D_, Ldirect);

	return lambertian * Lcolour;
}


vec3 calcLightContributionForSpotLight(vec3 vertexPos, vec3 mNormal, vec3 L, vec3 Lcolour, vec3 Lk, vec3 Ldirect, float Ldisp ){

	//distance vector
	vec3 D = L - vertexPos;

	//GLSL's length function
	float lengthD = length(D);

	//normalise distance vector
	vec3 D_ = normalize(D);

	//calculate for attentuation
	float A = 1.0 / (Lk.x + Lk.y * lengthD + Lk.z * lengthD * lengthD);

	//Lambertian
	float lambertian = clamp(dot(D_, mNormal), 0.0, 1.0);

	float acos = acos(dot(-D_, normalize(Ldirect)));
	if (acos > Ldisp) {
		
		A = 0.0;
	}

	return acos * lambertian * A * Lcolour;
}


void main(void) {

 //The variables of lights, colour and light display
	vec3 L[3];
	L[0] = vec3(0.0, 100.0, 0.0); // Light position 0  
	L[1] = vec3(0.0, 0.0, 100.0); // Light position 1
	L[2] = vec3(100.0, 0.0, 0.0); // Light position 2


	vec3 Lk[3];
	Lk[0] = vec3(1.0f, 0.0, 0.0); // attenuation attributes for light 0	
	Lk[1] = vec3(1.0f, 0.0, 0.0); // attenuation attributes for light 1
	Lk[2] = vec3(1.0f, 0.0, 0.0); // attenuation attributes for light 2

	vec3 Lcolour[3];
	Lcolour[0] = vec3(1.0f, 1.0f, 1.0f);	//red, green, blue
	Lcolour[1] = vec3(1.0f, 1.0f, 1.0f);	//white
	Lcolour[2] = vec3(1.0f, 1.0f, 1.0f);	//white


	//Calculate brightness
	brightness = vec3(0.0f, 0.0f, 0.0f);		

	for (int i=0; i < 2; ++i) {

		if (Ltype[i]==0) {
			brightness += calcLightContributionForPointLight(vertexPos, mNormal, L[i], Lcolour[i], Lk[i]);
			//brightness += vec3(0.0, 0.0, 1.0);//calcLightContributionForPointLight(vertexPos, mNormal, L[i], Lcolour[i], Lk[i]);
		}
		else if (Ltype[i]==1) {
			brightness += calcLightContributionForDirectionalLight(vertexPos, mNormal, L[i], Lcolour[i], Ldirect[i]);
			//brightness += vec3(0.0, 1.0, 0.0);//calcLightContributionForDirectionalLight(vertexPos, mNormal, L[i], Lcolour[i], Ldirect[i]);
		}
		else if (Ltype[i]==2) {
			brightness += calcLightContributionForSpotLight(vertexPos, mNormal, L[i], Lcolour[i], Lk[i], Ldirect[i], Ldisp[i]);
		}
	}
		
		gl_Position = mvpMatrix * vec4(vertexPos, 1.0);
		
		texCoord = vertexTexCoord;
	
}
