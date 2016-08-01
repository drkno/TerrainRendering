#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "CubePatches.h"
#include "loadTGA.h"
using namespace std;
using namespace glm;

GLuint vaoID;
GLuint theProgram;
GLuint matrixLoc;
float horizontalAngle = 0.0, verticalAngle = 0.0;
mat4 proj;
vec3 cameraPos;
vec3 lookPos;
GLuint texID;

vec3 upVec;

#ifdef _WIN32
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif

void loadTextures()
{
	glGenTextures(1, &texID);   //Generate 1 texture ID
								// Load brick texture
	glActiveTexture(GL_TEXTURE0);  //Texture unit 0
	glBindTexture(GL_TEXTURE_2D, texID);
	loadTGA("heightmap\\HeightMap.tga");

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

GLuint loadShader(GLenum shaderType, string filename)
{
	ifstream shaderFile(filename.c_str());
	if(!shaderFile.good()) cout << "Error opening shader file." << endl;
	stringstream shaderData;
	shaderData << shaderFile.rdbuf();
	shaderFile.close();
	string shaderStr = shaderData.str();
	const char* shaderTxt = shaderStr.c_str();

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &shaderTxt, NULL);
	glCompileShader(shader);
	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
		const char *strShaderType = NULL;
		cerr <<  "Compile failure in shader: " << strInfoLog << endl;
		delete[] strInfoLog;
	}
	return shader;
}


void initialise()
{
	cameraPos = vec3(50.0, 15.0, 12.0);
	lookPos = vec3(50.0, 10.0, 0.0);
	upVec = vec3(0.0, 1.0, 0.0);

	

	GLuint shaderv = loadShader(GL_VERTEX_SHADER, "CubePatches.vert");
	GLuint shaderf = loadShader(GL_FRAGMENT_SHADER, "CubePatches.frag");
	GLuint shaderc = loadShader(GL_TESS_CONTROL_SHADER, "CubePatchesControl.glsl");
	GLuint shadere = loadShader(GL_TESS_EVALUATION_SHADER, "CubePatchesEvaluation.glsl");

	GLuint program = glCreateProgram();
	glAttachShader(program, shaderv);
	glAttachShader(program, shaderf);
	glAttachShader(program, shaderc);
	glAttachShader(program, shadere);
	glLinkProgram(program);


	loadTextures();
	GLuint texLoc = glGetUniformLocation(program, "heightMapSampler");
	glUniform1i(texLoc, 0);

	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}
	glUseProgram(program);

	proj = perspective(20.0f, 1.0f, 10.0f, 1000.0f);  //perspective projection matrix

	

	/*GLfloat outLevel[4] = { 1,1,1,1 };
	GLfloat inLevel[2] = { 1,1 };*/
	glPatchParameteri(GL_PATCH_VERTICES, 4);
	/*glPatchParameterfv(GL_PATCH_DEFAULT_OUTER_LEVEL, outLevel);
	glPatchParameterfv(GL_PATCH_DEFAULT_INNER_LEVEL, inLevel);*/


	GLuint vboID[4];

	glGenVertexArrays(1, &vaoID);
    glBindVertexArray(vaoID);

    glGenVertexArrays(2, vboID);

    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);  // Vertex position

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboID[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elems), elems, GL_STATIC_DRAW);

    glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glutPostRedisplay();
}

void display()
{
	vec3 look = rotate(lookPos - cameraPos, radians(horizontalAngle), vec3(0.0, 1.0, 0.0));
	look = rotate(look, radians(verticalAngle), vec3(1.0, 0.0, 0.0));
	mat4 view = lookAt(cameraPos, lookPos, upVec); //view matrix
	mat4 projView = proj*view;  //Product matrix

	mat4 matrix = mat4(1.0);
	mat4 prodMatrix = projView * matrix;        //Model-view-proj matrix

	glUniformMatrix4fv(matrixLoc, 1, GL_FALSE, &prodMatrix[0][0]);

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(vaoID);
	glDrawElements(GL_PATCHES, GRID_WIDTH * GRID_HEIGHT * 4, GL_UNSIGNED_SHORT, NULL);
	glFlush();
}

void handleSpecialKeypress(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		verticalAngle += 2;
		break;
	case GLUT_KEY_DOWN:
		verticalAngle -= 2;
		break;
	case GLUT_KEY_LEFT:
		horizontalAngle -= 2;
		break;
	case GLUT_KEY_RIGHT:
		horizontalAngle += 2;
		break;
	default:
		return;
	}

	glutPostRedisplay();
}

void handleKeypress(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		cameraPos += vec3(0.0, 0.0, -1.0);
		lookPos += vec3(0.0, 0.0, -1.0);
		break;
	case 'a':
		cameraPos += vec3(-1.0, 0.0, 0.0);
		lookPos += vec3(-1.0, 0.0, 0.0);
		break;
	case 's':
		cameraPos += vec3(0.0, 0.0, 1.0);
		lookPos += vec3(0.0, 0.0, 1.0);
		break;
	case 'd':
		cameraPos += vec3(1.0, 0.0, 0.0);
		lookPos += vec3(1.0, 0.0, 0.0);
		break;

	default: return;
	}
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	populateGrid();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Cube with Bezier patches");
	glutInitContextVersion (4, 2);
	glutInitContextProfile ( GLUT_CORE_PROFILE );

	if(glewInit() == GLEW_OK)
	{
		cout << "GLEW initialization successful! " << endl;
		cout << " Using GLEW version " << glewGetString(GLEW_VERSION) << endl;
	}
	else
	{
		cerr << "Unable to initialize GLEW  ...exiting." << endl;
		exit(EXIT_FAILURE);
	}

	initialise();
	glutDisplayFunc(display);
	glutSpecialFunc(handleSpecialKeypress);
	glutKeyboardFunc(handleKeypress);
	glutMainLoop();
}

