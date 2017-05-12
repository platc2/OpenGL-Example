#ifndef MEMORY_H
#define MEMORY_H

#include <cstddef>
#include <cstdint>
#include <map>

#include "Logging.h"


// alias definitions
using line_t = std::uint32_t;


struct SInfo
{
	char* szFile;
	line_t uiLine;
	int32_t iCount;
};

template<typename K, typename V>
class CMap final : public std::map<K, V>
{
public:
	~CMap()
	{
		logMessage("Hah, gayyyyy!");
		cleanup();
	}

	void cleanup();

	void* modify(void* const pLocation, const char* szFile, const line_t uiLine, const std::int32_t iDelta);
};

extern CMap<void*, SInfo*> mapAllocs;


// forward declarations
void* operator new(const size_t xSize, const char* szFile, const line_t uiLine);

void* operator new[](const size_t xSize, const char* szFile, const line_t uiLine);

template<typename T>
void mem_delete(T* const pLocation, const char* szFile, const line_t uiLine)
{
	if(pLocation)
	{
		if(mapAllocs.modify(static_cast<void* const>(pLocation), szFile, uiLine, -1))
		{
			::delete pLocation;
		}
	}
	else
	{
		logWarn("Deleting 'nullptr' @ %s:%u\n", szFile, uiLine);
	}
}

template<typename T>
void mem_delete_arr(T* const pLocation, const char* szFile, const line_t uiLine)
{
	if(pLocation)
	{
		if(mapAllocs.modify(pLocation, szFile, uiLine, -1))
		{
			::delete[] pLocation;
		}
	}
	else
	{
		logWarn("Deleting 'nullptr' @ %s:%u\n", szFile, uiLine);
	}
}

#define TRACE_NEW new(__FILE__, __LINE__)
#define TRACE_NEW_ARR new(__FILE__, __LINE__)
#define TRACE_DELETE(x) do { mem_delete(x, __FILE__, __LINE__); } while(0)
#define TRACE_DELETE_ARR(x) do { mem_delete_arr(x, __FILE__, __LINE__); } while(0)


#endif

