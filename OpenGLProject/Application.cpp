// OpenGLプログラム
// 日付：2023/4/13
// 作成者：TOU
// 更新日：2023/4/13　　現代のOpenGLを使用したプログラムを追加した。
// 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	// シェーダーの作成
	unsigned int id = glCreateShader(type);

	// シェーダーのソースコードの設定
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	// シェーダーのコンパイル
	glCompileShader(id);

	// エラーチェック
	int result;

	// シェーダーのコンパイル結果を取得
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		// シェーダーのコンパイル結果の長さを取得
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		// Stack領域でメモリーを確保する
		char* message = (char*)alloca(length * sizeof(char));

		// シェーダーのコンパイル情報
		glGetShaderInfoLog(id, length, &length, message);

		// エラーの表示
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") 
			<< " shader!" << std::endl;

		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}
// シェーダーのコンパイル
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// シェーダーの作成
	unsigned int program = glCreateProgram();
	// 頂点シェーダーのコンパイル
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	// フラグメントシェーダーのコンパイル
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//　プログラムにシェーダーをアタッチ
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	// プログラムのリンク
	glLinkProgram(program);

	// エラーチェック
	glValidateProgram(program);

	// シェーダーの中間ファイルを削除
	glDeleteShader(vs);
	glDeleteShader(fs);

	//　プログラムの返却
	return program;
}


int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1920, 1080, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	if (glewInit() != GLEW_OK)
	{
		cout << "Error" << endl;
		return -1;
	}

	// OpenGLのバージョンを表示
	cout << glGetString(GL_VERSION) << endl;

	float positions[6] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};

	unsigned int buffer = 0;
	// バッファの生成
	glGenBuffers(1, &buffer);

	// バインド　バッファの種類　バッファのID
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// バッファにデータを格納　
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	// 頂点属性の有効化
	glEnableVertexAttribArray(0);

	// 頂点属性の設定 
	// 1:頂点属性のインデックス 
	// 2:頂点属性の要素数 
	// 3:データの型 
	// 4:正規化するかどうか 
	// 5:データの間隔 
	// 6:データの先頭からのオフセット
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// Render here
		glClear(GL_COLOR_BUFFER_BIT);

		// 三角形を描画
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}
