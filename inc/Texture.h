#ifndef BFH_TEXTURE_H
#define BFH_TEXTURE_H

#include <cstdint>
#include <ciso646>

#include "Program.h"


namespace bfh
{

enum class EMagFilter
{
	eNearest, eLinear,
};

enum class EMinFilter
{
	eNearest, eLinear,
};

class Texture2D final
{
public:
	static Texture2D* fromFile(const char* szFilePath);

	static Texture2D* fromSource(const void* pData, const std::int32_t iWidth, const std::int32_t iHeight);

private:
	mutable uint32_t k_uiIdx;

public:
	Texture2D(const Texture2D& xTexture) = delete;

	Texture2D(const Texture2D&& xTexture);

	~Texture2D();

	Texture2D& operator=(const Texture2D& xTexture) = delete;

	Texture2D& operator=(const Texture2D&& xTexture);

	void bind(const bfh::CProgram& xProgram, const std::int32_t uiIdx, const char* szName) const;

	void setMagnificationFilter(const EMagFilter& xFilter) const;

	void setMinificationFilter(const EMinFilter& xFilter) const;

private:
	Texture2D(const std::uint32_t uiId);
};

}


#endif
