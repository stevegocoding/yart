set(TARGET test_camera)
project(${TARGET})

set(SRCS test_camera.cpp)

add_definitions(-DUNICODE)

link_directories(${CMAKE_BINARY_DIR}/lib)

add_executable(${TARGET} ${SRCS})
set_target_properties(${TARGET} PROPERTIES DEBUG_POSTFIX "_debug")
target_link_libraries(${TARGET} debug libyart_d.lib)
target_link_libraries(${TARGET} optimized libyart.lib)

# Installation
install(TARGETS ${TARGET} RUNTIME DESTINATION bin/${TARGET})