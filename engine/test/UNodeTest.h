#pragma once
#include "Test.h"

namespace utopia_test {
	class UNodeTest : public Test
	{
	private:
		void execute(const std::string& arg) override;
		void modelViewTest();
		void childTest();
		void parentTest();
		void childrenTest();
		void finalWorldCoordinatesTest();

	public:
		UNodeTest(const std::string& name);
	};
}