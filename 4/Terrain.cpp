#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include "Terrain.h"
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

vec3 upVec;

#ifdef _WIN32
extern "C" {
	_declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
}
#endif

void loadTextures()
{
	GLuint texID;
	glGenTextures(1, &texID);
	glActiveTexture(GL_TEXTURE0);
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

	GLuint shaderv = loadShader(GL_VERTEX_SHADER, "shaders\\VertexShader.glsl");
	GLuint shaderf = loadShader(GL_FRAGMENT_SHADER, "shaders\\FragmentShader.glsl");
	GLuint shaderc = loadShader(GL_TESS_CONTROL_SHADER, "shaders\\ControlShader.glsl");
	GLuint shadere = loadShader(GL_TESS_EVALUATION_SHADER, "shaders\\EvaluationShader.glsl");

	GLuint program = glCreateProgram();
	glAttachShader(program, shaderv);
	glAttachShader(program, shaderf);
	glAttachShader(program, shaderc);
	glAttachShader(program, shadere);
	glLinkProgram(program);

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

	matrixLoc = glGetUniformLocation(program, "mvpMatrix");
	loadTextures();
	GLuint texLoc = glGetUniformLocation(program, "heightMapSampler");
	glUniform1i(texLoc, 0);
	GLuint heightMapWidth = glGetUniformLocation(program, "heightMapWidth");
	glUniform1i(heightMapWidth, HEIGHTMAP_WIDTH);
	GLuint heightMapHeight = glGetUniformLocation(program, "heightMapHeight");
	glUniform1i(heightMapHeight, HEIGHTMAP_HEIGHT);
	GLuint heightMapHeightScale = glGetUniformLocation(program, "heightMapHeightScale");
	glUniform1f(heightMapHeightScale, HEIGHTMAP_SCALEHEIGHT);

	proj = perspective(20.0f, 1.0f, 10.0f, 1000.0f);  //perspective projection matrix
	glPatchParameteri(GL_PATCH_VERTICES, 4);

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
	vec3 look = rotate(lookPos - cameraPos, radians(horizontalAngle), vec3(0.0, 1.0, 0.0)) + cameraPos;
	mat4 view = lookAt(cameraPos, look, upVec); //view matrix
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
		cameraPos += vec3(0.0, 1.0, 0.0);
		lookPos += vec3(0.0, 1.0, 0.0);
		break;
	case GLUT_KEY_DOWN:
		cameraPos += vec3(0.0, -1.0, 0.0);
		lookPos += vec3(0.0, -1.0, 0.0);
		break;
	case GLUT_KEY_LEFT:
		horizontalAngle += 2;
		break;
	case GLUT_KEY_RIGHT:
		horizontalAngle -= 2;
		break;
	default:
		return;
	}

	glutPostRedisplay();
}

void handleKeypress(unsigned char key, int x, int y)
{
	vec3 change;
	switch (key)
	{
	case 'w':
		change = vec3(0.0, 0.0, -1.0);
		break;
	case 'a':
		change = vec3(-1.0, 0.0, 0.0);
		break;
	case 's':
		change = vec3(0.0, 0.0, 1.0);
		break;
	case 'd':
		change = vec3(1.0, 0.0, 0.0);
		break;

	default: return;
	}
	change = rotate(change, radians(horizontalAngle), vec3(0.0, 1.0, 0.0));
	cameraPos += change;
	lookPos += change;
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	populateGrid();

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(1000, 1000);
	glutCreateWindow("Terrain");
	glutInitContextVersion (4, 2);
	glutInitContextProfile ( GLUT_CORE_PROFILE );

	if(glewInit() == GLEW_OK)
	{
		cout << "GLEW initialization successful! " << endl;
		cout << " Using GLEW version " << glewGetString(GLEW_VERSION) << endl;

		cout << glGetString(GL_VERSION) << endl;
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

