// OpenGLプログラム
// 作成日：2023/4/13
// 作成者：TOU
// 
// 更新日：2023/4/13　　(Core-profile)OpenGLを使用したプログラムを追加した。
// 更新日：2023/4/14	shaderのソースコードを追加しました。
// 更新日：2023/4/15	シェーダーのコンパイルを追加しました。
// 更新日：2023/4/18	VERTEX Buffersを追加しました。
// 更新日：2023/4/20	indices Buffersを追加しました。
// 更新日：2023/4/20	OpenGLのError Check機能を追加しました。glGetError()を使っての基本的なBugCheck。
// 更新日：2023/4/20	Uniformsを追加しました。
// 更新日：2023/4/21	Vertex arraysを追加しました。
// 更新日：2023/4/27	オブジェクト指向でコードを再構成(IndexBuffer VertexBuffer)
// 更新日：2023/5/09	オブジェクト指向でコードを再構成(VertexArray)
// 更新日：2023/5/29	オブジェクト指向でコードを再構成(VertexArray)
// 更新日：2023/6/9		オブジェクト指向でコードを再構成(Render) 
// 更新日：2023/6/9		画像をGPUに読み込む機能を追加しました。　stb_image.hを使用しています。
//						(stb_image.h　使用方法：画像のパスを渡して、RGBAのpixels bufferのpointerがReturnされる） 
// 更新日：2023/6/14	Textureクラスを追加しました。
//						機能：画像をGPUに読み込む機能を追加しました。
// 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <windows.h>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include "StaticFunction.h"
#include "ShaderProgram.h"
#include "Texture.h"

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(1080, 1080, "Hello World", NULL, NULL);
	if (!window)
	{
		// ウィンドウの作成に失敗
		glfwTerminate();
		return -1;
	}

	// ウィンドウのコンテキストを作成
	glfwMakeContextCurrent(window); 	

	// V-syncの設定
	glfwSwapInterval(1);

	// GLEWの初期化
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error" << std::endl;
		return -1;
	}

	// OpenGLのバージョンを表示
	std::cout << glGetString(GL_VERSION) << std::endl;

	{
		float positions[] = {
			-0.5f, -0.5f, 0.0f, 0.0f,	//0
			 0.5f, -0.5f, 1.0f, 0.0f,	//1
			 0.5f,  0.5f, 1.0f, 1.0f,	//2
			-0.5f,  0.5f, 0.0f, 1.0f	//3
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));	//ブレンドの設定 : 透過の設定 
		GLCall(glEnable(GL_BLEND));									//ブレンドの設定


		VertexArray va;										//頂点配列	インスタンス
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));	//頂点バッファ	インスタンス

		VertexBufferLayout layout;							//頂点バッファレイアウト	インスタンス

		layout.Push<float>(2);	//頂点バッファレイアウトの設定
		layout.Push<float>(2);	//頂点バッファレイアウトの設定

		va.AddBuffer(vb, layout);

		//-----------------------------------------------------------------------------------------------------------

		// インデックスバッファの生成
		IndexBuffer ib(indices, 6);

		Shader shader("res/shaders/Basic.shader");		//シェーダープログラム	インスタンス
		shader.Bind();									//シェーダープログラムのバインド
		shader.SetUniform4f("u_Color", 1, 1, 1, 1.0);	//シェーダープログラムのColor変数の設定
		//-----------------------------------------------------------------------------------------------------------

		//stop
		Texture texture("res/textures/OpenGL.png");				//テクスチャー	インスタンス
		const int  texturesSlot_0 = 0;

		texture.Bind(texturesSlot_0);
		shader.SetUniform1i("u_Texture", texturesSlot_0);		//シェーダープログラムのテクスチャー変数の設定 : 0番目のテクスチャースロットを使用する。

		va.Unbind();
		vb.Unbind();
		ib.Unbind();
		shader.Unbind();

		Renderer renderer;

		float r = 0.0f;
		float increment = 0.05f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			//Escでプログラムを終了
			 if (isEscInput(window)){
				 break;
			 }			 

			// Render here
			renderer.Clear();

			shader.Bind();
			shader.SetUniform4f("u_Color",r, 0.3f, 0.8f, 1.0f);


			va.Bind(); //頂点配列のバインド
			ib.Bind(); //インデックスバッファのバインド

			renderer.Draw(va, ib, shader);


			if (r > 1.0f)
			{
				increment = -0.001f;
			}
			else if (r < 0.0f)
			{
				increment = 0.001f;
			}

			r += increment;


			// Swap front and back buffers
			glfwSwapBuffers(window);

			// Poll for and 6process events
			glfwPollEvents();

		}
	}

	FreeConsole();
	glfwTerminate();
	return 0;
}
