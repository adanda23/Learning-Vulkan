cmake_minimum_required(VERSION 3.20)
project(VulkanApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)  # Generates compile_commands.json for clangd

# Glob all .cpp files in the project root
file(GLOB SOURCES "*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

# Vulkan
find_package(Vulkan REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE Vulkan::Vulkan)

# Optional: uncomment if you're using GLFW for windowing
find_package(glfw3 REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

# Optional: uncomment if you're using GLM for math
# find_package(glm REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
