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

	class UObject
	{
	private:
		static unsigned int m_id_counter;
		struct pimpl;
		std::unique_ptr<pimpl> m_pimpl;
	public:
		LIB_API UObject(const std::string& name);
		
		UObject(const UObject&) = delete;
		UObject(UObject&&) noexcept = delete;
		LIB_API virtual ~UObject() noexcept;
		UObject& operator= (const UObject&) = delete;
		UObject& operator= (UObject&&) = delete;

		LIB_API void setName(const std::string& name);
		LIB_API const std::string& getName() const;
		LIB_API unsigned int getId() const;
		virtual void render() = 0;
	};

}
