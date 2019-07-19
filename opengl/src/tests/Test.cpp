#include "Test.h"
#include "imgui/imgui.h"
#include "GLFW/glfw3.h"

namespace test {
	Test::Test(GLFWwindow* pWindow)
        : m_pWindow(pWindow)
    {
        glfwGetWindowSize(m_pWindow, &m_windowWidth, &m_windowHeight);
    }

    TestMenu::TestMenu(Test* &currentTestPointer, GLFWwindow* pWindow)
		: Test(pWindow)
        , m_CurrentTest(currentTestPointer)
	{
	}

	void TestMenu::OnImGuiRender()
	{
		for (auto &test : m_Tests)
		{
			if (ImGui::Button(test.first.c_str()))
				m_CurrentTest = test.second();
		}
	}
}