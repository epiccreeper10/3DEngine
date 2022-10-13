#include "Engine.h" 
#include <iostream> 

float points[] = {
-0.5f,  -0.5f,  0.0f,
   0.0f,  0.0f,  0.0f,
	0.0f, -0.5f,  0.0f,

	-0.5f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f,
	-0.5f, -0.5f, 0.0f
};

glm::vec3 colors[] = {
	{1,0,0},
	{0,1,0},
	{0,0,1},
	{1,1,0},
	{1,1,1},
	{1,0,1},
	{0,1,1}
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
"  color = vec4(0.4, 0.2, 1.0, 1.0);"
"}";

int main(int argc, char** argv)
{
	std::cout << "1 : Initialising Memory" << "\n";
	neu::InitializeMemory();
	neu::SetFilePath("../Assets");
	std::cout << "1 : Memory Initialized" << "\n\n";
	std::cout << "2 : Creating Instance" << "\n";
	neu::Engine::Instance().Initialize();
	neu::Engine::Instance().Register();
	std::cout << "2 : Instance Created" << "\n\n";

	neu::g_renderer.CreateWindow("Neumont", 800, 600);
	std::cout << "3 : Program Started" << "\n";

	// create vertex buffer
	GLuint pvbo = 0;
	glGenBuffers(1, &pvbo);
	glBindBuffer(GL_ARRAY_BUFFER, pvbo);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), points, GL_STATIC_DRAW);

	GLuint cvbo = 0;
	glGenBuffers(1, &cvbo);
	glBindBuffer(GL_ARRAY_BUFFER, cvbo);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(glm::vec3), colors, GL_STATIC_DRAW);

	// create vertex array
	GLuint vao = 0;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, pvbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, cvbo);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	// create shader
	std::shared_ptr<neu::Shader> vs = neu::g_resources.Get<neu::Shader>("shaders/basic.vert", GL_VERTEX_SHADER);
	std::shared_ptr<neu::Shader> fs = neu::g_resources.Get<neu::Shader>("shaders/basic.frag", GL_FRAGMENT_SHADER);

	//GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	//glShaderSource(vs, 1, &vertex_shader, NULL);
	//glCompileShader(vs);
	//GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	//glShaderSource(fs, 1, &fragment_shader, NULL);
	//glCompileShader(fs);

	// create program
	GLuint program = glCreateProgram();
	glAttachShader(program, fs->m_shader);
	glAttachShader(program, vs->m_shader);
	glLinkProgram(program);
	glUseProgram(program);

	GLint uniform1 = glGetUniformLocation(program, "scale");
	GLint uniform2 = glGetUniformLocation(program, "tint");
	GLint uniform3 = glGetUniformLocation(program, "transform");

	glUniform3f(uniform2, 1, 0, 0);

	glm::mat4 mx;
	glUniformMatrix4fv(uniform3, 1, GL_FALSE, glm::value_ptr(mx));


	bool quit = false;
	while (!quit)
	{
		glUniform1f(uniform1, std::sin(neu::g_time.time * 2000));

		neu::Engine::Instance().Update();

		if (neu::g_inputSystem.GetKeyState(neu::key_escape) == neu::InputSystem::KeyState::Pressed) quit = true;

		

		neu::g_renderer.BeginFrame();
		glDrawArrays(GL_TRIANGLES, 0, 6);
		
		neu::g_renderer.EndFrame();
	}
	std::cout << "3 : Program Ended" << "\n\n";
	neu::Engine::Instance().Shutdown();

	return 0;
}