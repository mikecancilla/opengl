#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

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
            -0.5f, -0.5f,
             0.5f, -0.5f,
             0.5f,  0.5f,
            -0.5f,  0.5f
        };

        unsigned int indicies[] = {
            0, 1, 2,
            2, 3, 0
        };

        // Create and Bind the vertex array
        VertexArray va;

        // Create and Bind the vertex buffer
        VertexBuffer vb(positions, 4 * 2 * sizeof(float));

        // Define the layout of the vertex buffer memory
        VertexBufferLayout layout;
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);

        // Create and Bind the index buffer
        IndexBuffer ib(indicies, 6);

        // Load the shader source
        Shader shader("res/shaders/basic.shader");

        // Bind the shader
        shader.Bind();

        // Modify a uniform variable in the shader
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

        // Clear our state
        va.UnBind();
        vb.UnBind();
        ib.UnBind();
        shader.UnBind();

        float r = 0.f;
        float increment = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            GLCall(glClear(GL_COLOR_BUFFER_BIT));

            // Bind our shader
            shader.Bind();

            // Modify a uniform variable in the shader
            shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

            // Bind our vertex array object
            va.Bind();

            // Bind our index buffer
            ib.Bind();

            // Issue the draw call
            GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

            if(r > 1.f)
                increment = -0.05f;
            else if (r < 0.f)
                increment = 0.05f;

            r += increment;

            /* Swap front and back buffers */
            GLCall(glfwSwapBuffers(window));

            /* Poll for and process events */
            GLCall(glfwPollEvents());
        }
    }

    glfwTerminate();

    return 0;
}