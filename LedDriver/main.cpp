// dear imgui: standalone example application for GLFW + OpenGL2, using legacy fixed pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

// **DO NOT USE THIS CODE IF YOUR CODE/ENGINE IS USING MODERN OPENGL (SHADERS, VBO, VAO, etc.)**
// **Prefer using the code in the example_glfw_opengl2/ folder**
// See imgui_impl_glfw.cpp for details.

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl2.h"

#include "LedWindow.h"
#include "LedDriver.h"
#include <windows.h>

#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <gl/GLU.h>

#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"


// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif


static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	glfwSetErrorCallback(glfw_error_callback);
	
	LedWindow window(1280, 720, "Led Driver");
	LedDriver driver;
	
	ImGui::CreateContext();
	
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window.GetLedWindow(), true);
	ImGui_ImplOpenGL2_Init();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("c:/windows/fonts/simhei.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.IniFilename = NULL;
	while (!glfwWindowShouldClose(window.GetLedWindow()))
	{
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		driver.Draw();
		//driver.InitControlWindow();
		//{
		//	ImGui::SetNextWindowPos(ImVec2(0, 0));
		//	ImGui::SetNextWindowSize(ImVec2(500, 720));
		//	ImGui::Begin(u8"你好窗口", false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		//	
		//	ImGui::Text(u8"绘制区域");

		//	//TODO:
		//	ImGui::Button("hello");
		//	
		//	ImGui::End();

		//}
		/*渲染*/
		ImGui::Render();
		//glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

		window.SwapBuffers();
		window.PollEvent();
	}

	// Cleanup
	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	return 0;
}