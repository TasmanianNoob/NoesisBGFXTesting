//
// Created by charl on 31/10/2023.
//

#ifndef NOESISTESTING_BGFXRENDERDEVICE_HPP
#define NOESISTESTING_BGFXRENDERDEVICE_HPP

#include <array>

#include <NsRender/Texture.h>
#include <NsRender/RenderDevice.h>
#include <NsRender/RenderTarget.h>
#include <tuple>
#include <string_view>

#include "bgfx/bgfx.h"
#include "GLFW/glfw3.h"

//CREDIT HERE: https://github.com/Dakror/noesis-bgfx/tree/master & https://github.com/MarioSieg/BGFX_Noesis_Renderer/tree/master
class BGFXTexture : public Noesis::Texture
{
public:
	BGFXTexture(bgfx::TextureHandle handle, uint32_t width, uint32_t height, size_t stride, bool inverted);
	~BGFXTexture() override;

	// Returns the width of the texture
	[[nodiscard]] uint32_t GetWidth() const override;
	// Returns the height of the texture
	[[nodiscard]] uint32_t GetHeight() const override;
	// True if the texture has mipmaps
	[[nodiscard]] bool HasMipMaps() const override;
	// True is the texture must be vertically inverted when mapped. This is true for render targets
	// on platforms (OpenGL) where texture V coordinate is zero at the "bottom of the texture"
	[[nodiscard]] bool IsInverted() const override;
	[[nodiscard]] bool HasAlpha() const override;

public:
	const size_t stride;
	const bgfx::TextureHandle handle;

private:
	const uint32_t width;
	const uint32_t height;
	const bool inverted;
};

//CREDIT HERE: https://github.com/Dakror/noesis-bgfx/tree/master & https://github.com/MarioSieg/BGFX_Noesis_Renderer/tree/master
class BGFXRenderTarget : public Noesis::RenderTarget
{
public:
	BGFXRenderTarget(uint32_t width, uint32_t height);
	~BGFXRenderTarget() override;
	Noesis::Texture* GetTexture() override;

public:
	bgfx::TextureHandle stencilHandle;
	bgfx::FrameBufferHandle fboHandle;
	uint32_t width;
	uint32_t height;

private:
	Noesis::Ptr<BGFXTexture> texture;
};

//CREDIT HERE: https://github.com/Dakror/noesis-bgfx/tree/master & https://github.com/MarioSieg/BGFX_Noesis_Renderer/tree/master
class BGFXRenderDevice final : public Noesis::RenderDevice
{
public:
	explicit BGFXRenderDevice(GLFWwindow* window, bgfx::ViewId viewId, bool sRGB = false);
	~BGFXRenderDevice() override;

	[[nodiscard]] const Noesis::DeviceCaps& GetCaps() const override;
	Noesis::Ptr<Noesis::RenderTarget> CreateRenderTarget(const char* label, uint32_t width, uint32_t height,
														 uint32_t sampleCount, bool needsStencil) override;
	Noesis::Ptr<Noesis::RenderTarget> CloneRenderTarget(const char* label, Noesis::RenderTarget* surface) override;
	Noesis::Ptr<Noesis::Texture> CreateTexture(const char* label, uint32_t width, uint32_t height, uint32_t numLevels,
											   Noesis::TextureFormat::Enum format, const void** data) override;
	void UpdateTexture(Noesis::Texture* texture, uint32_t level, uint32_t x, uint32_t y, uint32_t width,
					   uint32_t height, const void* data) override;
	void BeginOffscreenRender() override;
	void EndOffscreenRender() override;
	void BeginOnscreenRender() override;
	void EndOnscreenRender() override;
	void SetRenderTarget(Noesis::RenderTarget* surface) override;
	void BeginTile(Noesis::RenderTarget* surface, const Noesis::Tile& tile) override;
	void EndTile(Noesis::RenderTarget* surface) override;
	void ResolveRenderTarget(Noesis::RenderTarget* surface, const Noesis::Tile* tiles, uint32_t numTiles) override;
	void* MapVertices(uint32_t bytes) override;
	void UnmapVertices() override;
	void* MapIndices(uint32_t bytes) override;
	void UnmapIndices() override;
	void DrawBatch(const Noesis::Batch& batch) override;
protected:
	int32_t OnDestroy() override;
private:
	static void CreateLayout(uint32_t attributes, bgfx::VertexLayout& layout);
	void CreatePrograms();
	void SetCaps(bool sRGB);
	void SetTextures(const Noesis::Batch& batch);
	void SetUniforms(const Noesis::Batch& batch);
	static uint32_t GetWrapTMode(const Noesis::SamplerState& sampler);
	static uint32_t GetWrapSMode(const Noesis::SamplerState& sampler);
	static uint32_t GetMinMode(const Noesis::SamplerState& sampler);
	static uint32_t GetMagMode(const Noesis::SamplerState& sampler);
	static uint32_t GetSamplerFlags(const Noesis::SamplerState& sampler);

	[[nodiscard]] bgfx::ShaderHandle LoadShader(const std::string& shaderName) const;
private:
	BGFXRenderTarget* renderTarget;
	Noesis::DeviceCaps caps;
	bgfx::ViewId viewId, maxViewId;

	static constexpr size_t totalVertexShaders = Noesis::Shader::Vertex::Count;
	static constexpr size_t totalFragmentShaders = Noesis::Shader::Count - 1; //no custom effect

