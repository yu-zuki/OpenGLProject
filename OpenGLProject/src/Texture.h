#pragma once

#include "Renderer.h"
#include <iostream>

class Texture
{
private:
	unsigned int m_RendererID;	  // OpenGLから取得のID
	std::string m_FilePath;       // テクスチャのファイルパス
	unsigned char* m_LocalBuffer; // テクスチャのローカルバッファ
	int m_Width, m_Height, m_BPP; // テクスチャの幅、高さ、BPP(1ピクセルあたりのバイト数)

public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }


};

