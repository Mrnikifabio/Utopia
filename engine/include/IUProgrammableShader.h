#pragma once

#include <string>

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
    class IUProgrammableShader
    {
    public:
        IUProgrammableShader() {}
        virtual ~IUProgrammableShader() {}


        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * Loads and compiles a vertex or fragment shader from source code stored in memory.
         * @param subtype subtype of shader (vertex or fragment)
         * @param data pointer to the string containing the source code
         * @return true/false on success/failure
         */
        LIB_API virtual bool loadFromMemory(const std::string& data) = 0;
        

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /**
         * Loads and compiles a vertex, pixel or geometry shader from a specified source text file.
         * @param subtype subtype of shader (pixel, vertex, geometry or program shader)
         * @param filename text file containing the vertex shader code
         * @return true/false on success/failure
         */
        LIB_API virtual bool loadFromFile(const std::string& filename);

    protected:
        bool commonLoad(unsigned int& glId, int glKind, const std::string& data);

    };
}

