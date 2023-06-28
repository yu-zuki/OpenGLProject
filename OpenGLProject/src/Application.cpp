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
// 更新日：2023/4/20	Uniformsを追加し ました。
// 更新日：2023/4/21	Vertex arraysを追加しました。
// 更新日：2023/4/27	オブジェクト指向でコードを再構成(IndexBuffer VertexBuffer)
// 更新日：2023/5/09	オブジェクト指向でコードを再構成(VertexArray)
// 更新日：2023/5/29	オブジェクト指向でコードを再構成(VertexArray)
// 更新日：2023/6/9		オブジェクト指向でコードを再構成(Render) 
// 更新日：2023/6/9		画像をGPUに読み込む機能を追加しました。　stb_image.hを使用しています。
//						(stb_image.h　使用方法：画像のパスを渡して、RGBAのpixels bufferのpointerがReturnされる） 
// 更新日：2023/6/14	Textureクラスを追加しました。
//						機能：画像をGPUに読み込む機能を追加しました。
// 更新日：2023/6/28	GUIを追加しました。　imguiを使用しています。
// 更新日：2023/6/28	基本なテスト機能を追加しました。
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

#include "vendor/glm/glm.hpp"
#include "vendor/glm/gtc/matrix_transform.hpp"

#include "vendor/imgui/imgui.h"
#include "vendor/imgui/imgui_impl_glfw_gl3.h"

#include "tests/TestClearColor.h"
#include "tests/TestTexture2D.h"

int main(void)
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
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

		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));	//ブレンドの設定 : 透過の設定 
		GLCall(glEnable(GL_BLEND));									//ブレンドの設定

		Renderer renderer;											//レンダラー	インスタンス

		ImGui::CreateContext();					//ImGuiのコンテキストの作成
		ImGui_ImplGlfwGL3_Init(window, true);	//ImGuiの初期化
		ImGui::StyleColorsDark(); 				//ImGuiのスタイルの設定


		test::TestClearColor test;				//テストクラスのインスタンス

		test::Test *currentTest = nullptr;							//テストクラスのポインター
		test::TestMenu* testMenu = new test::TestMenu(currentTest);	//テストメニューのインスタンス
		currentTest = testMenu;										//デフォルトTestをTestMenuに設定

		testMenu->RegisterTest<test::TestClearColor>("TestClearColor");		//テストメニューにTestClearColorを追加
		testMenu->RegisterTest<test::TestTexture2D>("TestTexture2D");		//テストメニューにTestTexture2Dを追加

		while (!glfwWindowShouldClose(window))
		{
			//Escでプログラムを終了
			 if (isEscInput(window)){
				 break;
			 }			 
	
			GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));	//背景色を黒に指定
			renderer.Clear();								//レンダラーのクリア			

			ImGui_ImplGlfwGL3_NewFrame(); //Gui
			
			if (currentTest)
			{
				currentTest->OnUpdate(0.f);
				currentTest->OnRender();

				//ImGui::SetNextWindowSize(ImVec2(500, 500));

				ImGui::Begin("Test"); //Gui
				if ( currentTest != testMenu  &&  ImGui::Button("<-") )
				{
					delete currentTest;
					currentTest = testMenu;
				}

				currentTest->OnImGuiRender();
				ImGui::End();
			}

			ImGui::Render(); //ImGuiのレンダリング

			ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData()); //ImGuiの描画

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
