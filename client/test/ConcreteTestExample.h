#include "Test.h"

namespace utopia_test {
	class ConcreteTestExample : public Test
	{
	private:
		void execute(const std::string& arg) override;
	public:
		ConcreteTestExample(const std::string& name);
	};
}