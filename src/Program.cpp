#include "Program.h"

#include <ciso646>
#include <algorithm>

#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/mat3x3.hpp"
#include "glm/mat4x4.hpp"

#include "Memory.h"

using namespace std;
using namespace glm;
using namespace bfh;


CProgram::CProgram(const CProgram&& xProgram) : CProgram(xProgram.m_uiId, std::move(xProgram.m_mapUniforms))
{
	m_mapLocations = std::move(xProgram.m_mapLocations);
	xProgram.m_uiId = 0;
}


CProgram::CProgram(const uint32_t uiId, const map<string, CUniformBase*> mapUniforms) : m_uiId(uiId), m_mapLocations(), m_mapUniforms(std::move(mapUniforms))
{
}


CProgram::~CProgram()
{
	glDeleteProgram(static_cast<GLuint>(m_uiId));

	for(const pair<string, CUniformBase*>& xPair : m_mapUniforms)
	{
		TRACE_DELETE(xPair.second);
	}
}


CProgram& CProgram::operator=(const CProgram&& xProgram)
{
	m_uiId = xProgram.m_uiId;
	m_mapLocations = std::move(xProgram.m_mapLocations);
	xProgram.m_uiId = 0;
	return *this;
}


void CProgram::bind() const
{
	glUseProgram(static_cast<GLuint>(m_uiId));
}


void CProgram::unbind() const
{
	glUseProgram(static_cast<GLuint>(0));
}


int32_t CProgram::insertOrGetLocation(const char* szName) const
{
	if(m_mapLocations.find(szName) == m_mapLocations.end())
	{
		m_mapLocations[szName] = glGetUniformLocation(m_uiId, szName);
	}
	return m_mapLocations[szName];
}


CProgram::Builder::Builder()
{
}


CProgram::Builder& CProgram::Builder::setShader(const CShader& xShader)
{
	m_mapShaders[xShader.m_eType] = xShader.m_uiId;
	return *this;
}


bool CProgram::Builder::hasShader(const EShaderType eType) const
{
	return not (m_mapShaders.find(eType) == m_mapShaders.end());
}


CProgram CProgram::Builder::build()
{
	if(not hasShader(EShaderType::eVertexShader))
	{
		throw CProgramIncompleteException("Vertex shader is missing!");
	}
	if(not hasShader(EShaderType::eFragmentShader))
	{
		throw CProgramIncompleteException("Fragment shader is missing!");
	}

	const GLuint uiProgram = glCreateProgram();

	for(const pair<EShaderType, uint32_t>& xPair : m_mapShaders)
	{
		glAttachShader(uiProgram, static_cast<GLuint>(xPair.second));
	}

	glLinkProgram(uiProgram);

	GLint iStatus;
	glGetProgramiv(uiProgram, GL_LINK_STATUS, &iStatus);
	if(iStatus == GL_FALSE)
	{
		GLint iInfoLogLength;
		glGetProgramiv(uiProgram, GL_INFO_LOG_LENGTH, &iInfoLogLength);
		GLchar* szInfoLog = TRACE_NEW_ARR(GLchar[iInfoLogLength + 1]);
		glGetProgramInfoLog(uiProgram, iInfoLogLength, nullptr, szInfoLog);
		CProgramLinkException xException(szInfoLog);
		TRACE_DELETE_ARR(szInfoLog);
		glDeleteProgram(uiProgram);
		throw xException;
	}

	for(const pair<EShaderType, uint32_t>& xPair : m_mapShaders)
	{
		glDetachShader(uiProgram, static_cast<GLuint>(xPair.second));
	}

	map<string, CUniformBase*> mapUniforms;
	for(const pair<const char*, CUniformBase::Builder*>& xPair : m_mapBuilders)
	{
		mapUniforms[xPair.first] = (xPair.second)->build(xPair.first, uiProgram);
		TRACE_DELETE(xPair.second);
	}

	return CProgram(uiProgram, mapUniforms);
}


CProgram* CProgram::Builder::buildPointer()
{
	return TRACE_NEW CProgram(build());
}
