#include "TestTexture2D.h"
#include "Renderer.h"

#include "imgui/imgui.h"
#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"
#include <glm/gtc/type_ptr.hpp>

#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Texture.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"

test::TestTexture2D::TestTexture2D()
	:m_TranslationA(1.0f), m_TranslationB(0.f), m_ScaleB(1.0f)
{
	m_Proj = glm::ortho(0.0f, 1920.0f, 0.0f, 1080.0f, -1.0f, 1.0f);			//正投影行列の生成
	m_View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0, 0));		//ビュー行列の生成

	float positions[] = {				//頂点座標
		-500.0f,	-500.0f,    0.0f, 0.0f,		// 0  左下
		 500.0f,	-500.0f,    1.0f, 0.0f,		// 1　右下
		 500.0f,	 500.0f,	1.0f, 1.0f,		// 2　右上
	    -500.0f,	 500.0f,	0.0f, 1.0f		// 3　左上
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0,
	};

	m_VAO = std::make_unique<VertexArray>();									//頂点配列	インスタンス
	m_VBO = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));	//頂点バッファ	インスタンス
	VertexBufferLayout layout;							//頂点バッファレイアウト	インスタンス
	layout.Push<float>(2);								//頂点バッファレイアウトの設定
	layout.Push<float>(2);								//頂点バッファレイアウトの設定
	m_VAO->AddBuffer(*m_VBO, layout);						//頂点配列に頂点バッファと頂点バッファレイアウトを追加
	m_IndexBuffer = std::make_unique<IndexBuffer>(indices, 6);	// インデックスバッファの生成;
	

	
	m_Shader = std::make_unique<Shader>("res/shaders/Basic.shader");		//シェーダープログラム	インスタンス
	m_Shader->Bind();														//シェーダープログラムのバインド
	
	m_Texture = std::make_unique<Texture>("res/textures/OpenGL.png");		//テクスチャー	インスタンス
	const int  texturesSlot_0 = 0;
	m_Texture->Bind(texturesSlot_0);
	m_Shader->SetUniform1i("u_Texture", texturesSlot_0);					//シェーダープログラムのテクスチャー変数の設定 : 0番目のテクスチャースロットを使用する。


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

	Renderer renderer;

	m_Texture->Bind();
	{
		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationA);				//モデル行列の生成
		glm::mat4 mvp = m_Proj * m_View * model;										//モデルビュー投影行列の生成
		m_Shader->Bind();																//シェーダープログラムのバインド
		m_Shader->SetUniformMat4f("u_ModelViewProjection", mvp);						//シェーダープログラムのモデルビュー投影行列変数の設定
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);								//描画
	}
	{

		glm::mat4 model = glm::translate(glm::mat4(1.0f), m_TranslationB);				//モデル行列の生成
		model = glm::scale(model, m_ScaleB);											//モデル行列の拡大縮小

		glm::mat4 mvp = m_Proj * m_View * model ;										//モデルビュー投影行列の生成
		m_Shader->Bind();																//シェーダープログラムのバインド
		m_Shader->SetUniformMat4f("u_ModelViewProjection", mvp);										//シェーダープログラムのモデルビュー投影行列変数の設定
		renderer.Draw(*m_VAO, *m_IndexBuffer, *m_Shader);								//描画
	}
}

void test::TestTexture2D::OnImGuiRender()
{
	//ImGui::PushItemWidth(500);	//幅の設定
	//ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 10)); // 横の設定
	//ImGui::PushItemHight(200);	//幅の設定

	ImGui::SliderFloat3("Translation_A", &m_TranslationA.x, 0.0f, 1080.0f);
	ImGui::SliderFloat3("Translation_B", &m_TranslationB.x, 0.0f, 1080.0f);
	ImGui::SliderFloat3("Scale_B", &m_ScaleB.x, 0.2f, 4.0f);

	//ImGui::PopStyleVar();
	//ImGui::PopItemWidth();		//幅の設定

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
}
