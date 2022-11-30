#pragma once
#include <string>
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

	class LIB_API UObject
	{
	private:
		static unsigned int m_id_counter;
		struct pimpl;
		std::unique_ptr<pimpl> m_pimpl;
	public:
		UObject(const std::string& name);
		UObject(const UObject&) = delete;
		UObject(UObject&&) noexcept = delete;
		virtual ~UObject() noexcept;
		UObject& operator= (const UObject&) = delete;
		UObject& operator= (UObject&&) = delete;

		const std::string& getName() const;
		unsigned int getId() const;
		virtual void render() = 0;
	};

}
