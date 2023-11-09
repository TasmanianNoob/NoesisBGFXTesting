@echo off

:: Vertex Shaders

CALL :shaderc Pos shader vertex
CALL :shaderc PosColor shader vertex "HAS_COLOR=1"
CALL :shaderc PosTex0 shader vertex "HAS_UV0=1"
CALL :shaderc PosTex0Rect shader vertex "HAS_UV0=1;HAS_RECT=1"
CALL :shaderc PosTex0RectTile shader vertex "HAS_UV0=1;HAS_RECT=1;HAS_TILE=1"
CALL :shaderc PosColorCoverage shader vertex "HAS_COLOR=1;HAS_COVERAGE=1"
CALL :shaderc PosTex0Coverage shader vertex "HAS_UV0=1;HAS_COVERAGE=1"
CALL :shaderc PosTex0CoverageRect shader vertex "HAS_UV0=1;HAS_COVERAGE=1;HAS_RECT=1"
CALL :shaderc PosTex0CoverageRectTile shader vertex "HAS_UV0=1;HAS_COVERAGE=1;HAS_RECT=1;HAS_TILE=1"
CALL :shaderc PosColorTex1_SDF shader vertex "HAS_COLOR=1;HAS_UV1=1;SDF=1"
CALL :shaderc PosTex0Tex1_SDF shader vertex "HAS_UV0=1;HAS_UV1=1;SDF=1"
CALL :shaderc PosTex0Tex1Rect_SDF shader vertex "HAS_UV0=1;HAS_UV1=1;HAS_RECT=1;SDF=1"
CALL :shaderc PosTex0Tex1RectTile_SDF shader vertex "HAS_UV0=1;HAS_UV1=1;HAS_RECT=1;HAS_TILE=1;SDF=1"
CALL :shaderc PosColorTex1 shader vertex "HAS_COLOR=1;HAS_UV1=1"
CALL :shaderc PosTex0Tex1 shader vertex "HAS_UV0=1;HAS_UV1=1"
CALL :shaderc PosTex0Tex1Rect shader vertex "HAS_UV0=1;HAS_UV1=1;HAS_RECT=1"
CALL :shaderc PosTex0Tex1RectTile shader vertex "HAS_UV0=1;HAS_UV1=1;HAS_RECT=1;HAS_TILE=1"
CALL :shaderc PosColorTex0Tex1 shader vertex "HAS_COLOR=1;HAS_UV0=1;HAS_UV1=1"
CALL :shaderc PosTex0Tex1_Downsample shader vertex "HAS_UV0=1;HAS_UV1=1;DOWNSAMPLE=1"
CALL :shaderc PosColorTex1Rect shader vertex "HAS_COLOR=1;HAS_UV1=1;HAS_RECT=1"
CALL :shaderc PosColorTex0RectImagePos shader vertex "HAS_COLOR=1;HAS_UV0=1;HAS_RECT=1;HAS_IMAGE_POSITION=1"

:: DOES NOTHING RIGHT NOW

CALL :shaderc PosColor_SRGB shader vertex "HAS_COLOR=1;SRGB=1"
CALL :shaderc PosColorCoverage_SRGB shader vertex "HAS_COLOR=1;HAS_COVERAGE=1;SRGB=1"
CALL :shaderc PosColorTex1_SDF_SRGB shader vertex "HAS_COLOR=1;HAS_UV1=1;SDF=1;SRGB=1"
CALL :shaderc PosColorTex1_SRGB shader vertex "HAS_COLOR=1;HAS_UV1=1;SRGB=1"
CALL :shaderc PosColorTex0Tex1_SRGB shader vertex "HAS_COLOR=1;HAS_UV0=1;HAS_UV1=1;SRGB=1"
CALL :shaderc PosColorTex1Rect_SRGB shader vertex "HAS_COLOR=1;HAS_UV1=1;HAS_RECT=1;SRGB=1"
CALL :shaderc PosColorTex0RectImagePos_SRGB shader vertex "HAS_COLOR=1;HAS_UV0=1;HAS_RECT=1;HAS_IMAGE_POSITION=1;SRGB=1"

