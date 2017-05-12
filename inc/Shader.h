#ifndef BFH_SHADER_H
#define BFH_SHADER_H

#include <stdexcept>
#include <cstdint>
#include <ciso646>


namespace bfh
{

enum class EShaderType : std::uint8_t
{
	eVertexShader, eFragmentShader, eGeometryShader
};


class CShaderCompileException final : public std::exception
{
public:
	CShaderCompileException(const char* szMessage) : std::exception(szMessage)
	{
	}
};


class CShader final
{
	friend class CProgram;

public:
	static CShader fromSource(const EShaderType eType, const char* szSource);

	static CShader fromFile(const EShaderType eType, const char* szFilePath);

private:
	mutable std::uint32_t m_uiId;
	mutable EShaderType m_eType;

public:
	CShader(const CShader& xShader) = delete;

	CShader(const CShader&& xShader);

	~CShader();

	CShader& operator=(const CShader& xShader) = delete;

	CShader& operator=(const CShader&& xShader);

private:
	CShader(const uint32_t uiId, const EShaderType eType);
};


static_assert(not std::is_copy_assignable<CShader>::value, "CShader mustn't be copy-assignable");
static_assert(not std::is_copy_constructible<CShader>::value, "CShader mustn't be copy-constructible");
static_assert(std::is_move_assignable<CShader>::value, "CShader must be move-assignable");
static_assert(std::is_move_constructible<CShader>::value, "CShader must be move-constructible");


}


#endif
