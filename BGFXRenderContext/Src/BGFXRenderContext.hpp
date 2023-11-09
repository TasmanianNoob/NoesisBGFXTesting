////
//// Created by charl on 31/10/2023.
////
//
//#ifndef NOESISTESTING_BGFXRENDERCONTEXT_HPP
//#define NOESISTESTING_BGFXRENDERCONTEXT_HPP
//
//#include <NsCore/Noesis.h>
//#include <NsCore/Ptr.h>
//#include <NsRender/RenderContext.h>
//
////README: NOT NEEDED?
////TODO: COMPLETE RENDERDEVICE/FACTORY FIRST?
//
//namespace Noesis { class RenderDevice; }
//
//class BGFXRenderContext final : public NoesisApp::RenderContext
//{
//public:
//	[[nodiscard]] const char* Description() const override;
//	[[nodiscard]] uint32_t Score() const override;
//	[[nodiscard]] bool Validate() const override;
//	void Init(void* window, uint32_t& samples, bool vsync, bool sRGB) override;
//	void Shutdown() override;
//	void SetWindow(void* window) override;
//	void SaveState() override;
//	[[nodiscard]] Noesis::RenderDevice* GetDevice() const override;
//	void BeginRender() override;
//	void EndRender() override;
//	void Resize() override;
//	[[nodiscard]] float GetGPUTimeMs() const override;
//	void SetClearColor(float r, float g, float b, float a) override;
//	void SetDefaultRenderTarget(uint32_t width, uint32_t height, bool doClearColor) override;
//	Noesis::Ptr<NoesisApp::Image> CaptureRenderTarget(Noesis::RenderTarget* surface) const override;
//	void Swap() override;
//	void* CreatePixelShader(const NoesisApp::ShaderSource& source) override;
//private:
//	Noesis::Ptr<Noesis::RenderDevice> mRenderer;
//
//	NS_DECLARE_REFLECTION(BGFXRenderContext, RenderContext)
//};
//
//
//#endif //NOESISTESTING_BGFXRENDERCONTEXT_HPP
