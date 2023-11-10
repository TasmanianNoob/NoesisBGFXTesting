# NoesisTesting
# Project Inspiration and Demonstration
I heavily depended on https://github.com/MarioSieg/BGFX_Noesis_Renderer/tree/main and https://github.com/Dakror/noesis-bgfx/tree/master but tried to remove their in-house features (convert to plug and play).
Later worked from https://github.com/kuncarous/nextmu/tree/main as well.

https://github.com/kuncarous/nextmu/tree/main is probably a better implementation and I would recommend using his stuff if this doesn't work.

Example is a conversion of this project https://github.com/Noesis/Tutorials/tree/master/Samples/IntegrationGLUT
You can press F1 to view debug information. No idea if it's useful though.

Here's an example of the intended output (The mouse click artifacts aren't actually there):
https://i.imgur.com/x76BcOz.gif

# Getting Setup
If you're trying to get this to work first make sure that `BGFXRenderDevice/Src/Shaders/build_shaders.bat` is ran before the program to compile the shaders.

If the program crashes immediately hopefully the issue is just you're compiling to the wrong shader language.
Change `--profile s_5_0` to whatever is more appropriate. Use the debugger to check what `bgfx::getRendererType()` is set to. Probably metal on MacOS and spirv on Linux.
Possible profiles:
```
100_es               OpenGL ES Shading Language / WebGL (ESSL)
300_es
310_es
320_es

s_3_0                High-Level Shading Language (HLSL)
s_4_0
s_5_0

metal                Metal Shading Language (MSL)

pssl                 PlayStation Shader Language (PSSL)

spirv                Standard Portable Intermediate Representation - V (SPIR-V)
spirv10-10
spirv13-11
spirv14-11
spirv15-12
spirv16-13

120                  OpenGL Shading Language (GLSL)
130
140
150
330
400
410
420
430
440
```

If the program still crashes then idk lol.

# Other

This project was made in CLion on windows. I use vcpkg for bgfx, glfw3 and glm. There are other packages in vcpkg.json. No idea why; they might be needed. 
NoesisGUI is installed in `C:/dev/NoesisGUI`
