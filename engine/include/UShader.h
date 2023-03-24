#pragma once

#include "UObject.h"
// GLM:
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// C/C++:
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



namespace utopia
{

	class UShader : UObject
	{

	public:
		// Const/dest:	 
		LIB_API UShader(const std::string& name);
		LIB_API virtual ~UShader() noexcept;

		// Get/set:
		LIB_API int getParamLocation(const std::string& name) const;
		LIB_API void setMatrix4(int param, const glm::mat4& mat);
		LIB_API void setMatrix3(int param, const glm::mat3& mat);
		LIB_API void setFloat(int param, float value);
		LIB_API void setInt(int param, int value);
		LIB_API void setVec3(int param, const glm::vec3& vect);
		LIB_API void setVec4(int param, const glm::vec4& vect);


		void bind(int location, const char* attribName);

		// Rendering:				
		LIB_API virtual void render()=0;
		LIB_API int getID() const;

		void setID(unsigned int id);

	private:
		unsigned int glId;
		static bool uniformJustSaved(const std::string& name);
		static int getUniformByName(const std::string& name);
		static void addUniform(const std::string name, int rID);


		

	};
}
