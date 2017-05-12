#include <ciso646>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "SOIL.h"
// OpenGL wrapper- and helperclasses
#include "Shader.h"
#include "Program.h"
#include "Texture.h"
#include "Cube.h"
#include "Camera.h"
// Logging & Memorymanagement
#include "Logging.h"
#include "Memory.h"

using namespace bfh;
using namespace glm;
using namespace std;


struct SWindowSettings
{
	enum
	{
		X = 100,
		Y = 100,
		Width = 800,
		Height = 800,
	};
};

static float fFov = 60.0f;

static CProgram* pProgram, *pLightProgram;
static Texture2D* pDiffuseTexture, *pSpecularTexture;
static CCamera xCamera;
static bool bWireframe = false, bLinear = false, bFirstMouseMovement = false;
static bool rgKeys[1024];

//static vec3 xLightPos = vec3(0.5f, 0.5f, 1.3f);
static vec3 xLightPos = vec3(1.2f, 1.0f, 2.0f);
static vec3 xLightColor = vec3(1.0f, 1.0f, 1.0f);

static double dOldX = 0.0, dOldY = 0.0;
static float dDeltaTime = 0.0, dLastFrame = 0.0;

static vec3 rgPositions[] =
{
	vec3(0.0f,  0.0f,  0.0f),
	vec3(2.0f,  5.0f, -15.0f),
	vec3(-1.5f, -2.2f, -2.5f),
	vec3(-3.8f, -2.0f, -12.3f),
	vec3(2.4f, -0.4f, -3.5f),
	vec3(-1.7f,  3.0f, -7.5f),
	vec3(1.3f, -2.0f, -2.5f),
	vec3(1.5f,  2.0f, -2.5f),
	vec3(1.5f,  0.2f, -1.5f),
	vec3(-1.3f,  1.0f, -1.5f)
};


bool checkError()
{
	const GLenum xError = glGetError();
	if(xError == GL_NO_ERROR)
	{
		return false;
	}

	logError("GLError: %s", gluErrorString(xError));
	return true;
}


void loadProgram()
{
	try
	{
		pProgram = CProgram::Builder()
			// vertex shader
			.setShader(CShader::fromFile(EShaderType::eVertexShader,
				"C:\\Users\\Casimir\\Desktop\\C++Intro\\CPP_Projects\\CPP-OpenGL\\glsl\\vertex.vert"))
			// fragment shader
			.setShader(CShader::fromFile(EShaderType::eFragmentShader,
				"C:\\Users\\Casimir\\Desktop\\C++Intro\\CPP_Projects\\CPP-OpenGL\\glsl\\fragment.frag"))
			// register uniforms
			.registerUniform<mat4>("projection")
			.registerUniform<mat4>("view")
			.registerUniform<mat4>("model")
			.registerUniform<vec3>("viewPos")
			.registerUniform<bool>("wireframe")
			.registerUniform<vec3>("material.ambient")
			.registerUniform<int32_t>("material.diffuse")
			.registerUniform<int32_t>("material.specular")
			.registerUniform<float>("material.shininess")
			.registerUniform<vec3>("light.position")
			.registerUniform<vec3>("light.ambient")
			.registerUniform<vec3>("light.diffuse")
			.registerUniform<vec3>("light.specular")
			.buildPointer();

		pLightProgram = CProgram::Builder()
			.setShader(CShader::fromFile(EShaderType::eVertexShader,
				"C:\\Users\\Casimir\\Desktop\\C++Intro\\CPP_Projects\\CPP-OpenGL\\glsl\\light.vert"))
			.setShader(CShader::fromFile(EShaderType::eFragmentShader,
				"C:\\Users\\Casimir\\Desktop\\C++Intro\\CPP_Projects\\CPP-OpenGL\\glsl\\light.frag"))
			.registerUniform<mat4>("projection")
			.registerUniform<mat4>("view")
			.registerUniform<mat4>("model")
			.registerUniform<vec3>("lightColor")
			.buildPointer();
	}
	catch(const exception& eException)
	{
		logFatal("Error in program creation:\n%s", eException.what());
		exit(1);
	}
}


void loadTexture()
{
	pDiffuseTexture = Texture2D::fromFile("C:\\Users\\Casimir\\Desktop\\C++Intro\\CPP_Projects\\CPP-OpenGL\\res\\container.png");
	pSpecularTexture = Texture2D::fromFile("C:\\Users\\Casimir\\Desktop\\C++Intro\\CPP_Projects\\CPP-OpenGL\\res\\container_s.png");
}


