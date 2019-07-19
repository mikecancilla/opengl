#include "TestTexture2DSimple.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	TestTexture2DSimple::TestTexture2DSimple(int windowWidth, int windowHeight)
		: Test(windowWidth, windowHeight)
		, m_Proj(glm::ortho(-1.f, 1.f, -1.f, 1.f, -1.f, 1.f))
		, m_View(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)))
	{
        /*
        float positions[] = {
			-1.f, -1.f, // Bottom Left, 0
			 1.f, -1.f, // Bottom Right, 1
			 1.f,  1.f, // Top Right, 2
			-1.f,  1.f  // Top Left, 3
		};
        */
        float positions[] = {
			-50.f, -50.f, 0.f, 0.f, // Bottom Left, 0
			 50.f, -50.f, 1.f, 0.f, // Bottom Right, 1
			 50.f,  50.f, 1.f, 1.f, // Top Right, 2
			-50.f,  50.f, 0.f, 1.f  // Top Left, 3
		};

		unsigned short indicies[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// Create and Bind the vertex array
		m_VAO = std::make_unique<VertexArray>();

		// Create and Bind the vertex buffer
		m_VertexBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));

		// Define the layout of the vertex buffer memory
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);

		m_VAO->AddBuffer(*m_VertexBuffer, layout);

		// Create and Bind the index buffer
		m_IndexBuffer = std::make_unique<IndexBuffer>(indicies, 6);

		// Create the shader
		m_Shader = std::make_unique<Shader>("res/shaders/BasicSimple.shader");

		// Bind the shader
		m_Shader->Bind();

		//m_Texture = std::make_unique<Texture>("res/textures/ChernoLogo.png");
		m_Texture = std::make_unique<Texture>("D:\\src\\mike\\mp2ts_analyzer\\1930_ford_model_a.jpg");

		m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2DSimple::~TestTexture2DSimple()
	{
	}

	void TestTexture2DSimple::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2DSimple::OnRender()
	{
		GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Texture->Bind();

		{
			glm::mat4 model = glm::mat4(1.f);
			glm::mat4 mvp = m_Proj * m_View * model; // multiply in reverse order because all the matrice numbers are in column order
												 // Bind our shader
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}
	}

	void TestTexture2DSimple::OnImGuiRender()
	{
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}