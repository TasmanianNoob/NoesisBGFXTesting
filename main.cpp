#include <iostream>
#include <chrono>

#include "bgfx/bgfx.h"
#include "bgfx/platform.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include "GLFW/glfw3.h"
#include "GLFW/glfw3native.h"

#include "BGFXRenderDevice/Include/NsRender/BGFXFactory.hpp"

#include <NsApp/ThemeProviders.h>
#include <NsGui/IntegrationAPI.h>
#include <NsGui/IRenderer.h>
#include <NsGui/IView.h>
#include <NsGui/Grid.h>
#include <NsGui/Uri.h>

static bool showDebug = false;

static void glfw_keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_F1 && action == GLFW_RELEASE)
	{
		showDebug = !showDebug;
	}
}

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	int width = 1000, height = 600;

	GLFWwindow* window = glfwCreateWindow(width, height, "Noesis Test", nullptr, nullptr);


	if (window == nullptr)
	{
		std::cout << "bad 1\n";
		return 1;
	}

	glfwSetKeyCallback(window, glfw_keyCallback);

	bgfx::Init init;

	init.type = bgfx::RendererType::Enum::Count;
	init.platformData.nwh = glfwGetWin32Window(window);

	//MacOS retina screen display DPI shenanigans
	int glfwWidth, glfwHeight;
	glfwGetWindowSize(window, &glfwWidth, &glfwHeight);
	init.resolution.width = (uint32_t)glfwWidth;
	init.resolution.height = (uint32_t)glfwHeight;
	init.resolution.reset = BGFX_RESET_VSYNC;

	init.debug = true;

	bgfx::renderFrame();
	bgfx::init(init);

	bgfx::ViewId viewId = 0;
	bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x000000FF, 1.0f, 0);
	bgfx::setViewRect(viewId, 0, 0, bgfx::BackbufferRatio::Equal);

	Noesis::IView* _view;

	Noesis::GUI::SetLogHandler([](const char*, uint32_t, uint32_t level, const char*, const char* msg)
							   {
								   // [TRACE] [DEBUG] [INFO] [WARNING] [ERROR]
								   const char* prefixes[] = { "T", "D", "I", "W", "E" };
								   printf("[NOESIS/%s] %s\n", prefixes[level], msg);
							   });

	// https://www.noesisengine.com/docs/Gui.Core.Licensing.html
	Noesis::GUI::SetLicense(NS_LICENSE_NAME, NS_LICENSE_KEY);

	// Noesis initialization. This must be the first step before using any NoesisGUI functionality
	Noesis::GUI::Init();

	// Setup theme
	NoesisApp::SetThemeProviders();
	Noesis::GUI::LoadApplicationResources("Theme/NoesisTheme.DarkBlue.xaml");

	{    // For simplicity purposes we are not using resource providers in this sample. ParseXaml() is
		// enough if there is no extra XAML dependencies
		Noesis::Ptr<Noesis::Grid> xaml(Noesis::GUI::ParseXaml<Noesis::Grid>(R"(
			<Grid xmlns="http://schemas.microsoft.com/winfx/2006/xaml/presentation">
				<Grid.Background>
					<LinearGradientBrush StartPoint="0,0" EndPoint="0,1">
						<GradientStop Offset="0" Color="#FF123F61"/>
						<GradientStop Offset="0.6" Color="#FF0E4B79"/>
						<GradientStop Offset="0.7" Color="#FF106097"/>
					</LinearGradientBrush>
				</Grid.Background>
				<Viewbox>
					<StackPanel Margin="50">
						<Button Content="Hello World!" Margin="0,30,0,0"/>
						<Rectangle Height="5" Margin="-10,20,-10,0">
							<Rectangle.Fill>
								<RadialGradientBrush>
									<GradientStop Offset="0" Color="#40000000"/>
									<GradientStop Offset="1" Color="#00000000"/>
								</RadialGradientBrush>
							</Rectangle.Fill>
						</Rectangle>
					</StackPanel>
				</Viewbox>
			</Grid>
		)"));

		// View creation to render and interact with the user interface
		// We transfer the ownership to a global pointer instead of a Ptr<> because there is no way
		// in GLUT to do shutdown and we don't want the Ptr<> to be released at global time
		_view = Noesis::GUI::CreateView(xaml).GiveOwnership();
	}

	_view->SetFlags(Noesis::RenderFlags_PPAA | Noesis::RenderFlags_LCD);

	auto renderDevice = BGFXFactory::CreateDevice(window, viewId, true);

	// Renderer initialization with an OpenGL device
	_view->GetRenderer()->Init(renderDevice);

	auto lastTime = std::chrono::high_resolution_clock::now();
	auto accumulatedTime = 0.0f;

	_view->SetSize(width, height);

	while(!glfwWindowShouldClose(window))
	{
		auto currentTime = std::chrono::high_resolution_clock::now();
		auto deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
		accumulatedTime += deltaTime;

		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		_view->MouseMove(xpos, ypos);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			_view->MouseButtonDown(xpos, ypos, Noesis::MouseButton_Left);
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			_view->MouseButtonUp(xpos, ypos, Noesis::MouseButton_Left);
		}

		// Update view (layout, animations, ...)
		_view->Update(accumulatedTime);

		// Offscreen rendering phase populates textures needed by the on-screen rendering
		_view->GetRenderer()->UpdateRenderTree();
		_view->GetRenderer()->RenderOffscreen();

		// If you are going to render here with your own engine you need to restore the GPU state
		// because noesis changes it. In this case only framebuffer and viewport need to be restored
		bgfx::reset(width, height, BGFX_RESET_VSYNC);
		bgfx::resetView(0);
		bgfx::setViewRect(0, 0, 0, width, height);
		bgfx::touch(0);

		// Rendering is done in the active framebuffer
		_view->GetRenderer()->Render();

		bgfx::setDebug(showDebug ? BGFX_DEBUG_STATS : BGFX_DEBUG_NONE);
		bgfx::frame();

		glfwPollEvents();

		lastTime = currentTime;
	}

	_view->GetRenderer()->Shutdown();
	_view->Release();
	Noesis::GUI::Shutdown();

	return 0;
}
