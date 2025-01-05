set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_INSTALL OFF)

add_subdirectory(${ROOT_DIR}/3rdparty/glfw)
set_property(TARGET "glfw" PROPERTY FOLDER "3rdparty")
set_property(TARGET "update_mappings" PROPERTY FOLDER "3rdparty")