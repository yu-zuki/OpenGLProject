#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_Count(count)
{
	//プラットホームチェック　GLuintは4バイト？
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	// バッファの生成
	GLCall( glGenBuffers(1, &m_RendererID) );

	// バインド　バッファの種類　バッファのID
	GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID) );
						 
	// バッファにデータを格納　
	GLCall( glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall( glDeleteBuffers(1, &m_RendererID) );
}

void IndexBuffer::Bind() const
{
	GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID) );
}

void IndexBuffer::Unbind() const
{
	GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0) );
}

