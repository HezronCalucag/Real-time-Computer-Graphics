// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include <glew/glew.h>
#include <glew/wglew.h>
// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include <glew/glew.h>
#include <glew/wglew.h>
#include <GL\freeglut.h>
#include <CoreStructures\CoreStructures.h>
#include <iostream>
#include <string>
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include "src/CGTexturedQuad.h"
#include "src/aiWrapper.h"
#include "src/CGPrincipleAxes.h"
#include "src/texture_loader.h"
#include "VBOBuilder.h"
#include "src/shader_setup.h"

using namespace std;
using namespace CoreStructures;

#pragma region Scene variables and resources

// Variables needed to track where the mouse pointer is so we can determine which direction it's moving in
int								mouse_x, mouse_y;
bool							mDown = false;

//Player variables in terms of X, Y, Z positons and player movement
float                           playerX = 0, playerY = -10, playerZ = -200;
bool                            wKeydown, aKeyDown, sKeyDown, dKeyDown;

GUClock* mainClock = nullptr;

//
// Main scene resources
//
GUPivotCamera* mainCamera = nullptr;
CGPrincipleAxes* principleAxes = nullptr;
CGTexturedQuad* texturedQuad = nullptr;

//Define objects and its texture for render
const aiScene* aiRobot;
GLuint		   robotTexture;
VBOBuilder*    robot;

const aiScene* aiLevelMap;
GLuint         levelmapTexture;
VBOBuilder*    levelmap;

//Define the use of mesh shader
GLuint		   meshShader;

//Define camera postion
GUVector4      camFocalPoint = GUVector4(0.0f, 0.0f, 0.0f, 1.0f);

#pragma endregion


#pragma region Function Prototypes

void init(int argc, char* argv[]); // Main scene initialisation function
void update(void); // Main scene update function
void display(void); // Main scene render function

// Event handling functions
void mouseButtonDown(int button_id, int state, int x, int y);
void mouseMove(int x, int y);
void mouseWheel(int wheel, int direction, int x, int y);
void keyDown(unsigned char key, int x, int y);
void closeWindow(void);
void reportContextVersion(void);
void reportExtensions(void);

#pragma endregion


int main(int argc, char* argv[])
{
	init(argc, argv);
	glutMainLoop();

	// Stop clock and report final timing data
	if (mainClock) {

		mainClock->stop();
		mainClock->reportTimingData();
		mainClock->release();
	}

	return 0;
}