:: Fragment Shaders

CALL :shaderc RGBA shader fragment "EFFECT_RGBA=1"
CALL :shaderc Mask shader fragment "EFFECT_MASK=1"
CALL :shaderc Clear shader fragment "EFFECT_CLEAR=1"

CALL :shaderc Path_Solid shader fragment "EFFECT_PATH=1;PAINT_SOLID=1"
CALL :shaderc Path_Linear shader fragment "EFFECT_PATH=1;PAINT_LINEAR=1"
CALL :shaderc Path_Radial shader fragment "EFFECT_PATH=1;PAINT_RADIAL=1"
CALL :shaderc Path_Pattern shader fragment "EFFECT_PATH=1;PAINT_PATTERN=1"

CALL :shaderc Path_Pattern_Clamp shader fragment "EFFECT_PATH=1;PAINT_PATTERN=1;CLAMP_PATTERN=1"
CALL :shaderc Path_Pattern_Repeat shader fragment "EFFECT_PATH=1;PAINT_PATTERN=1;REPEAT_PATTERN=1"
CALL :shaderc Path_Pattern_MirrorU shader fragment "EFFECT_PATH=1;PAINT_PATTERN=1;MIRRORU_PATTERN=1"
CALL :shaderc Path_Pattern_MirrorV shader fragment "EFFECT_PATH=1;PAINT_PATTERN=1;MIRRORV_PATTERN=1"
CALL :shaderc Path_Pattern_Mirror shader fragment "EFFECT_PATH=1;PAINT_PATTERN=1;MIRROR_PATTERN=1"

CALL :shaderc Path_AA_Solid shader fragment "EFFECT_PATH_AA=1;PAINT_SOLID=1"
CALL :shaderc Path_AA_Linear shader fragment "EFFECT_PATH_AA=1;PAINT_LINEAR=1"
CALL :shaderc Path_AA_Radial shader fragment "EFFECT_PATH_AA=1;PAINT_RADIAL=1"
CALL :shaderc Path_AA_Pattern shader fragment "EFFECT_PATH_AA=1;PAINT_PATTERN=1"

CALL :shaderc Path_AA_Pattern_Clamp shader fragment "EFFECT_PATH_AA=1;PAINT_PATTERN=1;CLAMP_PATTERN=1"
CALL :shaderc Path_AA_Pattern_Repeat shader fragment "EFFECT_PATH_AA=1;PAINT_PATTERN=1;REPEAT_PATTERN=1"
CALL :shaderc Path_AA_Pattern_MirrorU shader fragment "EFFECT_PATH_AA=1;PAINT_PATTERN=1;MIRRORU_PATTERN=1"
CALL :shaderc Path_AA_Pattern_MirrorV shader fragment "EFFECT_PATH_AA=1;PAINT_PATTERN=1;MIRRORV_PATTERN=1"
CALL :shaderc Path_AA_Pattern_Mirror shader fragment "EFFECT_PATH_AA=1;PAINT_PATTERN=1;MIRROR_PATTERN=1"

CALL :shaderc SDF_Solid shader fragment "EFFECT_SDF=1;PAINT_SOLID=1"
CALL :shaderc SDF_Linear shader fragment "EFFECT_SDF=1;PAINT_LINEAR=1"
CALL :shaderc SDF_Radial shader fragment "EFFECT_SDF=1;PAINT_RADIAL=1"
CALL :shaderc SDF_Pattern shader fragment "EFFECT_SDF=1;PAINT_PATTERN=1"

CALL :shaderc SDF_Pattern_Clamp shader fragment "EFFECT_SDF=1;PAINT_PATTERN=1;CLAMP_PATTERN=1"
CALL :shaderc SDF_Pattern_Repeat shader fragment "EFFECT_SDF=1;PAINT_PATTERN=1;REPEAT_PATTERN=1"
CALL :shaderc SDF_Pattern_MirrorU shader fragment "EFFECT_SDF=1;PAINT_PATTERN=1;MIRRORU_PATTERN=1"
CALL :shaderc SDF_Pattern_MirrorV shader fragment "EFFECT_SDF=1;PAINT_PATTERN=1;MIRRORV_PATTERN=1"
CALL :shaderc SDF_Pattern_Mirror shader fragment "EFFECT_SDF=1;PAINT_PATTERN=1;MIRROR_PATTERN=1"

