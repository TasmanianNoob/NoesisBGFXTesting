//
// Created by charl on 31/10/2023.
//

#include "BGFXRenderDevice.hpp"

#include <stdexcept>
#include <fstream>
#include <string>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/ext/matrix_clip_space.hpp"

#include "bgfx/platform.h"

BGFXRenderDevice::BGFXRenderDevice(GLFWwindow* window, const bgfx::ViewId viewId, const bool sRGB) : viewId(viewId)
{
	bgfx::setViewName(viewId, "Noesis");
	bgfx::setViewMode(viewId, bgfx::ViewMode::Sequential);

	for (int i = 0; i < Noesis::Shader::Vertex::Format::Count; ++i)
	{
		const uint32_t attributes = Noesis::AttributesForFormat[i];
		CreateLayout(attributes, layouts[i]);
	}

	SetCaps(sRGB);

	CreatePrograms();

	glfwGetWindowSize(window, (int*) &width, (int*) &height);
}

BGFXRenderDevice::~BGFXRenderDevice()
{
}

const Noesis::DeviceCaps& BGFXRenderDevice::GetCaps() const
{
	return caps;
}

Noesis::Ptr<Noesis::RenderTarget> BGFXRenderDevice::CreateRenderTarget(const char* label, uint32_t width,
																	   uint32_t height, uint32_t sampleCount,
																	   bool needsStencil)
{
	Noesis::Ptr<BGFXRenderTarget> renderTarget = *new BGFXRenderTarget(width, height);

	renderTarget->stencilHandle = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::D24S8);

	const std::array<bgfx::TextureHandle, 2> attachments = { ((BGFXTexture*) renderTarget->GetTexture())->handle, renderTarget->stencilHandle };
	renderTarget->fboHandle = bgfx::createFrameBuffer(attachments.size(), attachments.data(), false);

	return renderTarget;
}

Noesis::Ptr<Noesis::RenderTarget> BGFXRenderDevice::CloneRenderTarget(const char* label, Noesis::RenderTarget* surface)
{
	BGFXRenderTarget* castSurface = (BGFXRenderTarget*) surface;
	Noesis::Ptr<BGFXRenderTarget> renderTarget = *new BGFXRenderTarget(castSurface->width, castSurface->height);

	const std::array<bgfx::TextureHandle, 2> attachments = { ((BGFXTexture*) castSurface->GetTexture())->handle, castSurface->stencilHandle };
	renderTarget->fboHandle = bgfx::createFrameBuffer(attachments.size(), attachments.data(), false);

	return renderTarget;
}

Noesis::Ptr<Noesis::Texture> BGFXRenderDevice::CreateTexture(const char* label, uint32_t width, uint32_t height,
															 uint32_t numLevels, Noesis::TextureFormat::Enum format,
															 const void** data)
{
	bgfx::TextureFormat::Enum textureFormat;
	size_t stride;

	switch (format)
	{
		case Noesis::TextureFormat::RGBA8:
			textureFormat = bgfx::TextureFormat::RGBA8;
			stride = 4;
			break;
		case Noesis::TextureFormat::R8:
			textureFormat = bgfx::TextureFormat::R8;
			stride = 1;
			break;
		default:
			throw std::runtime_error("Unsupported texture format: BGFXRenderDevice::CreateTexture");
	}

	return *new BGFXTexture(bgfx::createTexture2D(width, height, false, 1, textureFormat, (uint64_t) BGFX_TEXTURE_RT, data != nullptr ? bgfx::copy(*data, width * height * stride) : nullptr), width, height, stride, false);
}

void BGFXRenderDevice::UpdateTexture(Noesis::Texture* texture, uint32_t level, uint32_t x, uint32_t y, uint32_t width,
									 uint32_t height, const void* data)
{
	auto* tex = dynamic_cast<BGFXTexture*>(texture);

	bgfx::updateTexture2D(tex->handle, 0, level, x, y, width, height, bgfx::copy(data, width * height * tex->stride));
}

