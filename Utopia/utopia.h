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




/**
 * @brief Simple static class example.
 */
class LIB_API Utopia
{

public:

	static Utopia& getInstance()
	{
		static Utopia m_instance; // Guaranteed to be destroyed.
		// Instantiated on first use.
		return m_instance;
	}

	Utopia(Utopia const&) = delete;
	void operator=(Utopia const&) = delete;


	bool init();
	void clear();
	void free();

	void enableDepth();
	void enableCullFace();
	void enableLight0();
	void enableLighting();
	void enableWireFrameMode();
	void enableSolidMode();

	void setKeyboardCallback(void (*callback)(unsigned char, int, int));
	void setDisplayCallback(void (*callback)(void));
	void setReshapeCallback(void (*callback)(int, int));
	void setSpecialCallback(void (*callback)(int, int, int)); 
	void setCloseCallback(void(*callback)(void)); //invoked when the window is closed

	void display();
	void mainLoop();

	bool isRunning();

	void swap();

private:
	// Const/dest (as private to prevent instanciation):
	Utopia() : m_initFlag{ false } {};
	~Utopia() {}
	bool m_initFlag;

};