CALL :shaderc SDF_LCD_Solid shader fragment "EFFECT_SDF_LCD=1;PAINT_SOLID=1"
CALL :shaderc SDF_LCD_Linear shader fragment "EFFECT_SDF_LCD=1;PAINT_LINEAR=1"
CALL :shaderc SDF_LCD_Radial shader fragment "EFFECT_SDF_LCD=1;PAINT_RADIAL=1"
CALL :shaderc SDF_LCD_Pattern shader fragment "EFFECT_SDF_LCD=1;PAINT_PATTERN=1"

CALL :shaderc SDF_LCD_Pattern_Clamp shader fragment "EFFECT_SDF_LCD=1;PAINT_PATTERN=1;CLAMP_PATTERN=1"
CALL :shaderc SDF_LCD_Pattern_Repeat shader fragment "EFFECT_SDF_LCD=1;PAINT_PATTERN=1;REPEAT_PATTERN=1"
CALL :shaderc SDF_LCD_Pattern_MirrorU shader fragment "EFFECT_SDF_LCD=1;PAINT_PATTERN=1;MIRRORU_PATTERN=1"
CALL :shaderc SDF_LCD_Pattern_MirrorV shader fragment "EFFECT_SDF_LCD=1;PAINT_PATTERN=1;MIRRORV_PATTERN=1"
CALL :shaderc SDF_LCD_Pattern_Mirror shader fragment "EFFECT_SDF_LCD=1;PAINT_PATTERN=1;MIRROR_PATTERN=1"

CALL :shaderc Opacity_Solid shader fragment "EFFECT_OPACITY=1;PAINT_SOLID=1"
CALL :shaderc Opacity_Linear shader fragment "EFFECT_OPACITY=1;PAINT_LINEAR=1"
CALL :shaderc Opacity_Radial shader fragment "EFFECT_OPACITY=1;PAINT_RADIAL=1"
CALL :shaderc Opacity_Pattern shader fragment "EFFECT_OPACITY=1;PAINT_PATTERN=1"

CALL :shaderc Opacity_Pattern_Clamp shader fragment "EFFECT_OPACITY=1;PAINT_PATTERN=1;CLAMP_PATTERN=1"
CALL :shaderc Opacity_Pattern_Repeat shader fragment "EFFECT_OPACITY=1;PAINT_PATTERN=1;REPEAT_PATTERN=1"
CALL :shaderc Opacity_Pattern_MirrorU shader fragment "EFFECT_OPACITY=1;PAINT_PATTERN=1;MIRRORU_PATTERN=1"
CALL :shaderc Opacity_Pattern_MirrorV shader fragment "EFFECT_OPACITY=1;PAINT_PATTERN=1;MIRRORV_PATTERN=1"
CALL :shaderc Opacity_Pattern_Mirror shader fragment "EFFECT_OPACITY=1;PAINT_PATTERN=1;MIRROR_PATTERN=1"

CALL :shaderc Upsample shader fragment "EFFECT_UPSAMPLE=1"
CALL :shaderc Downsample shader fragment "EFFECT_DOWNSAMPLE=1"
CALL :shaderc Shadow shader fragment "EFFECT_SHADOW=1;PAINT_SOLID=1"
CALL :shaderc Blur shader fragment "EFFECT_BLUR=1;PAINT_SOLID=1"

EXIT /B 0

:shaderc
    if exist %1.bin del %1.bin
    shaderc.exe -f %2.%3 -o %1.bin --platform windows --profile s_5_0 --type %3 -i %CD% --verbose --define %4
    if %errorlevel% neq 0 EXIT /B %errorlevel%
    EXIT /B 0