#pragma once
#include <string>
#include <functional>

namespace utopia_test {

	class Test
	{
	private:
		std::string m_name;
		std::function<void()> m_function;
	public:
		Test(const std::string& name);

		Test(const Test&) = delete;
		Test(Test&&) noexcept = delete;
		Test& operator= (const Test&) = delete;
		Test& operator= (Test&&) = delete;

		const std::string& getName() const;
		virtual void execute(const std::string& arg) = 0;
	};
}
