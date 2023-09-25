#include <iostream>
#include <chrono>

#include "glad.h" // Goes before GLFW include
#include "GLFW/glfw3.h"
#include "Shader.h"

#include <NsRender/GLFactory.h>
#include <NsApp/ThemeProviders.h>
#include <NsGui/FontProperties.h>
#include <NsGui/IntegrationAPI.h>
#include <NsGui/IRenderer.h>
#include <NsGui/IView.h>
#include <NsGui/Grid.h>
#include <NsGui/Uri.h>

int main()
{
	glfwInit();

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "Noesis Test", nullptr, nullptr);

	if (window == nullptr)
	{
		std::cout << "bad 1\n";
		return 1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
	{
		std::cout << "bad 1\n";
		return 2;
	}

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

	{
		// For simplicity purposes we are not using resource providers in this sample. ParseXaml() is
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

	// Renderer initialization with an OpenGL device
	_view->GetRenderer()->Init(NoesisApp::GLFactory::CreateDevice(false));

	auto lastTime = std::chrono::high_resolution_clock::now();
	auto accumulatedTime = 0.0f;

	//should be used on resize
	_view->SetSize(800, 600 * 0.25f);

	while(!glfwWindowShouldClose(window))
	{
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		// 450 = (height - 150); // 150 from _view->SetSize(800, 600 * 0.25f -> 150);
		// 225 = offset from the bottom of the window
		_view->MouseMove(xpos, ypos - 450 + 225);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			_view->MouseButtonDown(xpos, ypos - 450 + 225, Noesis::MouseButton_Left);
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			_view->MouseButtonUp(xpos, ypos - 450 + 225, Noesis::MouseButton_Left);
		}

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		auto currentTime = std::chrono::high_resolution_clock::now();
		auto deltaTime = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - lastTime).count();
		accumulatedTime += deltaTime;

		// Update view (layout, animations, ...)
		_view->Update(accumulatedTime);

		// Offscreen rendering phase populates textures needed by the on-screen rendering
		_view->GetRenderer()->UpdateRenderTree();
		_view->GetRenderer()->RenderOffscreen();

		// If you are going to render here with your own engine you need to restore the GPU state
		// because noesis changes it. In this case only framebuffer and viewport need to be restored

		glViewport(0, 225, 800, 150);
		glDisable(GL_SCISSOR_TEST);

		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearStencil(0);
		glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		// Rendering is done in the active framebuffer
		_view->GetRenderer()->Render();

		glViewport(0, 450, 800, 150);
		_view->MouseMove(xpos, ypos - 450 + 450);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			_view->MouseButtonDown(xpos, ypos - 450 + 450, Noesis::MouseButton_Left);
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			_view->MouseButtonUp(xpos, ypos - 450 + 450, Noesis::MouseButton_Left);
		}

		_view->Update(accumulatedTime);

		_view->GetRenderer()->UpdateRenderTree();

		_view->GetRenderer()->Render();

		glViewport(0, 0, 800, 150);
		_view->MouseMove(xpos, ypos - 450 + 0);

		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
		{
			_view->MouseButtonDown(xpos, ypos - 450 + 0, Noesis::MouseButton_Left);
		}
		else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
		{
			_view->MouseButtonUp(xpos, ypos - 450 + 0, Noesis::MouseButton_Left);
		}

		_view->Update(accumulatedTime);

		_view->GetRenderer()->UpdateRenderTree();

		_view->GetRenderer()->Render();

		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, 800, 600);


		glfwSwapBuffers(window);
		glfwPollEvents();

		lastTime = currentTime;
	}

	_view->Release();
	Noesis::GUI::Shutdown();

	return 0;
}
