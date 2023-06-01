#pragma once

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
