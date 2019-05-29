#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace test {
	TestTexture2D::TestTexture2D()
		: m_TranslationA(200, 200, 0)
		, m_TranslationB(400, 200, 0)
		, m_Proj(glm::ortho(0.f, 960.f, 0.f, 540.f, -1.f, 1.f))
		, m_View(glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0)))
	{

		float positions[] = {
			-50.f, -50.f, 0.f, 0.f, // Bottom Left, 0
			 50.f, -50.f, 1.f, 0.f, // Bottom Right, 1
			 50.f,  50.f, 1.f, 1.f, // Top Right, 2
			-50.f,  50.f, 0.f, 1.f  // Top Left, 3
		};

		unsigned int indicies[] = {
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
		m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");

		// Bind the shader
		m_Shader->Bind();

		// Modify a uniform variable in the shader
		m_Shader->SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		m_Texture = std::make_unique<Texture>("res/textures/ChernoLogo.png");
		m_Shader->SetUniform1i("u_Texture", 0);
	}

	TestTexture2D::~TestTexture2D()
	{
	}

	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}

	void TestTexture2D::OnRender()
	{
		GLCall(glClearColor(0.f, 0.f, 0.f, 1.f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		Renderer renderer;

		m_Texture->Bind();

		glm::mat4 proj = glm::ortho(0.f, 960.f, 0.f, 540.f, -1.f, 1.f);
		glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0));

		{
			glm::mat4 model = glm::translate(glm::mat4(1.f), m_TranslationA);
			glm::mat4 mvp = m_Proj * m_View * model; // multiply in reverse order because all the matrice numbers are in column order
												 // Bind our shader
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

		{
			glm::mat4 model = glm::translate(glm::mat4(1.f), m_TranslationB);
			glm::mat4 mvp = m_Proj * m_View * model; // multiply in reverse order because all the matrice numbers are in column order
												 // Bind our shader
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);
		}

	}

	void TestTexture2D::OnImGuiRender()
	{
		ImGui::SliderFloat3("Translation A", &m_TranslationA.x, 0.0f, 960.0f);
		ImGui::SliderFloat3("Translation B", &m_TranslationB.x, 0.0f, 960.0f);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	}
}