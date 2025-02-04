cmake_minimum_required(VERSION 3.12)

####################################################################################################
# glad
####################################################################################################
add_subdirectory(${ROOT_DIR}/3rdparty/glad)
set_property(TARGET "glad" PROPERTY FOLDER "3rdparty")

####################################################################################################
# glfw
####################################################################################################
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_INSTALL OFF)

add_subdirectory(${ROOT_DIR}/3rdparty/glfw)
set_property(TARGET "glfw" PROPERTY FOLDER "3rdparty")
set_property(TARGET "update_mappings" PROPERTY FOLDER "3rdparty")

####################################################################################################
# glm
####################################################################################################
add_subdirectory(${ROOT_DIR}/3rdparty/glm)
set_property(TARGET "glm" PROPERTY FOLDER "3rdparty")

####################################################################################################
# imgui
####################################################################################################
# 基础代码
set(IMGUI_DIR "${ROOT_DIR}/3rdparty/imgui")
include_directories(${IMGUI_DIR})
set(HEADER_FILES
    ${IMGUI_DIR}/imconfig.h
    ${IMGUI_DIR}/imgui_internal.h
    ${IMGUI_DIR}/imgui.h
    ${IMGUI_DIR}/imstb_rectpack.h
    ${IMGUI_DIR}/imstb_textedit.h
    ${IMGUI_DIR}/imstb_truetype.h
)
set(SOURCE_FILES
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui.cpp
)

# 框架代码
option(IMGUI_WITH_GLFW "imgui with glfw" ON)
option(IMGUI_WITH_OPENGL3 "imgui with opengl3" ON)
if(IMGUI_WITH_GLFW)
    include_directories(${ROOT_DIR}/3rdparty/glfw/include)
    list(APPEND HEADER_FILES ${IMGUI_DIR}/backends/imgui_impl_glfw.h)
    list(APPEND SOURCE_FILES ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp)
endif()
if(IMGUI_WITH_OPENGL3)
    list(APPEND HEADER_FILES ${IMGUI_DIR}/backends/imgui_impl_opengl3.h ${IMGUI_DIR}/backends/imgui_impl_opengl3_loader.h)
    list(APPEND SOURCE_FILES ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp)
endif()

# 文件分类
if(CMAKE_CXX_PLATFORM_ID MATCHES "Windows")
    source_group(TREE ${IMGUI_DIR} PREFIX "Header Files" FILES ${HEADER_FILES})
    source_group(TREE ${IMGUI_DIR} PREFIX "Source Files" FILES ${SOURCE_FILES})
elseif(CMAKE_CXX_PLATFORM_ID MATCHES "Linux")
    source_group("Header Files" FILES ${HEADER_FILES})
    source_group("Source Files" FILES ${SOURCE_FILES})
endif()

add_library("imgui" ${HEADER_FILES} ${SOURCE_FILES})

target_link_libraries("imgui" "opengl32")
target_link_libraries("imgui" "glfw")

set_property(TARGET "imgui" PROPERTY FOLDER "3rdparty")

####################################################################################################
# assimp
####################################################################################################
set(BUILD_SHARED_LIBS ON)
set(ASSIMP_BUILD_TESTS OFF)
set(ASSIMP_INSTALL OFF)

add_subdirectory(assimp)

set_property(TARGET "assimp" PROPERTY FOLDER "3rdparty")
set_property(TARGET "zlibstatic" PROPERTY FOLDER "3rdparty")
set_property(TARGET "UpdateAssimpLibsDebugSymbolsAndDLLs" PROPERTY FOLDER "3rdparty")