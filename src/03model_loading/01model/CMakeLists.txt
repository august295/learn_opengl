set(ProjectName "01model")
CreateTarget(${ProjectName} "Exe" "03model_loading")

target_link_libraries(${ProjectName} "opengl32")

include_directories(${ROOT_DIR}/3rdparty/glfw/include)
target_link_libraries(${ProjectName} "glfw")

include_directories(${ROOT_DIR}/3rdparty/glad/include)
target_link_libraries(${ProjectName} "glad")

include_directories(${ROOT_DIR}/3rdparty/stb)

include_directories(${ROOT_DIR}/3rdparty/glm)
target_link_libraries(${ProjectName} "glm::glm")

include_directories(${ROOT_DIR}/3rdparty/imgui)
include_directories(${ROOT_DIR}/3rdparty/imgui/backends)
target_link_libraries(${ProjectName} "imgui")

include_directories(${ROOT_DIR}/3rdparty/assimp/include)
target_link_libraries(${ProjectName} "assimp")
