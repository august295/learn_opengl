set(ProjectName "02basic_lighting")
CreateTarget(${ProjectName} "Exe" "02lighting")

target_link_libraries(${ProjectName} "opengl32")

include_directories(${ROOT_DIR}/3rdparty/glfw/include)
target_link_libraries(${ProjectName} "glfw")

include_directories(${ROOT_DIR}/3rdparty/glad/include)
target_link_libraries(${ProjectName} "glad")

include_directories(${ROOT_DIR}/3rdparty/stb)

include_directories(${ROOT_DIR}/3rdparty/glm)
target_link_libraries(${ProjectName} "glm::glm")
