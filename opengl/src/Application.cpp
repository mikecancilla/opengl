#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

// GLM uses column major order, which is what OpenGL expects
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

///////
// MAIN
///////

int main(void)
{
    GLFWwindow* window;
    unsigned int err = GLFW_NO_ERROR;

    /* Initialize the library */
    if(!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if(glewInit() != GLEW_OK)
        std::cout << "Glew Initialization Error!" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl;

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
//        GLCall(glBlendFunc(GL_DST_ALPHA, GL_ONE_MINUS_DST_ALPHA));
//        GLCall(glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR));
//        GLCall(glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR));

        // Create and Bind the vertex array
        VertexArray va;

        // Create and Bind the vertex buffer
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        // Define the layout of the vertex buffer memory
        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        // Create and Bind the index buffer
        IndexBuffer ib(indicies, 6);

		// Create an orthographic projection matrix
		glm::mat4 proj = glm::ortho(0.f, 960.f, 0.f, 540.f, -1.f, 1.f);

		// Create the view and model
		glm::mat4 view = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, 0));
//		glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(200, 200, 0));

//		glm::mat4 mvp = proj * view * model; // multiply in reverse order because all the matrice numbers are in column order

        // Load the shader source
        Shader shader("res/shaders/basic.shader");

        // Bind the shader
        shader.Bind();

        // Modify a uniform variable in the shader
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
//		shader.SetUniformMat4f("u_MVP", mvp);

        Texture texture("res/textures/ChernoLogo.png");
        texture.Bind();
        shader.SetUniform1i("u_Texture", 0);

        // Clear our state
        va.UnBind();
        vb.UnBind();
        ib.UnBind();
        shader.UnBind();

        Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfwGL3_Init(window, true);
		ImGui::StyleColorsDark();

		glm::vec3 translationA(200, 200, 0);
		glm::vec3 translationB(400, 200, 0);

		float r = 0.f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

			ImGui_ImplGlfwGL3_NewFrame();

			{
				glm::mat4 model = glm::translate(glm::mat4(1.f), translationA);
				glm::mat4 mvp = proj * view * model; // multiply in reverse order because all the matrice numbers are in column order
    			 // Bind our shader
				shader.Bind();
				shader.SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(va, ib, shader);
			}

            // Modify a uniform variable in the shader
            //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			{
				glm::mat4 model = glm::translate(glm::mat4(1.f), translationB);
				glm::mat4 mvp = proj * view * model; // multiply in reverse order because all the matrice numbers are in column order
				// Bind our shader
				shader.Bind();
				shader.SetUniformMat4f("u_MVP", mvp);
				renderer.Draw(va, ib, shader);
			}

            // Issue the draw call
            //GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if(r > 1.f)
                increment = -0.05f;
            else if (r < 0.f)
                increment = 0.05f;

            r += increment;

			// 1. Show a simple window.
			// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets automatically appears in a window called "Debug".
			{
				ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
				ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			ImGui::Render();
			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }

	ImGui_ImplGlfwGL3_Shutdown();
	ImGui::DestroyContext();

    glfwTerminate();

    return 0;
}