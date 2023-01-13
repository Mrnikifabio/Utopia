#include "Test.h"

namespace utopia_test {
	class BoxTest : public Test
	{
	private:
		void execute(const std::string& arg) override;
		void setAndGetHookPointNode();
		void setAndGetGroundNode();
		void setAndGetTopNode();
	public:
		BoxTest(const std::string& name);
	};
}