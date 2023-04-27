#include "VertexBuffer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
	// バッファの生成
	GLCall( glGenBuffers(1, &m_RendererID) );

	// バインド　バッファの種類　バッファのID
	GLCall( glBindBuffer(GL_ARRAY_BUFFER, m_RendererID) );

	// バッファにデータを格納　
	GLCall( glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW) );
}

VertexBuffer::~VertexBuffer()
{
	GLCall( glDeleteBuffers(1, &m_RendererID) );
}

void VertexBuffer::Bind() const
{
	GLCall( glBindBuffer(GL_ARRAY_BUFFER, m_RendererID) );
}

void VertexBuffer::Unbind() const
{
	GLCall( glBindBuffer(GL_ARRAY_BUFFER, 0) );
}
