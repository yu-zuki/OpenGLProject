#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "ShaderProgram.h"
#include "Renderer.h"

Shader::Shader(const std::string& filepath)
	: m_FilePath(filepath), m_RendererID(0)
{
	// �V�F�[�_�[�̃\�[�X�R�[�h���擾
	ShaderProgramSource source = ParseShader(filepath);
	// �V�F�[�_�[�̃\�[�X�R�[�h��\��
	std::cout << "VERTEX" << std::endl;
	std::cout << source.VertexSource << std::endl;
	std::cout << "FRAGMENT" << std::endl;
	std::cout << source.FragmentSource << std::endl;

	// �V�F�[�_�[�̍쐬
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
	// �v���O�����̃o�C���h
	glUseProgram(m_RendererID);
}

Shader::~Shader()
{
	// �v���O�����̍폜
	glDeleteProgram(m_RendererID);
}

void Shader::Bind() const
{
	GLCall(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
	GLCall(glUseProgram(0));
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

void Shader::SetUniformMat4f(const std::string& name, const glm::mat4& matrix)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]));	//
}

ShaderProgramSource Shader::ParseShader(const std::string& filepath)
{
	// �V�F�[�_�[�̃\�[�X�R�[�h�̓ǂݍ���
	std::ifstream stream(filepath);

	// �V�F�[�_�[�̎�ނ𔻕ʂ��邽�߂̗񋓌^
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))	// 1�s���ǂݍ���
	{
		// �V�F�[�_�[�̎�ނ𔻕�
		if (line.find("#shader") != std::string::npos)
		{
			// �V�F�[�_�[�̎�ނ�\��
			if (line.find("vertex") != std::string::npos)
			{
				// ���_�V�F�[�_�[
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				// �t���O�����g�V�F�[�_�[
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			// �V�F�[�_�[�̃\�[�X�R�[�h��ǉ�
			ss[(int)type] << line << '\n';
		}
	}

	// �V�F�[�_�[�̃\�[�X�R�[�h��ԋp
	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	// �V�F�[�_�[�̍쐬
	unsigned int id = glCreateShader(type);

	// �V�F�[�_�[�̃\�[�X�R�[�h�̐ݒ�
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	// �V�F�[�_�[�̃R���p�C��
	glCompileShader(id);

	// �G���[�`�F�b�N
	int result;

	// �V�F�[�_�[�̃R���p�C�����ʂ��擾
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		// �V�F�[�_�[�̃R���p�C�����ʂ̒������擾
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		// Stack�̈�Ń������[���m�ۂ���
		char* message = (char*)alloca(length * sizeof(char));

		// �V�F�[�_�[�̃R���p�C�����
		glGetShaderInfoLog(id, length, &length, message);

		// �G���[�̕\��
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader!" << std::endl;

		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// �V�F�[�_�[�̍쐬
	unsigned int program = glCreateProgram();
	// ���_�V�F�[�_�[�̃R���p�C��
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	// �t���O�����g�V�F�[�_�[�̃R���p�C��
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//�@�v���O�����ɃV�F�[�_�[���A�^�b�`
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	// �v���O�����̃����N
	glLinkProgram(program);

	// �G���[�`�F�b�N
	glValidateProgram(program);

	// �V�F�[�_�[�̒��ԃt�@�C�����폜
	glDeleteShader(vs);
	glDeleteShader(fs);

	//�@�v���O�����̕ԋp
	return program;
}

int Shader::GetUniformLocation(const std::string& name) const
{
// �L���b�V���ɂ���ꍇ�̓L���b�V������擾
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[name];
	}

	// �L���b�V���ɂȂ��ꍇ�͎擾���ăL���b�V���ɒǉ�
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
	{
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	}

	// �L���b�V���ɒǉ�
	m_UniformLocationCache[name] = location;

	return location;
}