void BGFXRenderDevice::BeginOffscreenRender()
{
}

void BGFXRenderDevice::EndOffscreenRender()
{
}

void BGFXRenderDevice::BeginOnscreenRender()
{
	bgfx::setViewRect(this->viewId, 0, 0, this->width, this->height);
}

void BGFXRenderDevice::EndOnscreenRender()
{
}

void BGFXRenderDevice::SetRenderTarget(Noesis::RenderTarget* surface)
{
	std::cout << "DEBUGGING BGFXRenderDevice::SetRenderTarget(Noesis::RenderTarget* surface)\n";

	renderTarget = dynamic_cast<BGFXRenderTarget*>(surface);
	//TODO: SHOULD SET VIEWPORT?
	std::cout << "OKAY IF BGFXRenderDevice::BeginTile IS NEXT\n";
}

void BGFXRenderDevice::BeginTile(Noesis::RenderTarget* surface, const Noesis::Tile& tile)
{
	std::cout << "DEBUGGING BGFXRenderDevice::BeginTile(Noesis::RenderTarget* surface, const Noesis::Tile& tile)\n";

	auto castSurface = dynamic_cast<BGFXRenderTarget*>(surface);

	maxViewId++;

	bgfx::setViewClear(maxViewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_STENCIL, 0);
	bgfx::setViewName(maxViewId, "Noesis FBO 1");
	bgfx::setViewMode(maxViewId, bgfx::ViewMode::Sequential);
	bgfx::setViewRect(maxViewId, 0, 0, castSurface->width, castSurface->height);
	bgfx::setViewScissor(maxViewId, tile.x, tile.y, tile.width, tile.height);
	bgfx::setViewTransform(maxViewId, identityMatrix, glm::value_ptr(glm::ortho(-0.5f * width, 0.5f * width, -0.5f * height, 0.5f * height, 0.0f, 10.0f)));
	bgfx::setViewFrameBuffer(maxViewId, renderTarget->fboHandle);

	viewId = maxViewId;
}

void BGFXRenderDevice::EndTile(Noesis::RenderTarget* surface)
{
	bgfx::setViewScissor(viewId, 0, 0, 0, 0);
}

void BGFXRenderDevice::ResolveRenderTarget(Noesis::RenderTarget* surface, const Noesis::Tile* tiles, uint32_t numTiles)
{
}

void* BGFXRenderDevice::MapVertices(uint32_t bytes)
{
	return dynamicVB.data();
}

void BGFXRenderDevice::UnmapVertices()
{
}

void* BGFXRenderDevice::MapIndices(uint32_t bytes)
{
	return dynamicIB.data();
}

void BGFXRenderDevice::UnmapIndices()
{
}

