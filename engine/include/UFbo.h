#pragma once
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
	class UFbo
	{
		//////////
	public:
	// Constants:
	static const unsigned int MAX_ATTACHMENTS = 8; ///< Maximum number of available render buffers or textures per FBO	

	// Enumerations:
	enum : unsigned int ///< Kind of operation
	{
		BIND_DEPTHBUFFER = 0,
		BIND_COLORTEXTURE,
		BIND_DEPTHTEXTURE,
	};

	// Const/dest:	 
	UFbo();
	~UFbo();

	// Get/set:   
	unsigned int getTexture(unsigned int textureNumber);
	int getSizeX();
	int getSizeY();
	int getSizeZ();
	unsigned int getHandle();

	// Management:
	bool isOk();
	bool bindTexture(unsigned int textureNumber, unsigned int operation, unsigned int texture, int param1 = 0, int param2 = 0);
	bool bindRenderBuffer(unsigned int renderBuffer, unsigned int operation, int sizeX, int sizeY);

	// Rendering:     
	bool render(void* data = nullptr);
	static void disable();


	private: 

	// Generic data:
	int sizeX, sizeY, sizeZ;	         				///< FBO width, height and depth
	unsigned int texture[MAX_ATTACHMENTS];             ///< Attached textures
	int drawBuffer[MAX_ATTACHMENTS];       		      ///< Set color attachment per texture

	// OGL stuff:
	unsigned int glId;                                 ///< OpenGL ID
	unsigned int glRenderBufferId[MAX_ATTACHMENTS];    ///< Render buffer IDs

	// MRT cache:   
	int nrOfMrts;                                      ///< Number of MRTs
	unsigned int* mrt;                                 ///< Cached list of buffers 

	// Cache:
	bool updateMrtCache();
	};
}