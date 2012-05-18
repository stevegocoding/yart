set(TARGET test_intersect)
project(${TARGET})

set(SRCS test_ray_intersect.cpp)

add_definitions(-DUNICODE)

link_directories(${CMAKE_BINARY_DIR}/lib)

add_executable(${TARGET} ${SRCS})
set_target_properties(${TARGET} PROPERTIES DEBUG_POSTFIX "_debug")
target_link_libraries(${TARGET} debug libyart_d.lib)
target_link_libraries(${TARGET} optimized libyart.lib)
target_link_libraries(${TARGET} debug assimp_d.lib)
target_link_libraries(${TARGET} optimized assimp.lib)

# Installation
install(TARGETS ${TARGET} RUNTIME DESTINATION bin/${TARGET})

install(TARGETS ${TARGET} RUNTIME DESTINATION bin/${TARGET})
install(FILES ${FX_FILES} DESTINATION bin/${TARGET})
install(DIRECTORY ${DEP_DLL_DEBUG_DIR} ${DEP_DLL_REL_DIR} DESTINATION bin/${TARGET})