void BGFXRenderDevice::DrawBatch(const Noesis::Batch& batch)
{
	bgfx::setViewTransform(viewId, identityMatrix, batch.vertexUniforms[0].values);

	bgfx::VertexLayout& layout = layouts[Noesis::FormatForVertex[Noesis::VertexForShader[batch.shader.v]]];

	uint8_t* begin, *end, *target;
	bgfx::TransientVertexBuffer vertexBuffer = {};
	bgfx::TransientIndexBuffer indexBuffer = {};

	bgfx::allocTransientVertexBuffer(&vertexBuffer, batch.numVertices, layout);
	target = vertexBuffer.data;
	begin = dynamicVB.data() + batch.vertexOffset;
	end = begin + batch.numVertices * layout.getStride();
	std::copy(begin, end, target);
	bgfx::setVertexBuffer(0, &vertexBuffer, 0, batch.numVertices);

	bgfx::allocTransientIndexBuffer(&indexBuffer, batch.numIndices);
	target = indexBuffer.data;
	begin = dynamicIB.data() + batch.startIndex * sizeof(uint16_t);
	end = begin + batch.numIndices * sizeof(uint16_t);
	std::copy(begin, end, target);
	bgfx::setIndexBuffer(&indexBuffer, batch.startIndex, batch.numIndices);

	uint32_t state = 0, stencil = 0;

	if (batch.renderState.f.blendMode)
	{
		state |= BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_ONE, BGFX_STATE_BLEND_INV_SRC_ALPHA) | BGFX_STATE_BLEND_EQUATION_ADD;
	}
	if (batch.renderState.f.colorEnable)
	{
		state |= BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A;
	}
	if (batch.renderState.f.stencilMode)
	{
		switch ((Noesis::StencilMode::Enum)batch.renderState.f.stencilMode)
		{
			case Noesis::StencilMode::Disabled:
				break;
			case Noesis::StencilMode::Equal_Keep:
				stencil |= BGFX_STENCIL_OP_FAIL_S_KEEP | BGFX_STENCIL_OP_FAIL_Z_KEEP | BGFX_STENCIL_OP_PASS_Z_KEEP;
				break;
			case Noesis::StencilMode::Equal_Incr:
				stencil |= BGFX_STENCIL_OP_FAIL_S_KEEP | BGFX_STENCIL_OP_FAIL_Z_KEEP | BGFX_STENCIL_OP_PASS_Z_INCR;
				break;
			case Noesis::StencilMode::Equal_Decr:
				stencil |= BGFX_STENCIL_OP_FAIL_S_KEEP | BGFX_STENCIL_OP_FAIL_Z_KEEP | BGFX_STENCIL_OP_PASS_Z_DECR;
				break;
			default:
				abort();
		}
	}
	if (batch.stencilRef)
	{
		stencil |= BGFX_STENCIL_TEST_EQUAL | BGFX_STENCIL_FUNC_REF(batch.stencilRef) | BGFX_STENCIL_FUNC_RMASK(0xFF);
	}
	else if (stencil != 0)
	{
		stencil |= BGFX_STENCIL_TEST_ALWAYS;
	}

	state |= BGFX_STATE_MSAA;

	SetTextures(batch);
	SetUniforms(batch);

	bgfx::setStencil(stencil);
	bgfx::setState(state);
	bgfx::submit(viewId, programs[batch.shader.v]);
}

int32_t BGFXRenderDevice::OnDestroy()
{
	for (bgfx::ShaderHandle& vShader : vertexShaders) bgfx::destroy(vShader);
	for (bgfx::ShaderHandle& fShader : fragmentShaders) bgfx::destroy(fShader);
	for (bgfx::ProgramHandle& program : programs) bgfx::destroy(program);
	for (bgfx::UniformHandle& texUniform : textureUniforms) bgfx::destroy(texUniform);
	for (bgfx::UniformHandle& uniform : uniforms) bgfx::destroy(uniform);

	return 0;
}

void BGFXRenderDevice::SetCaps(const bool sRGB)
{
	auto bgfxCaps = bgfx::getCaps();

	switch (bgfxCaps->rendererType)
	{
		case bgfx::RendererType::Direct3D9:
		case bgfx::RendererType::Direct3D11:
		case bgfx::RendererType::Direct3D12:
		case bgfx::RendererType::Vulkan:
			//	caps.centerPixelOffset = 0.5f;
			//	break;
		case bgfx::RendererType::OpenGL:
		case bgfx::RendererType::Metal:
			//	caps.centerPixelOffset = 0.0f;
			//	break;
		default:
			caps.centerPixelOffset = 0.0f;
			//	throw std::runtime_error("Unsupported renderer type: BGFXRenderDevice::BGFXRenderDevice(const bgfx::ViewId viewId");
	}

	caps.clipSpaceYInverted = bgfxCaps->originBottomLeft;
	caps.depthRangeZeroToOne = !bgfxCaps->homogeneousDepth;
	caps.linearRendering = false; //sRGB;
	caps.subpixelRendering = false;
}

