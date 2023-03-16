#pragma once
#include "UShader.h"
#include "IUProgrammableShader.h"

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
	class UFragmentShader : public UShader, public IUProgrammableShader
	{
	public:
		// Const/dest:	 
		LIB_API UFragmentShader(const std::string& name);
		LIB_API virtual ~UFragmentShader() noexcept;
		LIB_API void render() override;
		LIB_API bool loadFromMemory(const std::string& data) override;
	};
}