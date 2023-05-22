#include "glm/glm.hpp"

namespace glm
{
	inline glm::vec3 getPositionFromMatrix(const glm::mat4& matrix)
	{
		return glm::vec3(matrix[3][0], matrix[3][1], matrix[3][2]);
	}
}
