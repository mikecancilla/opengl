#pragma once

#include <functional>
#include <vector>

//#include <GLFW/glfw3.h>

struct GLFWwindow;

namespace test {

	class Test
	{
	public:
		Test(GLFWwindow* pWindow);
		virtual ~Test() {}

		virtual void OnUpdate(float deltaTime) {}
		virtual void OnRender() {}
		virtual void OnImGuiRender() {}

        GLFWwindow* m_pWindow;
        int m_windowWidth;
        int m_windowHeight;
    };

	class TestMenu : public Test
	{
	public:
		TestMenu(Test* &currentTestPointer, GLFWwindow* pWindow);
	
		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name, GLFWwindow* pWindow)
		{
			std::cout << "Registering test " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, [pWindow]() { return new T(pWindow); }));
		}

	private:
		Test* &m_CurrentTest;
		std::vector <std::pair<std::string, std::function<Test*()>>> m_Tests;
	};

}