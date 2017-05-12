#ifndef BFH_CUBE_H
#define BFH_CUBE_H

#include <ciso646>
#include "GL/glew.h"


namespace bfh
{

class CProgram;


class CCube
{
private:
	static const CCube k_xInstance;

	mutable GLuint m_xVertexArrayObject;
	mutable GLuint m_xVertexBufferObject;
	mutable GLuint m_xElementBufferObject;
	mutable bool m_bInitialized;


public:
	static const CCube& getInstance()
	{
		if(not k_xInstance.initialized())
		{
			k_xInstance.init();
		}
		return k_xInstance;
	}

	void draw(const CProgram& xProgram) const;


private:
	CCube() : m_xVertexArrayObject(0), m_xVertexBufferObject(0), m_xElementBufferObject(0), m_bInitialized(false)
	{
	}

	void init() const;

	bool initialized() const
	{
		return m_bInitialized;
	}
};

}


#endif