void init(int argc, char* argv[]) {

	// Initialise FreeGLUT
	glutInit(&argc, argv);

	glutInitContextVersion(4, 3);
	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE/* | GLUT_MULTISAMPLE*/);
	glutSetOption(GLUT_MULTISAMPLE, 4);

	// Setup window
	int windowWidth = 800;
	int windowHeight = 600;
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(64, 64);
	glutCreateWindow("3D Example 01");
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

	// Register callback functions
	glutIdleFunc(update); // Main scene update function
	glutDisplayFunc(display); // Main render function
	glutKeyboardFunc(keyDown); // Key down handler
	glutMouseFunc(mouseButtonDown); // Mouse button handler
	glutMotionFunc(mouseMove); // Mouse move handler
	glutMouseWheelFunc(mouseWheel); // Mouse wheel event handler
	glutCloseFunc(closeWindow); // Main resource cleanup handler


	// Initialise glew
	glewInit();

	// Initialise OpenGL...

	wglSwapIntervalEXT(0);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glFrontFace(GL_CCW); // Default anyway

	// Setup colour to clear the window
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// Setup main camera
	float viewportAspect = (float)windowWidth / (float)windowHeight;
	mainCamera = new GUPivotCamera(0.0f, 0.0f, 15.0f, 55.0f, viewportAspect, 0.1f);

	//Imports the models and the texture from files/texture library
	aiRobot = aiImportModel(string("robot.fbx"),
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	robotTexture = fiLoadTexture("..\\..\\Common2\\Resources\\Textures\\RobotUVWTexture.jpg", TextureProperties(false));
	robot = new VBOBuilder(aiRobot);

	aiLevelMap = aiImportModel(string("levelmap.fbx"),
		aiProcess_GenSmoothNormals |
		aiProcess_CalcTangentSpace |
		aiProcess_Triangulate |
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	levelmapTexture = fiLoadTexture("..\\..\\Common2\\Resources\\Textures\\levelmapUVWTexture.jpg", TextureProperties(false));
	levelmap = new VBOBuilder(aiLevelMap);

	//Loads shaders from source files and setting it up
	meshShader = setupShaders(string("shaders\\Mesh_shader.vs"), string("shaders\\Mesh_shader.fs"));

	// Setup and start the main clock
	mainClock = new GUClock();
}

// Main scene update function (called by FreeGLUT's main event loop every frame) 
void update(void) {
	
	// Update clock
	mainClock->tick();

	// Redraw the screen
	display();

	// Update the window title to show current frames-per-second and seconds-per-frame data
	char timingString[256];
	sprintf_s(timingString, 256, "CIS5013. Average fps: %.0f; Average spf: %f", mainClock->averageFPS(), mainClock->averageSPF() / 1000.0f);
	glutSetWindowTitle(timingString);


	//Updates the player movement variables
	if (wKeydown == true)
	{
		playerZ = playerZ + 0.5;
		wKeydown = false;
		camFocalPoint.z = playerZ + 1.0f; //Camera follows the player's movements
	}
	if (sKeyDown == true)
	{
		playerZ = playerZ - 0.5f;
		sKeyDown = false;
	}
	if (aKeyDown == true)
	{
		playerX = playerX + 0.5f;
		aKeyDown = false;
		cout << playerX << endl;
	}
	if (dKeyDown == true)
	{
		playerX = playerX - 0.5f;
		dKeyDown = false;
		cout << playerX << endl;
	}
}


void display(void) {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set viewport to the client area of the current window
	glViewport(0, 0, glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));

	// Get view-projection transform as a GUMatrix4
	GUMatrix4 T = mainCamera->projectionTransform() * mainCamera->viewTransform() * GUMatrix4::translationMatrix(-playerX, -playerY, -playerZ);

	if (principleAxes)
		principleAxes->render(T);

	//Displays the model, the texture and shaders. Also translation matrix allows the X, Y, Z positions, the scale and the rotation position 

	//Render robot
	glBindTexture(GL_TEXTURE_2D, robotTexture);
	glEnable(GL_TEXTURE_2D);
	static GLint mvplocation = glGetUniformLocation(meshShader, "mvpMatrix");
	glUseProgram(meshShader);
	GUMatrix4 modelMat = GUMatrix4::translationMatrix(playerX, playerY, playerZ) * GUMatrix4::scaleMatrix(25.0f, 25.0f, 25.0f) * GUMatrix4::rotationMatrix(4.7f, 0.0f, 0.0f);
	GUMatrix4 mvpMatrix = T * modelMat;
	glUniformMatrix4fv(mvplocation, 1, GL_FALSE, (const GLfloat*)&(mvpMatrix.M));
	robot->render();

	//Render level map
	glBindTexture(GL_TEXTURE_2D, levelmapTexture);
	glEnable(GL_TEXTURE_2D);
	static GLint mvplocation2 = glGetUniformLocation(meshShader, "mvpMatrix");
	glUseProgram(meshShader);
	GUMatrix4 modelMatX = GUMatrix4::translationMatrix(0, -20, 0) * GUMatrix4::scaleMatrix(25.0f, 25.0f, 25.0f) * GUMatrix4::rotationMatrix(4.7f, 0.0f, 0.0f);
	GUMatrix4 mvpMatrix2 = T * modelMatX;
	glUniformMatrix4fv(mvplocation2, 1, GL_FALSE, (const GLfloat*)&(mvpMatrix2.M));
	levelmap->render();

	//Enclose the functions and operate double buffering
	glutSwapBuffers();
}



#pragma region Event handling functions

void mouseButtonDown(int button_id, int state, int x, int y) {
	//Detects mouse inputs
	if (button_id == GLUT_LEFT_BUTTON) {

		if (state == GLUT_DOWN) {

			mouse_x = x;
			mouse_y = y;

			mDown = true;

		}
		else if (state == GLUT_UP) {

			mDown = false;
		}
	}
}


void mouseMove(int x, int y) {
	//Moves the camera via mouse in terms of X and Y positions
	int dx = x - mouse_x;
	int dy = y - mouse_y;

	if (mainCamera)
		mainCamera->transformCamera((float)-dy, (float)-dx, 0.0f);

	mouse_x = x;
	mouse_y = y;
}


void mouseWheel(int wheel, int direction, int x, int y) {
	//Rescales the camera radius via mouse wheel
	if (mainCamera) {

		if (direction < 0)
			mainCamera->scaleCameraRadius(1.1f);
		else if (direction > 0)
			mainCamera->scaleCameraRadius(0.9f);
	}
}


void keyDown(unsigned char key, int x, int y) {

	// Toggle fullscreen (This does not adjust the display mode however!)
	if (key == 'f')
		glutFullScreenToggle();


	//Detects the WASD input for the player movement
	if (key == 'w')
		wKeydown = true;
	if (key == 's')
		sKeyDown = true;
	if (key == 'a')
		aKeyDown = true;
	if (key == 'd')
		dKeyDown = true;
}


void closeWindow(void) {

	// Clean-up scene resources

	if (mainCamera)
		mainCamera->release();

	if (principleAxes)
		principleAxes->release();

	if (texturedQuad)
		texturedQuad->release();
}


#pragma region Helper Functions

void reportContextVersion(void) {

	int majorVersion, minorVersion;

	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);

	cout << "OpenGL version " << majorVersion << "." << minorVersion << "\n\n";
}

void reportExtensions(void) {

	cout << "Extensions supported...\n\n";

	const char* glExtensionString = (const char*)glGetString(GL_EXTENSIONS);

	char* strEnd = (char*)glExtensionString + strlen(glExtensionString);
	char* sptr = (char*)glExtensionString;

	while (sptr < strEnd) {

		int slen = (int)strcspn(sptr, " ");
		printf("%.*s\n", slen, sptr);
		sptr += slen + 1;
	}
}

#pragma endregion


#pragma endregion

