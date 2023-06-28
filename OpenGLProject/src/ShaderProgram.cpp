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
	// シェーダーのソースコードを取得
	ShaderProgramSource source = ParseShader(filepath);
	// シェーダーのソースコードを表示
	std::cout << "VERTEX" << std::endl;
	std::cout << source.VertexSource << std::endl;
	std::cout << "FRAGMENT" << std::endl;
	std::cout << source.FragmentSource << std::endl;

	// シェーダーの作成
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
	// プログラムのバインド
	glUseProgram(m_RendererID);
}

Shader::~Shader()
{
	// プログラムの削除
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
	// シェーダーのソースコードの読み込み
	std::ifstream stream(filepath);

	// シェーダーの種類を判別するための列挙型
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))	// 1行ずつ読み込み
	{
		// シェーダーの種類を判別
		if (line.find("#shader") != std::string::npos)
		{
			// シェーダーの種類を表示
			if (line.find("vertex") != std::string::npos)
			{
				// 頂点シェーダー
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				// フラグメントシェーダー
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			// シェーダーのソースコードを追加
			ss[(int)type] << line << '\n';
		}
	}

	// シェーダーのソースコードを返却
	return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	// シェーダーの作成
	unsigned int id = glCreateShader(type);

	// シェーダーのソースコードの設定
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	// シェーダーのコンパイル
	glCompileShader(id);

	// エラーチェック
	int result;

	// シェーダーのコンパイル結果を取得
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		// シェーダーのコンパイル結果の長さを取得
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		// Stack領域でメモリーを確保する
		char* message = (char*)alloca(length * sizeof(char));

		// シェーダーのコンパイル情報
		glGetShaderInfoLog(id, length, &length, message);

		// エラーの表示
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
	// シェーダーの作成
	unsigned int program = glCreateProgram();
	// 頂点シェーダーのコンパイル
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	// フラグメントシェーダーのコンパイル
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//　プログラムにシェーダーをアタッチ
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	// プログラムのリンク
	glLinkProgram(program);

	// エラーチェック
	glValidateProgram(program);

	// シェーダーの中間ファイルを削除
	glDeleteShader(vs);
	glDeleteShader(fs);

	//　プログラムの返却
	return program;
}

int Shader::GetUniformLocation(const std::string& name) const
{
// キャッシュにある場合はキャッシュから取得
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
	{
		return m_UniformLocationCache[name];
	}

	// キャッシュにない場合は取得してキャッシュに追加
	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1)
	{
		std::cout << "Warning: uniform '" << name << "' doesn't exist!" << std::endl;
	}

	// キャッシュに追加
	m_UniformLocationCache[name] = location;

	return location;
}
