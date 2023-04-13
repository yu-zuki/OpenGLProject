// OpenGL�v���O����
// ���t�F2023/4/13
// �쐬�ҁFTOU
// �X�V���F2023/4/13�@�@�����OpenGL���g�p�����v���O������ǉ������B
// 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>

using std::cout;
using std::endl;

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

	// OpenGL�̃o�[�W������\��
	cout << glGetString(GL_VERSION) << endl;

	float positions[6] = {
		-0.5f, -0.5f,
		0.0f, 0.5f,
		0.5f, -0.5f
	};

	unsigned int buffer = 0;
	// �o�b�t�@�̐���
	glGenBuffers(1, &buffer);

	// �o�C���h�@�o�b�t�@�̎�ށ@�o�b�t�@��ID
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// �o�b�t�@�Ƀf�[�^���i�[�@
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	// ���_�����̗L����
	glEnableVertexAttribArray(0);

	// ���_�����̐ݒ� 
	// 1:���_�����̃C���f�b�N�X 
	// 2:���_�����̗v�f�� 
	// 3:�f�[�^�̌^ 
	// 4:���K�����邩�ǂ��� 
	// 5:�f�[�^�̊Ԋu 
	// 6:�f�[�^�̐擪����̃I�t�Z�b�g
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// Render here
		glClear(GL_COLOR_BUFFER_BIT);

		// �O�p�`��`��
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// Swap front and back buffers
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();

	}

	glfwTerminate();
	return 0;
}
