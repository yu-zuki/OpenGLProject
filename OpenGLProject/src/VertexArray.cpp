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
	
	Bind();	// 頂点配列のバインド

	vb.Bind();	// 頂点バッファのバインド

	const auto& elements = layout.GetElements();
	
	unsigned int offset = 0;		//偏位量の加算（各配列の頭の位置を確認する用）

	for (unsigned int i = 0; i < elements.size(); ++i)
	{
		const auto& element = elements[i];
		// 頂点属性の有効化
		GLCall(glEnableVertexAttribArray(i));

		// 頂点属性の設定 
		// 1:頂点属性のインデックス 
		// 2:頂点属性の要素数 
		// 3:データの型 
		// 4:正規化するかどうか 
		// 5:データの間隔 
		// 6:データの先頭からのオフセット
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
