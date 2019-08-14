#include "LedWindow.h"
#include <exception>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


LedWindow::LedWindow(int width, int hight, const char * title_name)
	: m_pWindow(NULL), m_iWidth(width), m_iHight(hight), m_title_name(title_name)
{
	
	this->Init();
}

LedWindow::~LedWindow()
{
	glfwTerminate();
}

void LedWindow::Init()
{
	/*初始化窗口*/
	if (!glfwInit())
	{
		throw std::exception("glfwInit Error");
	}

	/*配置GLFW*/
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	/*创建窗口*/
	m_pWindow = glfwCreateWindow(this->m_iWidth, this->m_iHight, this->m_title_name, NULL, NULL);
	if (m_pWindow == NULL)
	{
		glfwTerminate();
		throw std::exception("Create Window Error");
	}
	
	//int n;
	//m_Image[0].pixels = stbi_load("C:\\Users\\CGinMax\\Desktop\\ledimage\\homo.jpg", &m_Image[0].width, &m_Image[0].height, &n, 0);
	//glfwSetWindowIcon(m_pWindow, 1, m_Image);
	/*配置GLFW上下文*/
	glfwMakeContextCurrent(m_pWindow);

	glfwSwapInterval(1); 

	glfwGetFramebufferSize(m_pWindow, &m_iWidth, &m_iHight);
	glViewport(0, 0, m_iWidth, m_iHight);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	/*设置窗口居中显示*/
	const GLFWvidmode* centralmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(m_pWindow, (centralmode->width - m_iWidth) / 2, (centralmode->height - m_iHight) / 2);

}

void LedWindow::PollEvent()
{
	glfwPollEvents();
}

void LedWindow::SwapBuffers()
{
	glfwSwapBuffers(m_pWindow);
}

GLFWwindow *LedWindow::GetLedWindow()
{
	
	return this->m_pWindow;
	
}
