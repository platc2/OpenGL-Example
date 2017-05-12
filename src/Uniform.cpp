#include "Uniform.h"

#include "GL/glew.h"
#include "glm/gtc/type_ptr.hpp"
#include "glm/mat4x4.hpp"

using namespace std;
using namespace glm;
using namespace bfh;


/** sets the uniform to the specified value */
template<>
void CUniform<int32_t>::set(const int32_t& iValue)
{
	glUniform1i(static_cast<GLint>(k_iLocation), static_cast<GLint>(iValue));
}


/** sets the uniform to the specified value */
template<>
void CUniform<uint32_t>::set(const uint32_t& iValue)
{
	glUniform1ui(static_cast<GLint>(k_iLocation), static_cast<GLuint>(iValue));
}


/** sets the uniform to the specified value */
template<>
void CUniform<float>::set(const float& fValue)
{
	glUniform1f(static_cast<GLint>(k_iLocation), static_cast<GLfloat>(fValue));
}


/** sets the uniform to the specified value */
template<>
void CUniform<vec3>::set(const vec3& xValue)
{
	glUniform3fv(static_cast<GLint>(k_iLocation), static_cast<GLsizei>(1), value_ptr(xValue));
}


/** sets the uniform to the specified value */
template<>
void CUniform<mat4>::set(const mat4& xValue)
{
	glUniformMatrix4fv(static_cast<GLint>(k_iLocation), static_cast<GLsizei>(1), static_cast<GLboolean>(false), static_cast<const GLfloat*>(value_ptr(xValue)));
}


/** sets the uniform to the specified value */
template<>
void CUniform<bool>::set(const bool& bValue)
{
	glUniform1i(static_cast<GLint>(k_iLocation), static_cast<GLint>(bValue));
}
