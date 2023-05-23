#include "UResources.h"
#include "Utopia.h"

using namespace utopia;
static HMODULE hModule;

HMODULE GCM()
{
	HMODULE hModule = NULL;
	GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS, (LPCTSTR)GCM, &hModule);
	return hModule;
}

utopia::UResources::~UResources()
{
	FreeLibrary(hModule);
}

UResources& UResources::getInstance()
{
	static UResources instance;
	return instance;
}

std::unique_ptr<std::string> utopia::UResources::getStringResource(int resourceId, int resourceType)
{
	hModule = GCM();
	HRSRC hResource = FindResource(hModule, MAKEINTRESOURCE(resourceId), MAKEINTRESOURCE(resourceType));
	DWORD size = SizeofResource(hModule, hResource);
	HGLOBAL hGlobal = LoadResource(hModule, hResource);
	LPVOID data = LockResource(hGlobal);
	auto str = std::unique_ptr<std::string>(new std::string((char*)data, size));
	FreeResource(hGlobal);
	return move(str);
}




