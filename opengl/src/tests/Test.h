#pragma once

#include <functional>
#include <vector>

namespace test {

	class Test
	{
	public:
		Test(int windowWidth = 960, int windowHeight = 540)
        : m_windowWidth(windowWidth)
        , m_windowHeight(windowHeight)
        {}

		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

        int m_windowWidth;
        int m_windowHeight;
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test* &currentTestPointer);
	
		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name, unsigned int windowWidth, unsigned int windowHeight)
		{
			std::cout << "Registering test " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, [windowWidth, windowHeight]() { return new T(windowWidth, windowHeight); }));
		}

	private:
		Test* &m_CurrentTest;
		std::vector <std::pair<std::string, std::function<Test*()>>> m_Tests;
	};
}