void init()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	glEnable(GL_CULL_FACE);
	glEnable(GL_MULTISAMPLE);

	glEnable(GL_DEPTH_TEST);

	loadProgram();
	loadTexture();

	if(checkError())
	{
		exit(0);
	}
}


void cleanup()
{
	TRACE_DELETE(pProgram);
	TRACE_DELETE(pLightProgram);

	TRACE_DELETE(pDiffuseTexture);
	TRACE_DELETE(pSpecularTexture);
}


void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	pProgram->bind();
	const mat4 xProjection = perspective(radians(fFov), SWindowSettings::Width / static_cast<float>(SWindowSettings::Height), 0.1f, 100.0f);
	const mat4 xView = xCamera.viewMatrix();
	pProgram->uniform<mat4>("projection")->set(xProjection);
	pProgram->uniform<mat4>("view")->set(xView);
	pProgram->uniform<vec3>("viewPos")->set(inverse(xView) * vec4(0.0f, 0.0f, 0.0f, 1.0f));

	pProgram->uniform<vec3>("material.ambient")->set(vec3(1.0f, 0.5f, 0.31f));
	pDiffuseTexture->bind(*pProgram, 0, "material.diffuse");
		pDiffuseTexture->setMinificationFilter(bLinear ? EMinFilter::eLinear : EMinFilter::eNearest);
		pDiffuseTexture->setMagnificationFilter(bLinear ? EMagFilter::eLinear : EMagFilter::eNearest);
	pSpecularTexture->bind(*pProgram, 1, "material.specular");
		pSpecularTexture->setMinificationFilter(bLinear ? EMinFilter::eLinear : EMinFilter::eNearest);
		pSpecularTexture->setMagnificationFilter(bLinear ? EMagFilter::eLinear : EMagFilter::eNearest);
	pProgram->uniform<float>("material.shininess")->set(32.0f);

	vec3 xLightColor;
	xLightColor.x = sin(glfwGetTime() * 2.0f);
	xLightColor.y = sin(glfwGetTime() * 0.7f);
	xLightColor.z = sin(glfwGetTime() * 1.3f);
	xLightColor = vec3(1.0f);
	pProgram->uniform<vec3>("light.ambient")->set(xLightColor * 0.2f);
	pProgram->uniform<vec3>("light.diffuse")->set(xLightColor * 0.5f);
	pProgram->uniform<vec3>("light.specular")->set(vec3(1.0f, 1.0f, 1.0f));
	pProgram->uniform<vec3>("light.position")->set(xLightPos);

	glPolygonMode(GL_FRONT_AND_BACK, bWireframe ? GL_LINE : GL_FILL);
	pProgram->uniform<bool>("wireframe")->set(bWireframe);
	if(bWireframe)
	{
		glDisable(GL_CULL_FACE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
	}

	CCube::getInstance().draw(*pProgram);

	float fTime = glfwGetTime();
	for(GLuint i = 0; i < 10; ++i)
	{
		mat4 xModel;
		xModel = translate(xModel, rgPositions[i]);
		GLfloat angle = (20.0f + fTime * 10.0f) * (i + 1);
		xModel = rotate(xModel, radians(angle), fTime * vec3(1.0f, 0.3f, 0.5f));
		xModel = scale(xModel, vec3(0.5f));
		pProgram->uniform<mat4>("model")->set(xModel);

		CCube::getInstance().draw(*pProgram);
	}

	pLightProgram->bind();
	pLightProgram->uniform<mat4>("projection")->set(xProjection);
	pLightProgram->uniform<mat4>("view")->set(xView);
	pLightProgram->uniform<mat4>("model")->set(scale(translate(mat4(), xLightPos), vec3(0.1f)));
	pLightProgram->uniform<vec3>("lightColor")->set(xLightColor);
	CCube::getInstance().draw(*pLightProgram);

	// check for errors
	checkError();
}


