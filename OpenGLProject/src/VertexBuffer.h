#pragma once

#include <GL/glew.h>
#include "Renderer.h"

class VertexBuffer
{
	private:
		// バッファのID
		unsigned int m_RendererID;
	public:
		VertexBuffer(const void* data, unsigned int size);
		~VertexBuffer();

		void Bind() const;
		void Unbind() const;
};