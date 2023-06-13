#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

// OpenGLのエラーをチェックする
bool GLLogCall(const char* funtion, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << funtion << " " << file << ":" << line << std::endl;
		return false;
	}

	return true;
}
 
void Renderer::Clear() const
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));	
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
	shader.Bind();	//shaderのバインド

	va.Bind();		//vertex arrayのバインド
	ib.Bind();		//index bufferのバインド

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));	//描画
}