void update()
{
	vec3 xMovement = vec3();
	if(rgKeys[GLFW_KEY_W])
	{
		xMovement.z += 1.0f;
	}
	if(rgKeys[GLFW_KEY_S])
	{
		xMovement.z -= 1.0f;
	}
	if(rgKeys[GLFW_KEY_D])
	{
		xMovement.x += 1.0f;
	}
	if(rgKeys[GLFW_KEY_A])
	{
		xMovement.x -= 1.0f;
	}
	if(rgKeys[GLFW_KEY_SPACE])
	{
		xMovement.y += 1.0f;
	}
	if(rgKeys[GLFW_KEY_C])
	{
		xMovement.y -= 1.0f;
	}
	if(xMovement.x or xMovement.y or xMovement.z)
	{
		xMovement = normalize(xMovement);
	}
	xMovement *= dDeltaTime * (rgKeys[GLFW_KEY_LEFT_SHIFT] ? 10.0f : 1.0f);
	xCamera.moveRight(xMovement.x);
	xCamera.moveUp(xMovement.y);
	xCamera.moveForward(xMovement.z);
}


void input(GLFWwindow* const pWindow, const int32_t iKey, const int32_t iScanCode, const int32_t iAction, const int32_t iMode)
{
	if(iAction == GLFW_PRESS and not (iMode & GLFW_MOD_ALT))
	{
		rgKeys[iKey] = true;
	}
	else if(iAction == GLFW_RELEASE and not (iMode & GLFW_MOD_ALT))
	{
		rgKeys[iKey] = false;
	}

	switch(iKey)
	{
	case GLFW_KEY_ESCAPE:
		if(iAction == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(pWindow, GL_TRUE);
		}
		break;
	case GLFW_KEY_W:
		if(iAction == GLFW_PRESS and iMode & GLFW_MOD_ALT)
		{
			bWireframe = not bWireframe;
		}
		break;
	case GLFW_KEY_M:
		if(iAction == GLFW_PRESS and iMode & GLFW_MOD_ALT)
		{
			bLinear = not bLinear;
		}
		break;
	}
}


void mouseMoved(GLFWwindow* const pWindow, const double dPosX, const double dPosY)
{
	if(not bFirstMouseMovement)
	{
		bFirstMouseMovement = true;
		dOldX = dPosX;
		dOldY = dPosY;
	}

	const double dDeltaX = (dPosX - dOldX);
	const double dDeltaY = -(dPosY - dOldY);  // invert as screen y-coordinates are inverted
	dOldX = dPosX;
	dOldY = dPosY;

	xCamera.lookUp(dDeltaY * 0.7f * dDeltaTime);
	xCamera.lookRight(dDeltaX * 0.7f * dDeltaTime);
}


void mouseScrolled(GLFWwindow* const pWindow, const double dOffsetX, const double dOffsetY)
{
	fFov += dOffsetY;
	fFov = max(30.0f, min(fFov, 170.0f));
}


int main(int iArgc, char* rgArgs[])
{
	// initialize GLFW
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	// initialize window
	GLFWwindow* const pWindow = glfwCreateWindow(SWindowSettings::Width, SWindowSettings::Height, "OpenGL Application", nullptr, nullptr);
	if(pWindow == nullptr)
	{
		logFatal("Failed to create GLFW");
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(pWindow, SWindowSettings::X, SWindowSettings::Y);

	// initialize OpenGL/GLEW
	glfwMakeContextCurrent(pWindow);
	glewInit();

	GLint iMaxAttribs, iMaxTextureUnits;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &iMaxAttribs);
	glGetIntegerv(GL_MAX_TEXTURE_UNITS, &iMaxTextureUnits);

	logMessage("OpenGL Version: %s, GLSL Version: %s", glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
	logMessage("Renderer: %s", glGetString(GL_RENDERER));
	logMessage("Vendor: %s", glGetString(GL_VENDOR));
	logMessage("Max vertex attributes: %d", iMaxAttribs);
	logMessage("Max texture units: %d", iMaxTextureUnits);
	

	// initialize callbacks
	glfwSetKeyCallback(pWindow, input);
	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(pWindow, mouseMoved);
	glfwSetScrollCallback(pWindow, mouseScrolled);

	// custom settings
	init();
	atexit(cleanup);

	while(not glfwWindowShouldClose(pWindow))
	{
		const double dCurrentTime = glfwGetTime();
		dDeltaTime = (dCurrentTime - dLastFrame);
		dLastFrame = dCurrentTime;

		glfwPollEvents();

		update();
		render();

		glfwSwapBuffers(pWindow);
	}
	glfwTerminate();

	return 0;
}
