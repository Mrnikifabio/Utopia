#pragma once
#include "Test.h"

namespace utopia_test {
	class UNodeTest : public Test
	{
	private:
		void execute() override;
	public:
		UNodeTest(const std::string& name);
	};
}