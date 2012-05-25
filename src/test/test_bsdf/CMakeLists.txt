set(TARGET test_bsdf)
project(${TARGET})

set(SRCS test_bsdf.cpp)

add_definitions(-DUNICODE)

link_directories(${CMAKE_BINARY_DIR}/lib)

add_executable(${TARGET} ${SRCS})
set_target_properties(${TARGET} PROPERTIES DEBUG_POSTFIX "_debug")
target_link_libraries(${TARGET} debug libyart_d.lib)
target_link_libraries(${TARGET} optimized libyart.lib)

target_link_libraries(${TARGET} debug libboost_thread-vc100-mt-gd-1_49.lib )
target_link_libraries(${TARGET} optimized libboost_thread-vc100-mt-1_49.lib)


# Installation
install(TARGETS ${TARGET} RUNTIME DESTINATION bin/${TARGET})