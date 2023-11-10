cmake_minimum_required(VERSION 3.20)
project(NoesisTesting)

set(CMAKE_CXX_STANDARD 23)

add_executable(NoesisTesting main.cpp
        BGFXRenderDevice/Include/NsRender/BGFXFactory.hpp
        BGFXRenderDevice/Include/NsRender/BGFXRenderDeviceApi.hpp
        BGFXRenderDevice/Src/BGFXRenderDevice.cpp
        BGFXRenderDevice/Src/BGFXRenderDevice.hpp
        BGFXRenderDevice/Src/Render.BGFXRenderDevice.cpp)

# glfw
find_package(glfw3 CONFIG REQUIRED)

# glm
find_package(glm CONFIG REQUIRED)

# bgfx
find_package(bgfx CONFIG REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE bgfx::bx bgfx::bgfx bgfx::bimg bgfx::bimg_decode glm::glm glfw)

# noesisgui
set(NOESIS_DIR "C:/dev/NoesisGUI")
target_include_directories(${PROJECT_NAME} PUBLIC ${NOESIS_DIR}/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${NOESIS_DIR}/Src/Packages/)
target_include_directories(${PROJECT_NAME} PUBLIC ${NOESIS_DIR}/Src/Packages/App/ApplicationLauncher/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${NOESIS_DIR}/Src/Packages/App/Display/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${NOESIS_DIR}/Src/Packages/App/DisplayLauncher/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${NOESIS_DIR}/Src/Packages/App/Interactivity/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${NOESIS_DIR}/Src/Packages/App/LangServer/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${NOESIS_DIR}/Src/Packages/App/Launcher/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${NOESIS_DIR}/Src/Packages/App/MediaElement/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${NOESIS_DIR}/Src/Packages/App/Providers/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${NOESIS_DIR}/Src/Packages/App/Rive/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${NOESIS_DIR}/Src/Packages/App/RiveBase/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${NOESIS_DIR}/Src/Packages/App/Shaders/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${NOESIS_DIR}/Src/Packages/App/Theme/Include)
target_include_directories(${PROJECT_NAME} PUBLIC ${NOESIS_DIR}/Src/Packages/Render/RenderContext/Include)

if(WIN32)
    find_library(Noesis NAMES "Noesis" HINTS "${NOESIS_DIR}/Lib/windows_x86_64" REQUIRED)
    find_library(NoesisApp NAMES "NoesisApp" HINTS "${NOESIS_DIR}/Lib/windows_x86_64" REQUIRED)

    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${NOESIS_DIR}/Bin/windows_x86_64/Noesis.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )
    add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy
            "${NOESIS_DIR}/Bin/windows_x86_64/NoesisApp.dll"
            $<TARGET_FILE_DIR:${PROJECT_NAME}>
    )

    target_link_libraries(${PROJECT_NAME} PUBLIC ${Noesis} ${NoesisApp})
elseif(UNIX AND NOT APPLE)
    find_library(Noesis NAMES "libNoesis.so" HINTS "${NOESIS_DIR}/Bin/linux_x86_64" REQUIRED)
    find_library(NoesisApp NAMES "libNoesisApp.so" HINTS "${NOESIS_DIR}/Bin/linux_x86_64" REQUIRED)

    target_link_libraries(${PROJECT_NAME} PUBLIC ${Noesis} ${NoesisApp})
endif()
message("Linked Noesis, NoesisApp.")

