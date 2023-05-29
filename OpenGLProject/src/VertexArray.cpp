#include "VertexArray.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	
	Bind();	// ���_�z��̃o�C���h

	vb.Bind();	// ���_�o�b�t�@�̃o�C���h

	const auto& elements = layout.GetElements();
	
	unsigned int offset = 0;		//�Έʗʂ̉��Z�i�e�z��̓��̈ʒu���m�F����p�j

	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		// ���_�����̗L����
		GLCall(glEnableVertexAttribArray(i));

		// ���_�����̐ݒ� 
		// 1:���_�����̃C���f�b�N�X 
		// 2:���_�����̗v�f�� 
		// 3:�f�[�^�̌^ 
		// 4:���K�����邩�ǂ��� 
		// 5:�f�[�^�̊Ԋu 
		// 6:�f�[�^�̐擪����̃I�t�Z�b�g
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}

}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}
