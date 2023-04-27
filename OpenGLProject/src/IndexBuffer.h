#pragma once

#include <GL/glew.h>
#include "Renderer.h"

class IndexBuffer
{
	private:
		// バッファのID
		unsigned int m_RendererID;
		// バッファのサイズ
		unsigned int m_Count;
	public:
		IndexBuffer(const unsigned int* data, unsigned int count);
		~IndexBuffer();

		void Bind() const;
		void Unbind() const;

		//カウントの取得
		inline unsigned int GetCount() const { return m_Count; }
};