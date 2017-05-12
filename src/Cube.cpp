#include "Cube.h"
#include <vector>
#include "Program.h"

using namespace std;
using namespace bfh;


// vertices
#define L -1.0f
#define R  1.0f
#define U  1.0f
#define D -1.0f
#define F  1.0f
#define B -1.0f
// normals
#define LD L, 0.0f, 0.0f
#define RD R, 0.0f, 0.0f
#define UD 0.0f, U, 0.0f
#define DD 0.0f, D, 0.0f
#define FD 0.0f, 0.0f, F
#define BD 0.0f, 0.0f, B


struct SVertexData
{
	const float rgVertex[3];
	const float rgNormal[3];
	const float rgTexture[2];
};

const SVertexData rgData[] =
{
	// front face
	{ L, D, F,  FD,  0.0f, 0.0f },
	{ R, D, F,  FD,  1.0f, 0.0f },
	{ R, U, F,  FD,  1.0f, 1.0f },
	{ L, U, F,  FD,  0.0f, 1.0f },
	// right face
	{ R, D, F,  RD,  0.0f, 0.0f },
	{ R, D, B,  RD,  1.0f, 0.0f },
	{ R, U, B,  RD,  1.0f, 1.0f },
	{ R, U, F,  RD,  0.0f, 1.0f },
	// back face
	{ R, D, B,  BD,  0.0f, 0.0f },
	{ L, D, B,  BD,  1.0f, 0.0f },
	{ L, U, B,  BD,  1.0f, 1.0f },
	{ R, U, B,  BD,  0.0f, 1.0f },
	// left face
	{ L, D, B,  LD,  0.0f, 0.0f },
	{ L, D, F,  LD,  1.0f, 0.0f },
	{ L, U, F,  LD,  1.0f, 1.0f },
	{ L, U, B,  LD,  0.0f, 1.0f },
	// top face
	{ L, U, F,  UD,  0.0f, 0.0f },
	{ R, U, F,  UD,  1.0f, 0.0f },
	{ R, U, B,  UD,  1.0f, 1.0f },
	{ L, U, B,  UD,  0.0f, 1.0f },
	// down face
	{ L, D, B,  DD,  0.0f, 0.0f },
	{ R, D, B,  DD,  1.0f, 0.0f },
	{ R, D, F,  DD,  1.0f, 1.0f },
	{ L, D, F,  DD,  0.0f, 1.0f },
};

static const uint32_t rgIndices[] =
{
	 0,  1,  2,  0,  2,  3,  // front face
	 4,  5,  6,  4,  6,  7,  // right face
	 8,  9, 10,  8, 10, 11,  // back face
	12, 13, 14, 12, 14, 15,  // left face
	16, 17, 18, 16, 18, 19,  // top face
	20, 21, 22, 20, 22, 23,  // down face
};



const CCube CCube::k_xInstance{};


void CCube::init() const
{
	glGenBuffers(static_cast<GLsizei>(1), &m_xVertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_xVertexBufferObject);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rgData), rgData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, static_cast<GLuint>(0));
	glGenBuffers(static_cast<GLsizei>(1), &m_xElementBufferObject);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_xElementBufferObject);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(rgIndices), rgIndices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLuint>(0));

	glGenVertexArrays(static_cast<GLsizei>(1), &m_xVertexArrayObject);
	glBindVertexArray(m_xVertexArrayObject);
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_xElementBufferObject);
		glBindBuffer(GL_ARRAY_BUFFER, m_xVertexBufferObject);
		glEnableVertexAttribArray(static_cast<GLuint>(0));
		glVertexAttribPointer(static_cast<GLuint>(0), static_cast<GLint>(3), GL_FLOAT, GL_FALSE, sizeof(SVertexData), nullptr);
		glEnableVertexAttribArray(static_cast<GLuint>(1));
		glVertexAttribPointer(static_cast<GLuint>(1), static_cast<GLint>(3), GL_FLOAT, GL_FALSE, sizeof(SVertexData), reinterpret_cast<const void*>(offsetof(SVertexData, rgNormal)));
		glEnableVertexAttribArray(static_cast<GLuint>(2));
		glVertexAttribPointer(static_cast<GLuint>(2), static_cast<GLint>(2), GL_FLOAT, GL_FALSE, sizeof(SVertexData), reinterpret_cast<const void*>(offsetof(SVertexData, rgTexture)));
	}
	glBindVertexArray(static_cast<GLuint>(0));

	m_bInitialized = true;
}


void CCube::draw(const CProgram& xProgram) const
{
	xProgram.bind();

	glBindVertexArray(m_xVertexArrayObject);

	glDrawElements(GL_TRIANGLES, sizeof(rgIndices) / sizeof(uint32_t), GL_UNSIGNED_INT, nullptr);

	glBindVertexArray(0);
}
