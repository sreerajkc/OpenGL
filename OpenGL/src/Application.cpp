#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <string>
#include <sstream>
#include "Renderer.h"

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(std::string filePath)
{
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::ifstream stream(filePath);
	std::string line;
	std::stringstream ss[2];

	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(),ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)alloca(length * sizeof(char));// allocating memory on stack using alloca instead of creating heap allocation

		GLCall(glGetShaderInfoLog(id, length, &length, message));

		std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;

		GLCall(glDeleteShader(id));
	}

	return id;
}

// creates and compile shaders we provide
static unsigned int CreateShaderProgram(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));
	
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}

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

		ShaderProgramSource source = ParseShader("res/shaders/Basic.shader"); // path will be relative to the project working directory(Project > Properties > Debugging > Working Directory)
		unsigned int shaderProgram = CreateShaderProgram(source.VertexSource, source.FragmentSource);

		GLCall(glUseProgram(0));
		vao.Unbind();
		vbo.Unbind();
		ibo.Unbind();


		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			glClear(GL_COLOR_BUFFER_BIT);

			GLCall(glUseProgram(shaderProgram));

			GLCall(int location = glGetUniformLocation(shaderProgram, "u_Color"));
			ASSERT(location != -1);
			GLCall(glUniform4f(location, abs(sin(glfwGetTime())), 0.0f, 0.0f, 1.0f));

			vao.Bind();
			GLCall(glDrawElements(GL_TRIANGLES, ibo.GetCount(), GL_UNSIGNED_INT, 0));

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

		GLCall(glDeleteProgram(shaderProgram));
	}

	glfwTerminate();
	return 0;
}