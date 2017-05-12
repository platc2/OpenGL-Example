
#ifndef BFH_UNIFORM_H
#define BFH_UNIFORM_H
// TODO hide classes, make them only available when useful for client

#include <type_traits>
#include <cstdint>
#include <ciso646>

// TODO remove this ugly include
#include "GL/glew.h"

#include "Memory.h"


namespace bfh
{
/** base class for uniform values */
class CUniformBase
{
	/** friend class to access, to access private ctor */
	template<typename T>
	friend class CUniform;

public:
	class Builder;

public:
	/** deleted copy-ctor */
	CUniformBase(const CUniformBase& xBase) = delete;

	/** deleted move-ctor */
	CUniformBase(const CUniformBase&& xBase) = delete;

	/** deleted copy-assignment */
	CUniformBase& operator=(const CUniformBase& xBase) = delete;

	/** deleted move-assignment */
	CUniformBase& operator=(const CUniformBase&& xBase) = delete;

private:
	/** private ctor, prevent use for clients */
	CUniformBase() = default;
};

static_assert(not std::is_copy_constructible<CUniformBase>::value, "CUniformBase mustn't be copy-constructible!");
static_assert(not std::is_move_constructible<CUniformBase>::value, "CUniformBase mustn't be move-constructible!");
static_assert(not std::is_copy_assignable<CUniformBase>::value, "CUniformBase mustn't be copy-assignable!");
static_assert(not std::is_move_assignable<CUniformBase>::value, "CUniformBase mustn't be move-assignable!");


/** base class for uniform builders */
class CUniformBase::Builder
{
public:
	// TODO hide this constructor, make class non-instantiable
	/** default builder */
	CUniformBase::Builder() = default;

	/** deleted copy-ctor */
	CUniformBase::Builder(const CUniformBase::Builder& xBuilder) = delete;

	/** deleted move-ctor */
	CUniformBase::Builder(const CUniformBase::Builder&& xBuilder) = delete;

	/** deleted copy-assignment */
	CUniformBase::Builder& operator=(const CUniformBase::Builder& xBuilder) = delete;

	/** deleted move-assignment */
	CUniformBase::Builder& operator=(const CUniformBase::Builder&& xBuilder) = delete;

	/** constructs the uniform, given its name and the programid it is used for */
	virtual CUniformBase* build(const char* szName, const std::int32_t iProgramId) = 0;
};

static_assert(not std::is_copy_constructible<CUniformBase::Builder>::value, "CUniformBase::Builder mustn't be copy-constructible!");
static_assert(not std::is_move_constructible<CUniformBase::Builder>::value, "CUniformBase::Builder mustn't be move-constructible!");
static_assert(not std::is_copy_assignable<CUniformBase::Builder>::value, "CUniformBase::Builder mustn't be copy-assignable!");
static_assert(not std::is_move_assignable<CUniformBase::Builder>::value, "CUniformBase::Builder mustn't be move-assignable!");


/** uniform class */
template<typename T>
class CUniform final : public CUniformBase
{

public:
	class Builder;

private:
	/** location of the uniform */
	const int32_t k_iLocation;

public:
	/** deleted copy-ctor */
	CUniform(const CUniform& xUniform) = delete;

	/** deleted move-ctor */
	CUniform(const CUniform&& xUniform) = delete;

	/** deleted copy-assignment */
	CUniform& operator=(const CUniform& xUniform) = delete;

	CUniform& operator=(const CUniform&& xUniform) = delete;

	/** sets the uniform to the specified value */
	void set(const T& xValue);

private:
	// TODO hide the call to glGetUniformLocation
	/** private ctor, we don't want clients to instantiate this class accidently */
	CUniform(const char* szName, const int32_t iProgramId) : k_iLocation(glGetUniformLocation(iProgramId, szName))
	{
	}
};


/** builder for uniforms */
template<typename T>
class CUniform<T>::Builder final : public CUniformBase::Builder
{
public:
	/** constructs the uniform, given its name and the programid it is used for */
	CUniformBase* build(const char* szName, const int32_t iProgramId) override
	{
		return TRACE_NEW CUniform<T>(szName, iProgramId);
	}
};

}


#endif
