cmake_minimum_required(VERSION 3.8)

project("MEPTest")

add_executable(${PROJECT_NAME} Source.cpp)

add_subdirectory(MathExpressionParser)
target_link_libraries(${PROJECT_NAME} PUBLIC MathExpressionParser)
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}/MathExpressionParser")

add_subdirectory(doctest)
target_link_libraries(${PROJECT_NAME} PUBLIC doctest)