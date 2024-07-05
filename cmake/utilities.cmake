# Code re-use for adding an executable with some standard dependencies.
# See variable definitions in top level CMakeLists.txt file.

macro(glex_add_executable name sources)
    add_executable (
        ${name}
        ${sources}
        ${PROJECT_SHADERS}
        ${PROJECT_CONFIGS}
        ${VENDORS_SOURCES}
    )

    target_link_libraries (
        ${name}
        glfw
        ${GLFW_LIBRARIES}
        ${GLAD_LIBRARIES}
    )

    set_target_properties (
        ${name}
      PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY
        ${CMAKE_BINARY_DIR}/${PROJECT_NAME}
    )
endmacro()
