/*****************************************************************//**
 * \file   Utopia.h
 * \brief  Basic implementation of the gr. eng. lib.
 * 
 * \author Group10
 * \date   October 2022
 *********************************************************************/

#pragma once

/////////////
// VERSION //
/////////////

   // Generic info:
#define LIB_NAME      "Utopia"  ///
#define LIB_VERSION   10                              ///< Library version (divide by 10)

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


#include "glm/glm.hpp"
#include "glm/gtx/string_cast.hpp"

/////////////
// CLASSES //
/////////////

/**
 * @brief Simple static class example.
 */
class LIB_API Utopia
{
	//////////
public: //
//////////	   

   // Init/free:
	static bool init();
	static bool free();

	// Event manager:
	static bool doNothing();


	///////////
private:	//
///////////	   

	// Const/dest (as private to prevent instanciation):
	Utopia() {}
	~Utopia() {}

	// Internal vars:
	static bool initFlag;
};