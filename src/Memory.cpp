#include "Memory.h"
#include "Logging.h"
#include <iostream>
#include <map>
#include <cstdint>
#include <cstring>

using namespace std;



template<typename K, typename V>
void CMap<K, V>::cleanup()
{
	for(auto xElem : *this)
	{
		SInfo* pStruct = xElem.second;
		if(pStruct->iCount > 0)
		{
			logError("Memory not freed @ %s:%u\n", pStruct->szFile, pStruct->uiLine);
		}
		free(pStruct->szFile);
		free(pStruct);
	}

	// empty the map, in case we would call it a second time...!
	this->clear();
}

template<typename K, typename V>
void* CMap<K, V>::modify(void* const pLocation, const char* szFile, const line_t uiLine, const int32_t iDelta)
{
	SInfo* pInfo = nullptr;
	if(this->find(pLocation) == this->end())
	{
		pInfo = static_cast<SInfo*>(malloc(sizeof(SInfo)));
		pInfo->szFile = _strdup(szFile);
		pInfo->uiLine = uiLine;
		pInfo->iCount = 0;
		this->emplace(pLocation, pInfo);
	}
	else
	{
		pInfo = this->operator[](pLocation);
	}
	pInfo->iCount += iDelta;

	if(pInfo->iCount < 0)
	{
		logError("Memory already freed @ %s:%u\n", szFile, uiLine);
		return nullptr;
	}

	return pLocation;
}


CMap<void*, SInfo*> mapAllocs;


void* operator new(const size_t xSize, const char* szFile, const line_t uiLine)
{
	return (xSize ? mapAllocs.modify(static_cast<void*>(malloc(xSize)), szFile, uiLine, 1) : nullptr);
}


void* operator new[](const size_t xSize, const char* szFile, const line_t uiLine)
{
	return (xSize ? mapAllocs.modify(static_cast<void*>(malloc(xSize)), szFile, uiLine, 1) : nullptr);
}
