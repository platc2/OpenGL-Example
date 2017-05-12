#include "Shader.h"

#include <string>
#include <fstream>
#include <sstream>
#include "GL/glew.h"

#include "Memory.h"

using namespace std;
using namespace bfh;


static GLuint convertShaderType(const EShaderType eType)
{
	switch(eType)
	{
	case EShaderType::eVertexShader:
		return GL_VERTEX_SHADER;
	case EShaderType::eFragmentShader:
		return GL_FRAGMENT_SHADER;
	case EShaderType::eGeometryShader:
		return GL_GEOMETRY_SHADER;
	}
}


CShader::CShader(const CShader&& xShader) : CShader(xShader.m_uiId, xShader.m_eType)
{
	xShader.m_uiId = 0;
}


CShader::CShader(const uint32_t uiId, const EShaderType eType) : m_uiId(uiId), m_eType(eType)
{
}


CShader::~CShader()
{
	glDeleteShader(static_cast<GLuint>(m_uiId));
}


CShader& CShader::operator=(const CShader&& xShader)
{
	m_uiId = xShader.m_uiId;
	m_eType = xShader.m_eType;
	xShader.m_uiId = 0;
	return *this;
}


CShader CShader::fromFile(const EShaderType eType, const char* const szFilePath)
{
	ifstream xFileStream(szFilePath);
	if(not xFileStream.good())
	{
		throw std::runtime_error(string("Could not read file \"") + szFilePath + string("\""));
	}

	stringstream xSourceStream;
	xSourceStream << xFileStream.rdbuf();
	return CShader::fromSource(eType, xSourceStream.str().c_str());
	stringstream xStream(szFilePath);
}


CShader CShader::fromSource(const EShaderType eType, const char* const szSource)
{
	const GLuint uiShader = glCreateShader(convertShaderType(eType));

	glShaderSource(uiShader, static_cast<GLsizei>(1), &static_cast<const GLchar* const>(szSource), nullptr);
	glCompileShader(uiShader);

	// check compile status
	GLint iStatus;
	glGetShaderiv(uiShader, GL_COMPILE_STATUS, &iStatus);
	if(iStatus == GL_FALSE)
	{
		GLint iInfoLogLength;
		glGetShaderiv(uiShader, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		GLchar* szInfoLog = TRACE_NEW_ARR(GLchar[iInfoLogLength + 1]);
		glGetShaderInfoLog(uiShader, iInfoLogLength, nullptr, szInfoLog);
		CShaderCompileException eException(szInfoLog);
		TRACE_DELETE_ARR(szInfoLog);
		glDeleteShader(uiShader);
		throw eException;
	}

	return CShader(uiShader, eType);
}
