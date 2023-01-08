#include "gl/freeglut.h"

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
		KEY_UP = GLUT_KEY_UP,
		KEY_DOWN= GLUT_KEY_DOWN,
		KEY_LEFT=GLUT_KEY_LEFT,
		KEY_RIGHT= GLUT_KEY_RIGHT,
	};
}



