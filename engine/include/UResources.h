#pragma once
#include <gl/freeglut.h>
#include <iostream>

namespace utopia {

	class UResources
	{
	private:
		UResources() {};
		~UResources();
		
	public:
		static UResources& getInstance();
		UResources(UResources const&) = delete;
		void operator=(UResources const&) = delete;
		std::unique_ptr<std::string> getStringResource(int resourceId, int resourceType)
	;
	};

}
