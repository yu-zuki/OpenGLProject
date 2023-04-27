#pragma once

#include <GL/glew.h>
#include "Renderer.h"

class IndexBuffer
{
	private:
		// �o�b�t�@��ID
		unsigned int m_RendererID;
		// �o�b�t�@�̃T�C�Y
		unsigned int m_Count;
	public:
		IndexBuffer(const unsigned int* data, unsigned int count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		//�J�E���g�̎擾
		inline unsigned int GetCount() const { return m_Count; }
};