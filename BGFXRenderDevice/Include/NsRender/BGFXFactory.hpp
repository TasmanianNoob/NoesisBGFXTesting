//
// Created by charl on 31/10/2023.
//

#ifndef NOESISTESTING_BGFXFACTORY_HPP
#define NOESISTESTING_BGFXFACTORY_HPP

#include "BGFXRenderDeviceApi.hpp"

#include <NsCore/Noesis.h>
#include <NsRender/RenderDevice.h>

#include "bgfx/bgfx.h"
#include "GLFW/glfw3.h"

struct NS_RENDER_BGFXRENDERDEVICE_API BGFXFactory
{
	static Noesis::Ptr<Noesis::RenderDevice> CreateDevice(GLFWwindow* window, bgfx::ViewId viewId, bool sRGB = false);
};


#endif //NOESISTESTING_BGFXFACTORY_HPP
