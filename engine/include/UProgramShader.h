#pragma once
#include "UShader.h"
#include "UVertexShader.h"
#include "UFragmentShader.h"

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
	class UProgramShader : public UShader
	{
	private:
		bool built;
	public:
		// Const/dest:	 
		LIB_API UProgramShader(const std::string& name);
		LIB_API virtual ~UProgramShader() noexcept;
		LIB_API void render() override;
		LIB_API bool build(const UVertexShader& vertexShader, const UFragmentShader& fragmentShader);
		LIB_API bool isBuilt();
	};
}