	std::array<std::uint8_t, DYNAMIC_VB_SIZE> dynamicVB;
	std::array<std::uint8_t, DYNAMIC_IB_SIZE> dynamicIB;
	std::array<bgfx::VertexLayout, Noesis::Shader::Vertex::Format::Count> layouts;
	std::array<bgfx::ShaderHandle, totalVertexShaders> vertexShaders;
	std::array<std::string, totalVertexShaders> vertexShadersSource //COULD BE DONE WITH MAGIC_ENUM OR SIMILAR?
	{
			"Pos",
			"PosColor",
			"PosTex0",
			"PosTex0Rect",
			"PosTex0RectTile",
			"PosColorCoverage",
			"PosTex0Coverage",
			"PosTex0CoverageRect",
			"PosTex0CoverageRectTile",
			"PosColorTex1_SDF",
			"PosTex0Tex1_SDF",
			"PosTex0Tex1Rect_SDF",
			"PosTex0Tex1RectTile_SDF",
			"PosColorTex1",
			"PosTex0Tex1",
			"PosTex0Tex1Rect",
			"PosTex0Tex1RectTile",
			"PosColorTex0Tex1",
			"PosTex0Tex1_Downsample",
			"PosColorTex1Rect",
			"PosColorTex0RectImagePos"
	};
	std::array<bgfx::ShaderHandle, totalFragmentShaders> fragmentShaders;
	std::array<std::string, totalFragmentShaders> fragmentShadersSource //COULD BE DONE WITH MAGIC_ENUM OR SIMILAR?
	{
			"RGBA",
			"Mask",
			"Clear",
			"Path_Solid",
			"Path_Linear",
			"Path_Radial",
			"Path_Pattern",
			"Path_Pattern_Clamp",
			"Path_Pattern_Repeat",
			"Path_Pattern_MirrorU",
			"Path_Pattern_MirrorV",
			"Path_Pattern_Mirror",
			"Path_AA_Solid",
			"Path_AA_Linear",
			"Path_AA_Radial",
			"Path_AA_Pattern",
			"Path_AA_Pattern_Clamp",
			"Path_AA_Pattern_Repeat",
			"Path_AA_Pattern_MirrorU",
			"Path_AA_Pattern_MirrorV",
			"Path_AA_Pattern_Mirror",
			"SDF_Solid",
			"SDF_Linear",
			"SDF_Radial",
			"SDF_Pattern",
			"SDF_Pattern_Clamp",
			"SDF_Pattern_Repeat",
			"SDF_Pattern_MirrorU",
			"SDF_Pattern_MirrorV",
			"SDF_Pattern_Mirror",
			"SDF_LCD_Solid",
			"SDF_LCD_Linear",
			"SDF_LCD_Radial",
			"SDF_LCD_Pattern",
			"SDF_LCD_Pattern_Clamp",
			"SDF_LCD_Pattern_Repeat",
			"SDF_LCD_Pattern_MirrorU",
			"SDF_LCD_Pattern_MirrorV",
			"SDF_LCD_Pattern_Mirror",
			"Opacity_Solid",
			"Opacity_Linear",
			"Opacity_Radial",
			"Opacity_Pattern",
			"Opacity_Pattern_Clamp",
			"Opacity_Pattern_Repeat",
			"Opacity_Pattern_MirrorU",
			"Opacity_Pattern_MirrorV",
			"Opacity_Pattern_Mirror",
			"Upsample",
			"Downsample",
			"Shadow",
			"Blur"
	};
	std::array<bgfx::ProgramHandle, totalFragmentShaders> programs;
	std::array<bgfx::UniformHandle, 5> textureUniforms
	{
			bgfx::createUniform("s_pattern", bgfx::UniformType::Sampler),
			bgfx::createUniform("s_ramps", bgfx::UniformType::Sampler),
			bgfx::createUniform("s_image", bgfx::UniformType::Sampler),
			bgfx::createUniform("s_glyphs", bgfx::UniformType::Sampler),
			bgfx::createUniform("s_shadow", bgfx::UniformType::Sampler),
	};
	std::array<bgfx::UniformHandle, 3> uniforms
	{
			bgfx::createUniform("u_cbuffer1_vs", bgfx::UniformType::Vec4, 4),
			bgfx::createUniform("u_cbuffer0_fs", bgfx::UniformType::Vec4, 2),
			bgfx::createUniform("u_cbuffer1_fs", bgfx::UniformType::Vec4, 32),
	};

	static constexpr std::array<const bgfx::Attrib::Enum, Noesis::Shader::Vertex::Format::Attr::Count> SemanticAttributeLUT
	{
			bgfx::Attrib::Position,     // Pos
			bgfx::Attrib::Color0,       // Color
			bgfx::Attrib::TexCoord0,    // Tex0
			bgfx::Attrib::TexCoord1,    // Tex1
			bgfx::Attrib::TexCoord5,    // Coverage
			bgfx::Attrib::TexCoord2,    // Rect
			bgfx::Attrib::TexCoord3,    // Tile
			bgfx::Attrib::TexCoord4,    // ImagePos
	};

	static constexpr std::array<const std::tuple<const std::uint8_t, const bgfx::AttribType::Enum, const bool>, Noesis::Shader::Vertex::Format::Attr::Type::Count> SemanticTypeLUT
	{
			std::make_tuple(1, bgfx::AttribType::Float, false),  // Float
			std::make_tuple(2, bgfx::AttribType::Float, false),  // Float2
			std::make_tuple(4, bgfx::AttribType::Float, false),  // Float4
			std::make_tuple(4, bgfx::AttribType::Uint8, true),   // UByte4Norm
			std::make_tuple(4, bgfx::AttribType::Int16, true),   // UShort4Norm
	};

	size_t width, height;
	float identityMatrix[16] = {1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1};
};


#endif //NOESISTESTING_BGFXRENDERDEVICE_HPP
