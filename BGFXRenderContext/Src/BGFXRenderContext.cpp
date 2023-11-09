////
//// Created by charl on 31/10/2023.
////
//
////README: NOT NEEDED?
//#include "BGFXRenderContext.hpp"
//
//const char* BGFXRenderContext::Description() const
//{
//	return "BGFX";
//}
//
//uint32_t BGFXRenderContext::Score() const
//{
//	return 350;
//}
//
//Noesis::RenderDevice* BGFXRenderContext::GetDevice() const
//{
//	return mRenderer;
//}
//
////TODO: IMPLEMENT ALL BELOW
//bool BGFXRenderContext::Validate() const
//{
//	return RenderContext::Validate();
//}
//
//void BGFXRenderContext::Init(void* window, uint32_t& samples, bool vsync, bool sRGB)
//{
//	RenderContext::Init(window, samples, vsync, sRGB);
//}
//
//void BGFXRenderContext::Shutdown()
//{
//	RenderContext::Shutdown();
//}
//
//void BGFXRenderContext::SetWindow(void* window)
//{
//	RenderContext::SetWindow(window);
//}
//
//void BGFXRenderContext::SaveState()
//{
//	RenderContext::SaveState();
//}
//
//void BGFXRenderContext::BeginRender()
//{
//	RenderContext::BeginRender();
//}
//
//void BGFXRenderContext::EndRender()
//{
//	RenderContext::EndRender();
//}
//
//void BGFXRenderContext::Resize()
//{
//	RenderContext::Resize();
//}
//
//float BGFXRenderContext::GetGPUTimeMs() const
//{
//	return RenderContext::GetGPUTimeMs();
//}
//
//void BGFXRenderContext::SetClearColor(float r, float g, float b, float a)
//{
//	RenderContext::SetClearColor(r, g, b, a);
//}
//
//void BGFXRenderContext::SetDefaultRenderTarget(uint32_t width, uint32_t height, bool doClearColor)
//{
//	RenderContext::SetDefaultRenderTarget(width, height, doClearColor);
//}
//
//Noesis::Ptr<NoesisApp::Image> BGFXRenderContext::CaptureRenderTarget(Noesis::RenderTarget* surface) const
//{
//	return RenderContext::CaptureRenderTarget(surface);
//}
//
//void BGFXRenderContext::Swap()
//{
//	RenderContext::Swap();
//}
//
//void* BGFXRenderContext::CreatePixelShader(const NoesisApp::ShaderSource& source)
//{
//	return RenderContext::CreatePixelShader(source);
//}
//
//void BGFXRenderContext::StaticFillClassType(Noesis::TypeClassCreator& helper)
//{
//
//}
//
//const Noesis::TypeClass* BGFXRenderContext::GetClassType() const
//{
//	return RenderContext::GetClassType();
//}
//
//const Noesis::TypeClass* BGFXRenderContext::StaticGetClassType(Noesis::TypeTag<BGFXRenderContext>*)
//{
//	return nullptr;
//}
