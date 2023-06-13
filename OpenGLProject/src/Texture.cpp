#include "Texture.h"
#include "vendor/std_image/std_image.h"

Texture::Texture(const std::string& path)
	: m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
	stbi_set_flip_vertically_on_load(1);				//画像を上下反転させる

	//画像をロード
	//引数：ファイルパス、幅、高さ、チャンネル数、（RGBA）4チャンネルで読み込む
	m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);	

	GLCall(glGenTextures(1, &m_RendererID));			//テクスチャを生成
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));	//テクスチャをバインド

	//テクスチャの設定：テクスチャフィルタを線形フィルタに設定する
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));	//テクスチャの縮小方法を指定
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));	//テクスチャの拡大方法を指定
	
	//テクスチャの設定：テクスチャのラッピング方法を指定する
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));	//テクスチャのラッピング方法を指定
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));	//テクスチャのラッピング方法を指定

	//テクスチャのデータをOpenGLに渡す
	//引数：テクスチャの種類、ミップマップのレベル、テクスチャのフォーマット、幅、高さ、常に0、テクスチャのフォーマット、テクスチャのデータの型、テクスチャのデータ
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));

	//テクスチャのデータを解放
	if (m_LocalBuffer)
		stbi_image_free(m_LocalBuffer);

}

Texture::~Texture()
{
	GLCall(glDeleteTextures(1, &m_RendererID));			//テクスチャを削除
}

void Texture::Bind(unsigned int slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));			//テクスチャスロットを指定
	GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));		//テクスチャをバインド
}

void Texture::Unbind() const
{
}
