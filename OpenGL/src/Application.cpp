#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "Shader.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	glfwSwapInterval(1);//Turns the vsync off

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Print the version of OpenGL	
	std::cout << glGetString(GL_VERSION) << std::endl;
	{
		float positions[] =
		{
			-0.5f, -0.5f,// 0
			 0.5f, -0.5f,// 1
			 0.5f,	0.5f,// 2

			-0.5f,	0.5f,// 3
		};

		unsigned int indices[] =
		{
			0,1,2,
			2,3,0
		};

		VertexArray vao;
		VertexBuffer vbo(positions, 4 * 2 * sizeof(float));	

		VertexBufferLayout vbl;
		vbl.Push<float>(2);
		vao.AddBuffer(vbo, vbl);
		IndexBuffer ibo(indices, 6);

		Shader shader("res/shaders/Basic.shader");

		shader.Unbind();
		vao.Unbind();
		vbo.Unbind();
		ibo.Unbind();


		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			shader.Bind();
			shader.SetUniform4f("u_Color", abs(sin(glfwGetTime())), 0.0f, 0.0f, 1.0f);
			vao.Bind();

			GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, 0));

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	glfwTerminate();
	return 0;
}