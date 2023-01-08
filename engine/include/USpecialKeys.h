#include "Utopia.h"

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


namespace utopia
{
	LIB_API enum class USpecialKeys : int
	{
		KEY_UP = 101,
		KEY_DOWN= 103,
		KEY_LEFT=100,
		KEY_RIGHT= 102
	};
}



