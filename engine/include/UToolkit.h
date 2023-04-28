#include "glm/glm.hpp"


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

namespace glm
{
	class UToolkit
	{
		public:
			LIB_API static glm::vec3 getPositionByMatrix(const glm::mat4& matrix)
			{
				return glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
			}
	};

}
