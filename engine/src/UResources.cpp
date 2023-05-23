#include "UResources.h"
#include "Utopia.h"

using namespace utopia;

HMODULE GCM()
{
	HMODULE hModule = NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)GCM, &hModule);
	return hModule;
}

UResources& UResources::getInstance()
{
	static UResources instance;
	return instance;
}

std::string& utopia::UResources::getStringResource(int resourceId, int resourceType)
{
	HMODULE hModule = GCM();
	HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(resourceId), MAKEINTRESOURCE(resourceType));
	DWORD size = SizeofResource(hModule, hResource);
	HGLOBAL hGlobal = LoadResource(hModule, hResource);
	LPVOID data = LockResource(hGlobal);
	std::string* str = new std::string((char*)data, size);
	return *str;
}




