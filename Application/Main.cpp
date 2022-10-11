#include "Engine.h" 
#include <iostream> 

float points[] = {
  -0.5f, -0.5f,  0.0f,
   0.0f,  0.5f,  0.0f,
   0.5f, -0.5f,  0.0f
};
const char* vertex_shader =
"#version 430 core\n"
"in vec3 position;"
"void main() {"
"  gl_Position = vec4(position, 1.0);"
"}";



const char* fragment_shader =
"#version 430 core\n"
"out vec4 color;"
"void main() {"
"  color = vec4(1.0, 1.0, 1.0, 1.0);"
"}";

int main(int argc, char** argv)
{
	std::cout << "1 : Initialising Memory" << "\n";
	neu::InitializeMemory();
	std::cout << "1 : Memory Initialized" << "\n\n";
	std::cout << "2 : Creating Instance" << "\n";
	neu::Engine::Instance().Initialize();
	neu::Engine::Instance().Register();
	std::cout << "2 : Instance Created" << "\n\n";

	neu::g_renderer.CreateWindow("Neumont", 800, 600);
	std::cout << "3 : Program Started" << "\n";

	// create vertex buffer
	GLuint vbo = 0;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

	// create vertex array
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// create shader
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vertex_shader, NULL);
	glCompileShader(vs);
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fragment_shader, NULL);
	glCompileShader(fs);

	// create program
	GLuint program = glCreateProgram();
	glAttachShader(program, fs);
	glAttachShader(program, vs);
	glLinkProgram(program);
	glUseProgram(program);

	bool quit = false;
	while (!quit)
	{
		neu::Engine::Instance().Update();

		if (neu::g_inputSystem.GetKeyState(neu::key_escape) == neu::InputSystem::KeyState::Pressed) quit = true;

		

		neu::g_renderer.BeginFrame();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		neu::g_renderer.EndFrame();
	}
	std::cout << "3 : Program Ended" << "\n\n";
	neu::Engine::Instance().Shutdown();

	return 0;
}