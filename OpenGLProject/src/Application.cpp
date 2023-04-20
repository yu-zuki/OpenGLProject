// OpenGLプログラム
// 作成日：2023/4/13
// 作成者：TOU
// 
// 更新日：2023/4/13　　OpenGLを使用したプログラムを追加した。
// 更新日：2023/4/14	shaderのソースコードを追加しました。
// 更新日：2023/4/15	シェーダーのコンパイルを追加しました。
// 更新日：2023/4/18	VERTEX Buffersを追加しました。
// 更新日：2023/4/20	indices Buffersを追加しました。
// 更新日：2023/4/20	OpenGLのError Check機能を追加しました。glGetError()を使っての基本的なBugCheck。
// 

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

//
static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

// OpenGLのエラーをチェックする
static bool GLLogCall(const char* funtion, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << "): " << funtion << " " << file << ":" << line << std::endl;
		return false;
	}

	return true;
}

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	// シェーダーのソースコードの読み込み
	std::ifstream stream(filepath);

	// シェーダーの種類を判別するための列挙型
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))	// 1行ずつ読み込み
	{
		// シェーダーの種類を判別
		if (line.find("#shader") != std::string::npos)
		{
			// シェーダーの種類を表示
			if (line.find("vertex") != std::string::npos)
			{
				// 頂点シェーダー
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				// フラグメントシェーダー
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			// シェーダーのソースコードを追加
			ss[(int)type] << line << '\n';
		}
	}

	// シェーダーのソースコードを返却
	return { ss[0].str(), ss[1].str() };
}

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
	window = glfwCreateWindow(1080, 1080, "Hello World", NULL, NULL);
	if (!window)
	{
		// ウィンドウの作成に失敗
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); 	

	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error" << std::endl;
		return -1;
	}

	// OpenGLのバージョンを表示
	std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[] = {
		-0.5f, -0.5f,	//0
		 0.5f, -0.5f,	//1
		 0.5f,  0.5f,	//2
		 -0.5f,  0.5f,	//3
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int buffer = 0;
	// バッファの生成
	glGenBuffers(1, &buffer);

	// バインド　バッファの種類　バッファのID
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// バッファにデータを格納　
	glBufferData(GL_ARRAY_BUFFER, 6 * 2 *sizeof(float), positions, GL_STATIC_DRAW);

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
	
	// インデックスバッファの生成
	unsigned int ibo;
	// バッファの生成
	glGenBuffers(1, &ibo);

	// バインド　バッファの種類　バッファのID
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

	// バッファにデータを格納　
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6  * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	std::cout << "VERTEX" << std::endl;
	std::cout << source.VertexSource << std::endl;
	std::cout << "FRAGMENT" << std::endl;
	std::cout << source.FragmentSource << std::endl;

	// シェーダーのコンパイル
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	// シェーダーの使用
	glUseProgram(shader);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{  
		// Render here
		glClear(GL_COLOR_BUFFER_BIT);

		// 三角形を描画
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_INT, nullptr));
		
		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();

	}

	// プログラムの削除
	//glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}