void BGFXRenderDevice::CreateLayout(uint32_t attributes, bgfx::VertexLayout& layout)
{
	layout.begin();

	for (int i = 0; i < Noesis::Shader::Vertex::Format::Attr::Count; ++i)
	{
		if (attributes & (1 << i))
		{
			const uint8_t attrib = Noesis::TypeForAttr[i];
			const auto&[count, type, isNormalized] = SemanticTypeLUT[attrib];

			layout.add(SemanticAttributeLUT[i], count, type, isNormalized, false);
		}
	}

	layout.end();
}

void BGFXRenderDevice::SetTextures(const Noesis::Batch& batch)
{
	auto set { [i = -1, this](const bgfx::UniformHandle& handler, Noesis::Texture* texture, const Noesis::SamplerState& samplerState) mutable noexcept
	{
		++i;
		if (texture)
		{
			auto* const instance { dynamic_cast<BGFXTexture*>(texture) };
			bgfx::setTexture(i, handler, instance->handle, GetSamplerFlags(samplerState));
		}
	}};

	set(textureUniforms[0], batch.pattern, batch.patternSampler);
	set(textureUniforms[1], batch.ramps, batch.rampsSampler);
	set(textureUniforms[2], batch.image, batch.imageSampler);
	set(textureUniforms[3], batch.glyphs, batch.glyphsSampler);
	set(textureUniforms[4], batch.shadow, batch.shadowSampler);
}

void BGFXRenderDevice::SetUniforms(const Noesis::Batch& batch)
{
	constexpr auto set { [](bgfx::UniformHandle& uniform, const Noesis::UniformData& data) noexcept
	{
		if (data.values)
		{
			bgfx::UniformInfo info{};
			bgfx::getUniformInfo(uniform, info);
			bgfx::setUniform(uniform, data.values, info.num);
		}
	} };

	set(uniforms[0], batch.vertexUniforms[1]);
	set(uniforms[1], batch.pixelUniforms[0]);
	set(uniforms[1], batch.pixelUniforms[1]);
}

uint32_t BGFXRenderDevice::GetWrapTMode(const Noesis::SamplerState& sampler)
{
	switch (sampler.f.wrapMode)
	{
		case Noesis::WrapMode::Enum::ClampToEdge:       return BGFX_SAMPLER_V_CLAMP;
		case Noesis::WrapMode::Enum::ClampToZero:       return BGFX_SAMPLER_V_BORDER;
		case Noesis::WrapMode::Enum::Repeat:            return 0;
		case Noesis::WrapMode::Enum::MirrorU:           return 0;
		case Noesis::WrapMode::Enum::MirrorV:           return BGFX_SAMPLER_V_MIRROR;
		case Noesis::WrapMode::Enum::Mirror:            return BGFX_SAMPLER_V_MIRROR;
		default:                                        return 0;
	}
}

uint32_t BGFXRenderDevice::GetWrapSMode(const Noesis::SamplerState& sampler)
{
	switch (sampler.f.wrapMode)
	{
		case Noesis::WrapMode::Enum::ClampToEdge:   return BGFX_SAMPLER_U_CLAMP;
		case Noesis::WrapMode::Enum::ClampToZero:   return BGFX_SAMPLER_U_BORDER;
		case Noesis::WrapMode::Enum::Repeat:        return 0;
		case Noesis::WrapMode::Enum::MirrorU:       return BGFX_SAMPLER_U_MIRROR;
		case Noesis::WrapMode::Enum::MirrorV:       return 0;
		case Noesis::WrapMode::Enum::Mirror:        return BGFX_SAMPLER_U_MIRROR;
		default:                                    return 0;
	}
}

