#ifndef BFH_PROGRAM_H
#define BFH_PROGRAM_H

#include <stdexcept>
#include <cstdint>
#include <map>
#include <ciso646>

#include "Shader.h"
#include "Uniform.h"
#include "Logging.h"


namespace bfh
{


class CProgramLinkException final : public std::exception
{
public:
	CProgramLinkException(const char* szMessage) : std::exception(szMessage)
	{
	}
};


class CProgramIncompleteException final : public std::exception
{
public:
	CProgramIncompleteException(const char* szMessage) : std::exception(szMessage)
	{
	}
};


class CProgram final
{
public:
	class Builder;


private:
	mutable std::uint32_t m_uiId;

	mutable std::map<const char*, int32_t> m_mapLocations;

	mutable std::map<std::string, CUniformBase*> m_mapUniforms;


public:
	CProgram(const CProgram& xProgram) = delete;

	CProgram(const CProgram&& xProgram);

	~CProgram();

	CProgram& operator=(const CProgram& xProgram) = delete;

	CProgram& operator=(const CProgram&& xProgram);

	void bind() const;

	void unbind() const;

	template<typename T>
	CUniform<T>* uniform(const char* szName) const { return static_cast<CUniform<T>*>(m_mapUniforms[szName]); }

private:
	CProgram(const std::uint32_t uiId, const std::map<std::string, CUniformBase*> mapUniforms);

	int32_t insertOrGetLocation(const char* szName) const;
};


static_assert(not std::is_copy_assignable<CProgram>::value, "CProgram mustn't be copy-assignable");
static_assert(not std::is_copy_constructible<CProgram>::value, "CProgram mustn't be copy-constructible");
static_assert(std::is_move_assignable<CProgram>::value, "CProgram must be move-assignable");
static_assert(std::is_move_constructible<CProgram>::value, "CProgram must be move-constructible");


class CProgram::Builder final
{
private:
	std::map<EShaderType, std::uint32_t> m_mapShaders;

	std::map<const char*, CUniformBase::Builder*> m_mapBuilders;

public:
	CProgram::Builder();

	bool hasShader(const EShaderType eType) const;

	CProgram::Builder& setShader(const CShader& xShader);

	template<typename T>
	CProgram::Builder& registerUniform(const char* szName);

	CProgram build();

	CProgram* buildPointer();
};


template<typename T>
CProgram::Builder& CProgram::Builder::registerUniform(const char* szName)
{
	m_mapBuilders[szName] = TRACE_NEW CUniform<T>::Builder();
	return *this;
}


}


#endif
