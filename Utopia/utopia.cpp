/*****************************************************************//**
 * \file   utopia.cpp
 * \brief  Implementation of utopia.h
 *
 * \author Group 10
 * \date   October 2022
 *********************************************************************/


#include "utopia.h"
#include <iostream>



////////////
// STATIC //
////////////

   // Reserved pointer:
   bool Utopia::initFlag = false;



//////////////
// DLL MAIN //
//////////////

#ifdef _WINDOWS
#include <Windows.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * DLL entry point. Avoid to rely on it for easier code portability (Linux doesn't use this method).
 * @param instDLL handle
 * @param reason reason
 * @param _reserved reserved
 * @return true on success, false on failure
 */
int APIENTRY DllMain(HANDLE instDLL, DWORD reason, LPVOID _reserved)
{
	// Check use:
   switch (reason)
   {
         ///////////////////////////
         case DLL_PROCESS_ATTACH: //
            break;


         ///////////////////////////
         case DLL_PROCESS_DETACH: //
            break;
   }

   // Done:
   return true;
}
#endif



////////////////////////////////
// BODY OF CLASS Utopia //
////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Initialization method. Call this before any other Eureka function.
 * @return true on success, false on error
 */
bool LIB_API Utopia::init()
{
   // Prevent double init:
   if (initFlag)
   {
      std::cout << "ERROR: class already initialized" << std::endl;
      return false;
   }

	// Done:
   initFlag = true;
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Deinitialization method.
 * @return true on success, false on error
 */
bool LIB_API Utopia::free()
{
   // Really necessary?
   if (!initFlag)
   {
      std::cout << "ERROR: class not initialized" << std::endl;
      return false;
   }

	// Done:
   initFlag = false;
	return true;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * Simple, useless method.
 * @return true on success, false on error
 */
bool LIB_API Utopia::doNothing()
{
   // Probably excessive checking:
   if (!initFlag)
   {
      std::cout << "ERROR: class not initialized" << std::endl;
      return false;
   }

   std::cout << "If you read this, your library is working as expected" << std::endl;

   std::cout << "Instantiating a vec3" << std::endl;
   glm::vec3 a(1.0f, 2.0f, 3.0f);
   std::cout << glm::to_string(a) << std::endl;

	// Done:
	return true;
}



