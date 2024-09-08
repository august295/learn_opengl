set(ProjectName "04textures")
CreateTarget(${ProjectName} "Exe" "01getting_started")

target_link_libraries(${ProjectName} "opengl32")
include_directories(${ROOT_DIR}/3rdparty/glad/include)
include_directories(${ROOT_DIR}/3rdparty/glfw/include)
target_link_libraries(${ProjectName} "glfw" "glad")

include_directories(${ROOT_DIR}/3rdparty/stb)
