#pragma once
#include "UNode.h"
#include <unordered_map>
#include <memory>

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
	class OVONodeAbstractStrategy;
	class OVOFactory
	{
		struct pimpl;
	private:
		std::unique_ptr<pimpl> m_pimpl;
		
		LIB_API OVOFactory();
		LIB_API virtual ~OVOFactory() noexcept;
		
		void loadStrategies();
		std::unique_ptr<UNode> recursiveFetch(std::ifstream& inFile);

		friend OVONodeAbstractStrategy;

	public:
		LIB_API static OVOFactory& getInstance()
		{
			static OVOFactory m_instance;
			return m_instance;
		}

		OVOFactory(OVOFactory const&) = delete;
		void operator=(OVOFactory const&) = delete;

		LIB_API std::shared_ptr<UNode> fromFile(const std::string& path);
	};
}

