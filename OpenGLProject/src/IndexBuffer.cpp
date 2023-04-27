#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_Count(count)
{
	//�v���b�g�z�[���`�F�b�N�@GLuint��4�o�C�g�H
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	// �o�b�t�@�̐���
	GLCall( glGenBuffers(1, &m_RendererID) );

	// �o�C���h�@�o�b�t�@�̎�ށ@�o�b�t�@��ID
	GLCall( glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID) );
						 
	// �o�b�t�@�Ƀf�[�^���i�[�@
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

