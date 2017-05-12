#include "Texture.h"

#include <fstream>

#include "GL/glew.h"
#include "SOIL.h"

#include "Logging.h"
#include "Memory.h"

using namespace std;
using namespace bfh;


Texture2D* Texture2D::fromFile(const char* szFilePath)
{
	int32_t iWidth, iHeight;
	unsigned char* const pData = SOIL_load_image(szFilePath, &iWidth, &iHeight, nullptr, SOIL_LOAD_RGBA);
	Texture2D* pTexture = fromSource(static_cast<const void*>(pData), iWidth, iHeight);
	SOIL_free_image_data(pData);
	return pTexture;
}


Texture2D* Texture2D::fromSource(const void* pData, const int32_t iWidth, const int32_t iHeight)
{
	GLuint uiTextureId;
	glGenTextures(static_cast<GLsizei>(1), &uiTextureId);

	glBindTexture(GL_TEXTURE_2D, uiTextureId);
	glTexImage2D(GL_TEXTURE_2D, static_cast<GLint>(0), GL_RGBA, static_cast<GLsizei>(iWidth), static_cast<GLsizei>(iHeight), 0, GL_RGBA, GL_UNSIGNED_BYTE, pData);
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	return TRACE_NEW Texture2D(uiTextureId);
}


Texture2D::Texture2D(const Texture2D&& xTexture) : k_uiIdx(xTexture.k_uiIdx)
{
	xTexture.k_uiIdx = 0;
}


Texture2D::Texture2D(const uint32_t uiId) : k_uiIdx(uiId)
{
}


Texture2D::~Texture2D()
{
	glDeleteTextures(static_cast<GLsizei>(1), static_cast<const GLuint*>(&k_uiIdx));
}


Texture2D& Texture2D::operator=(const Texture2D&& xTexture)
{
	k_uiIdx = xTexture.k_uiIdx;
	xTexture.k_uiIdx = 0;
	return *this;
}


void Texture2D::bind(const CProgram& xProgram, const int32_t iIdx, const char* szName) const
{
	glActiveTexture(GL_TEXTURE0 + iIdx);
	glBindTexture(GL_TEXTURE_2D, k_uiIdx);
	xProgram.uniform<int32_t>(szName)->set(iIdx);
}


void Texture2D::setMagnificationFilter(const EMagFilter& xFilter) const
{
	glBindTexture(GL_TEXTURE_2D, k_uiIdx);
	switch(xFilter)
	{
	case EMagFilter::eLinear:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		break;
	case EMagFilter::eNearest:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		break;
	}
}


void Texture2D::setMinificationFilter(const EMinFilter& xFilter) const
{
	glBindTexture(GL_TEXTURE_2D, k_uiIdx);
	switch(xFilter)
	{
	case EMinFilter::eLinear:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		break;
	case EMinFilter::eNearest:
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		break;
	}
}
