#include "Texture.h"
#include "vendor/std_image/std_image.h"

Texture::Texture(const std::string& path)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);				//�摜���㉺���]������

	//�摜�����[�h
	//�����F�t�@�C���p�X�A���A�����A�`�����l�����A�iRGBA�j4�`�����l���œǂݍ���
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);	

	GLCall(glGenTextures(1, &m_RendererID));			//�e�N�X�`���𐶐�
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));	//�e�N�X�`�����o�C���h

	//�e�N�X�`���̐ݒ�F�e�N�X�`���t�B���^����`�t�B���^�ɐݒ肷��
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));	//�e�N�X�`���̏k�����@���w��
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));	//�e�N�X�`���̊g����@���w��
	
	//�e�N�X�`���̐ݒ�F�e�N�X�`���̃��b�s���O���@���w�肷��
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));	//�e�N�X�`���̃��b�s���O���@���w��
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));	//�e�N�X�`���̃��b�s���O���@���w��

	//�e�N�X�`���̃f�[�^��OpenGL�ɓn��
	//�����F�e�N�X�`���̎�ށA�~�b�v�}�b�v�̃��x���A�e�N�X�`���̃t�H�[�}�b�g�A���A�����A���0�A�e�N�X�`���̃t�H�[�}�b�g�A�e�N�X�`���̃f�[�^�̌^�A�e�N�X�`���̃f�[�^
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

	//�e�N�X�`���̃f�[�^�����
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);

}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));			//�e�N�X�`�����폜
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));			//�e�N�X�`���X���b�g���w��
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));		//�e�N�X�`�����o�C���h
}

void Texture::Unbind() const
{
}
