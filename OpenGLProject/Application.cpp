//OpenGLプロジェクト
// 作成日：2023/4/13
// 作成者：TOU
// 更新日：2023/4/13    現代のOpenGLをコードを更新した
// 

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

using std::cout;
using std::endl;

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    glewInit();

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glfwInit() != GLEW_OK)
    {
        cout << "Error" << endl;
    }

    // OpenGLのバージョンをコンソールに出力する
    cout << glGetString(GL_VERSION) << endl;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);

        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.f, 0.5f);
        glVertex2f(0.5f, -0.5f);

        glVertex2f(1.f, 1.f);
        glVertex2f(0.f, 0.f);
        glVertex2f(-1.f, -1.f);

		glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}