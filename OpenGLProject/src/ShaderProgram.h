#pragma once

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	// �V�F�[�_�[�̃\�[�X�R�[�h�̓ǂݍ���
	std::ifstream stream(filepath);

	// �V�F�[�_�[�̎�ނ𔻕ʂ��邽�߂̗񋓌^
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))	// 1�s���ǂݍ���
	{
		// �V�F�[�_�[�̎�ނ𔻕�
		if (line.find("#shader") != std::string::npos)
		{
			// �V�F�[�_�[�̎�ނ�\��
			if (line.find("vertex") != std::string::npos)
			{
				// ���_�V�F�[�_�[
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				// �t���O�����g�V�F�[�_�[
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			// �V�F�[�_�[�̃\�[�X�R�[�h��ǉ�
			ss[(int)type] << line << '\n';
		}
	}

	// �V�F�[�_�[�̃\�[�X�R�[�h��ԋp
	return { ss[0].str(), ss[1].str() };
}

static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	// �V�F�[�_�[�̍쐬
	unsigned int id = glCreateShader(type);

	// �V�F�[�_�[�̃\�[�X�R�[�h�̐ݒ�
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	// �V�F�[�_�[�̃R���p�C��
	glCompileShader(id);

	// �G���[�`�F�b�N
	int result;

	// �V�F�[�_�[�̃R���p�C�����ʂ��擾
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		// �V�F�[�_�[�̃R���p�C�����ʂ̒������擾
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		// Stack�̈�Ń������[���m�ۂ���
		char* message = (char*)alloca(length * sizeof(char));

		// �V�F�[�_�[�̃R���p�C�����
		glGetShaderInfoLog(id, length, &length, message);

		// �G���[�̕\��
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment")
			<< " shader!" << std::endl;

		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}
// �V�F�[�_�[�̃R���p�C��
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	// �V�F�[�_�[�̍쐬
	unsigned int program = glCreateProgram();
	// ���_�V�F�[�_�[�̃R���p�C��
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	// �t���O�����g�V�F�[�_�[�̃R���p�C��
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	//�@�v���O�����ɃV�F�[�_�[���A�^�b�`
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	// �v���O�����̃����N
	glLinkProgram(program);

	// �G���[�`�F�b�N
	glValidateProgram(program);

	// �V�F�[�_�[�̒��ԃt�@�C�����폜
	glDeleteShader(vs);
	glDeleteShader(fs);

	//�@�v���O�����̕ԋp
	return program;
}
