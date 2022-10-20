#include "program.h"
#include "engine.h"

namespace neu
{
	Program::~Program()
	{
		if (m_program)
		{
			glDeleteProgram(m_program);
		}
	}

	bool Program::Create(std::string name, ...)
	{
		rapidjson::Document document;
		bool success = neu::json::Load(name, document);
		if (!success)
		{
			LOG("Could not load program file {%s}.", name.c_str());
			return false;
		}

		// create prrogram
		m_program = glCreateProgram();

		// get/addd vertex shader
		std::string vertex_shader;
		READ_DATA(document, vertex_shader);
		if (!vertex_shader.empty())
		{
			auto vshader = g_resources.Get<Shader>(vertex_shader, GL_VERTEX_SHADER);
			AddShader(vshader);
		}

		// get/addd fragment shader
		std::string fragment_shader;
		READ_DATA(document, fragment_shader);
		if (!fragment_shader.empty())
		{
			auto fshader = g_resources.Get<Shader>(fragment_shader, GL_FRAGMENT_SHADER);
			AddShader(fshader);
		}

		Link();

		return true;
	}

	void Program::AddShader(const std::shared_ptr<Shader>& shader)
	{
		m_shaders.push_back(shader);
		glAttachShader(m_program, shader->m_shader);
	}

	void Program::Link()
	{
		// link shader programs
		glLinkProgram(m_program);

		// check program link status
		GLint status;
		glGetProgramiv(m_program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint length = 0;
			glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);
			if (length > 0)
			{
				std::string infolog(length, ' ');
				glGetProgramInfoLog(m_program, length, &length, &infolog[0]);
				LOG("Failed to link program");
				LOG("Program Info: %s", infolog.c_str());
			}
			glDeleteProgram(m_program);
			m_program = 0;
		}
	}

	void Program::Use()
	{
		glUseProgram(m_program);
	}

	void Program::SetUniform(const std::string& name, int value)
	{
		GLint uniform = GetUniform(name);
		if (uniform != -1) { glUniform1i(uniform, value); }
	}

	void Program::SetUniform(const std::string& name, unsigned int value)
	{
		GLint uniform = GetUniform(name);
		if (uniform != -1) { glUniform1ui(uniform, value); }
	}

	void Program::SetUniform(const std::string& name, float value)
	{
		GLint uniform = GetUniform(name);
		if (uniform != -1) { glUniform1f(uniform, value); }
	}

	void Program::SetUniform(const std::string& name, bool value)
	{
		// i dont think that this one is possible
		//GLint uniform = GetUniform(name);
		//if (uniform != -1) { glUniform1b(uniform, value); }
	}

	void Program::SetUniform(const std::string& name, const glm::vec2& value)
	{
		GLint uniform = GetUniform(name);
		if (uniform != -1) { glUniform2fv(uniform, 1, &value[0]); }
	}

	void Program::SetUniform(const std::string& name, const glm::vec3& value)
	{
		GLint uniform = GetUniform(name);
		if (uniform != -1) { glUniform3fv(uniform, 1, &value[0]); }
	}

	void Program::SetUniform(const std::string& name, const glm::vec4& value)
	{
		GLint uniform = GetUniform(name);
		if (uniform != -1) { glUniform4fv(uniform, 1, &value[0]); }
	}

	void Program::SetUniform(const std::string& name, const glm::mat3& value)
	{
		GLint uniform = GetUniform(name);
		if (uniform != -1) { glUniformMatrix3fv(uniform, 1, GL_FALSE, glm::value_ptr(value)); }
	}

	void Program::SetUniform(const std::string& name, const glm::mat4& value)
	{
		GLint uniform = GetUniform(name);
		if (uniform != -1) { glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(value)); }
	}

	GLint Program::GetUniform(const std::string& name)
	{
		// find uniform
		auto uniform = m_uniforms.find(name);
		// if not found get uniform in proram
		if (uniform == m_uniforms.end())
		{
			GLint location = glGetUniformLocation(m_program, name.c_str());
			if (location == -1)
			{
				LOG("Could not find uniform location: %s", name.c_str());
			}
			m_uniforms[name] = location;
		}
		return m_uniforms[name];
	}
}