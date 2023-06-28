#include "TestTexture2D.h"
#include "Renderer.h"

#include "imgui/imgui.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"

test::TestTexture2D::TestTexture2D()
{
	float positions[] = {
		 0.0f,   0.0f,   0.0f, 0.0f,	// 0
		 100.0f, 0.0f,   1.0f, 0.0f,	// 1
		 100.0f, 100.0f, 1.0f, 1.0f,    // 2
		 0.0f,   100.0f, 0.0f, 1.0f		// 3
	};


	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	VertexArray va;										//頂点配列	インスタンス
	VertexBuffer vb(positions, 4 * 4 * sizeof(float));	//頂点バッファ	インスタンス

	VertexBufferLayout layout;							//頂点バッファレイアウト	インスタンス

	layout.Push<float>(2);	//頂点バッファレイアウトの設定
	layout.Push<float>(2);	//頂点バッファレイアウトの設定

	va.AddBuffer(vb, layout);

	//-----------------------------------------------------------------------------------------------------------

	// インデックスバッファの生成
	IndexBuffer ib(indices, 6);

	glm::mat4 proj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);	//正投影行列の生成
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0, 0));	//ビュー行列の生成

	Shader shader("res/shaders/Basic.shader");		//シェーダープログラム	インスタンス
	shader.Bind();									//シェーダープログラムのバインド

	//stop
	Texture texture("res/textures/OpenGL.png");				//テクスチャー	インスタンス
	const int  texturesSlot_0 = 0;

	texture.Bind(texturesSlot_0);
	shader.SetUniform1i("u_Texture", texturesSlot_0);		//シェーダープログラムのテクスチャー変数の設定 : 0番目のテクスチャースロットを使用する。

	va.Unbind();
	vb.Unbind();
	ib.Unbind();
	shader.Unbind();
}

test::TestTexture2D::~TestTexture2D()
{ 
}

void test::TestTexture2D::OnUpdate(float deltaTime)
{
}

void test::TestTexture2D::OnRender()
{
	GLCall(glClearColor( 0.0f, 0.0f, 0.0f, 1.0f ));
	GLCall(glClear(GL_COLOR_BUFFER_BIT));			// GL_COLOR_BUFFER_BIT: カラーバッファをクリアする
}

void test::TestTexture2D::OnImGuiRender()
{

}
