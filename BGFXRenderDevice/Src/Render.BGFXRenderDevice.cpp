//
// Created by charl on 31/10/2023.
//

#include <NsCore/Package.h>
#include "BGFXRenderDevice.hpp"
#include "../Include/NsRender/BGFXFactory.hpp"

Noesis::Ptr<Noesis::RenderDevice> BGFXFactory::CreateDevice(GLFWwindow* window, bgfx::ViewId viewId, bool sRGB)
{
	return *new BGFXRenderDevice(window, viewId, sRGB);
}

NS_BEGIN_COLD_REGION

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_REGISTER_REFLECTION(Render, BGFXRenderDevice)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_INIT_PACKAGE(Render, BGFXRenderDevice)
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////
NS_SHUTDOWN_PACKAGE(Render, BGFXRenderDevice)
{
}

NS_END_COLD_REGION