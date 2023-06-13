#pragma once

#include "Renderer.h"
#include <iostream>

class Texture
{
private:
	unsigned int m_RendererID;	  // OpenGL����擾��ID
	std::string m_FilePath;       // �e�N�X�`���̃t�@�C���p�X
	unsigned char* m_LocalBuffer; // �e�N�X�`���̃��[�J���o�b�t�@
	int m_Width, m_Height, m_BPP; // �e�N�X�`���̕��A�����ABPP(1�s�N�Z��������̃o�C�g��)

public:
	Texture(const std::string& path);
	~Texture();

	void Bind(unsigned int slot = 0) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }


};

