#pragma once

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

//
void GLClearError();

// OpenGLのエラーをチェックする
bool GLLogCall(const char* funtion, const char* file, int line);

class VertexArray;
class IndexBuffer;
class Shader;

class Renderer
{
public:
	void Clear() const;
	void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};