uint32_t BGFXRenderDevice::GetMinMode(const Noesis::SamplerState& sampler)
{
	switch (sampler.f.mipFilter)
	{
		case Noesis::MipFilter::Disabled:       return 0;
		case Noesis::MipFilter::Nearest:        return BGFX_SAMPLER_MIN_POINT;
		case Noesis::MipFilter::Linear:         return BGFX_SAMPLER_MIN_ANISOTROPIC;
		default:                                return 0;
	}
}

uint32_t BGFXRenderDevice::GetMagMode(const Noesis::SamplerState& sampler)
{
	switch (sampler.f.minmagFilter)
	{
		case Noesis::MinMagFilter::Nearest:     return BGFX_SAMPLER_MAG_POINT;
		case Noesis::MinMagFilter::Linear:      return BGFX_SAMPLER_MAG_ANISOTROPIC;
		default:                                return 0;
	}
}

uint32_t BGFXRenderDevice::GetSamplerFlags(const Noesis::SamplerState& sampler)
{
	return GetWrapSMode(sampler) | GetWrapTMode(sampler) | GetMinMode(sampler) | GetMagMode(sampler);
}

void BGFXRenderDevice::CreatePrograms()
{
	for (int i = 0; i < vertexShadersSource.size(); ++i) vertexShaders[i] = LoadShader(vertexShadersSource[i]);
	for (int i = 0; i < fragmentShadersSource.size(); ++i) fragmentShaders[i] = LoadShader(fragmentShadersSource[i]);

	for (int i = 0; i < fragmentShadersSource.size(); ++i)
	{
		const auto vertexShader = vertexShaders[Noesis::VertexForShader[i]];
		const auto fragmentShader = fragmentShaders[i];

		// I think it's false because the same vertex shader is used for multiple fragment shaders
		// Not going to test because I clean up manually so who cares
		programs[i] = bgfx::createProgram(vertexShader, fragmentShader, false);
	}
}

bgfx::ShaderHandle BGFXRenderDevice::LoadShader(const std::string& shaderName) const
{
	std::ifstream file("../BGFXRenderDevice/Src/Shaders/" + shaderName + (GetCaps().linearRendering ? "_SRGB" : "") + ".bin", std::ios::binary | std::ios::ate);

	if (!file.is_open())
	{
		return BGFX_INVALID_HANDLE;
	}

	size_t fileSize = file.tellg();
	file.seekg(0, std::ios::beg);

	char* data = new char[fileSize];
	file.read(data, fileSize);
	file.close();

	const bgfx::Memory* const mem = bgfx::copy(data, fileSize);
	delete[] data;

	auto handle = bgfx::createShader(mem);
	bgfx::setName(handle, shaderName.c_str());

	return handle;
}

//TODO: PASS INVERTED TOO IF INVERTED/OPENGL?
BGFXRenderTarget::BGFXRenderTarget(uint32_t width, uint32_t height) : width(width), height(height), texture(*new BGFXTexture(bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::RGBA8, BGFX_TEXTURE_RT), width, height, 4, false))
{
}

BGFXRenderTarget::~BGFXRenderTarget()
{
	if (bgfx::isValid(fboHandle)) bgfx::destroy(fboHandle);
	if (bgfx::isValid(stencilHandle)) bgfx::destroy(stencilHandle);
}

Noesis::Texture* BGFXRenderTarget::GetTexture()
{
	return texture;
}

BGFXTexture::BGFXTexture(bgfx::TextureHandle handle, uint32_t width, uint32_t height, size_t stride, bool inverted)
		: handle(handle), width(width), height(height), stride(stride), inverted(inverted)
{
}

BGFXTexture::~BGFXTexture()
{
	bgfx::destroy(handle);
}

uint32_t BGFXTexture::GetWidth() const
{
	return width;
}

uint32_t BGFXTexture::GetHeight() const
{
	return height;
}

bool BGFXTexture::HasMipMaps() const
{
	return false;
}

bool BGFXTexture::IsInverted() const
{
	return inverted;
}

bool BGFXTexture::HasAlpha() const
{
	return true;
}