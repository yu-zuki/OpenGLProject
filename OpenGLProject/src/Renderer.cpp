#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include <iostream>

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

// OpenGL�̃G���[���`�F�b�N����
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
	shader.Bind();	//shader�̃o�C���h

	va.Bind();		//vertex array�̃o�C���h
	ib.Bind();		//index buffer�̃o�C���h

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));	//�`��
}

