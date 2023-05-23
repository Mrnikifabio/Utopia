#pragma once
#include <gl/freeglut.h>
#include <iostream>

#ifdef _WINDOWS 	
// Export API:
// Specifies i/o linkage (VC++ spec):
#ifdef UTOPIA_EXPORTS
#define LIB_API __declspec(dllexport)
#else
#define LIB_API __declspec(dllimport)
#endif      	
#else // Under Linux
#define LIB_API  // Dummy declaration
#endif

namespace utopia {

	class UResources
	{
	private:
		UResources() {};
		~UResources() {};
		
	public:
		static UResources& getInstance();
		UResources(UResources const&) = delete;
		void operator=(UResources const&) = delete;
		std::string& getStringResource(int resourceId, int resourceType)
	;
	};

}
