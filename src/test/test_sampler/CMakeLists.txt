set(TARGET test_sampler)
project(${TARGET})

set(SRCS test_sampler.cpp dummy_integrator.h)

if(WIN32)
  set(SRCS ${SRCS} minimal.rc)
endif(WIN32)

add_definitions(-DUNICODE)

include(${wxWidgets_USE_FILE})

link_directories(${CMAKE_BINARY_DIR}/lib)

message("hahaha ${YART_CORE_TARGET}")
add_executable(${TARGET} WIN32 ${SRCS})
#add_dependencies(${TARGET} ${LIB_TARGET})
set_target_properties(${TARGET} PROPERTIES DEBUG_POSTFIX "_debug")

target_link_libraries(${TARGET} debug libyart_d.lib ${wxWidgets_LIBRARIES})
target_link_libraries(${TARGET} optimized libyart.lib ${wxWidgets_LIBRARIES})

# Installation
install(TARGETS ${TARGET} RUNTIME DESTINATION bin/${TARGET})