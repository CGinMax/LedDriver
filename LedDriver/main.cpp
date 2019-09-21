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
#include "IconsFontAwesome5.h"
//#include <windows.h>
#include <opencv2/opencv.hpp>

#include <GLFW/glfw3.h>
#include <gl/GL.h>
#include <gl/GLU.h>

//#ifndef STB_IMAGE_IMPLEMENTATION
//#define STB_IMAGE_IMPLEMENTATION
//#endif
//#include "stb_image.h"


// [Win32] Our example includes a copy of glfw3.lib pre-compiled with VS2010 to maximize ease of testing and compatibility with old VS compilers.
// To link with VS2010-era libraries, VS2015+ requires linking with legacy_stdio_definitions.lib, which we do using this pragma.
// Your own project should not be affected, as you are likely to link with a newer binary of GLFW that is adequate for your version of Visual Studio.
#if defined(_MSC_VER) && (_MSC_VER >= 1900) && !defined(IMGUI_DISABLE_WIN32_FUNCTIONS)
#pragma comment(lib, "legacy_stdio_definitions")
#endif

void DrawImageTest()
{
	ImDrawList *dl = ImGui::GetWindowDrawList();
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	cv::Mat mDrawImage = cv::imread("C:\\Users\\CGinMax\\Desktop\\µØÇò.jpg", cv::IMREAD_UNCHANGED);
	//cv::imshow("hello", mDrawImage);
	int mWidth = mDrawImage.cols;
	int mHeight = mDrawImage.rows;
	int mChannels = mDrawImage.channels();
	GLubyte *imagepixel;
	imagepixel = new GLubyte[mWidth*mHeight * mChannels];
	memcpy(imagepixel, mDrawImage.data, mWidth*mHeight * mChannels * sizeof(unsigned char));
	GLubyte *tmpPixels = imagepixel;
	if (tmpPixels)
	{
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, mWidth, mHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, imagepixel);
		if (mChannels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, mWidth, mHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, imagepixel);
		if (mChannels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, 4, mWidth, mHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, tmpPixels);

		/*if (mChannels == 3)
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, mWidth, mHeight, GL_BGR_EXT, GL_UNSIGNED_BYTE, (void*)imagepixel);
		if (mChannels == 4)
			gluBuild2DMipmaps(GL_TEXTURE_2D, GL_BGRA_EXT, mWidth, mHeight, GL_BGRA_EXT, GL_UNSIGNED_BYTE, (void*)imagepixel);
*/
	}
	ImGui::Value("nr_channels: ", mChannels);
	ImGui::Image((ImTextureID)texture, ImVec2(float(mWidth), float(mHeight)));
	delete[] imagepixel;
}


static void glfw_error_callback(int error, const char* description)
{
	fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	glfwSetErrorCallback(glfw_error_callback);

	LedWindow window(1280, 720, "Led Driver");
	std::shared_ptr<CommonData> projectData = std::make_shared<CommonData>();
	//CommonData *projectData = new CommonData;
	LedDriver *driver = new LedDriver(projectData);
	ImGui::CreateContext();
	
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window.GetLedWindow(), true);
	ImGui_ImplOpenGL2_Init();

	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("Fonts/simhei.ttf", 15.0f, NULL, io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
	//Ìí¼Ófont icon
	static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	io.Fonts->AddFontFromFileTTF("Fonts/fa-solid-900.ttf", 15.0f, &icons_config, icons_ranges);

	io.MouseDrawCursor = true;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.IniFilename = NULL;
	
	ImGuiStyle& style = ImGui::GetStyle();
	style.Colors[ImGuiCol_Button] = ImColor(35, 106, 190, 180);
	style.Colors[ImGuiCol_TabActive] = ImColor(48, 49, 51, 249);
	style.Colors[ImGuiCol_TabHovered] = ImColor(25, 185, 238, 204);
	style.WindowPadding.x = 2.0f;
	//style.FrameRounding = 8.0f;
	style.ScrollbarSize = 17.0f;
	//driver->InitLineImage();

	while (!glfwWindowShouldClose(window.GetLedWindow()))
	{
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		driver->Draw();
		
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
	delete driver;

	return 0;
}