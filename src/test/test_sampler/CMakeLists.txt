set(TARGET test_sampler)
project(${TARGET})

set(SRCS test_sampler.cpp)

if(WIN32)
  set(SRCS ${SRCS} minimal.rc)
endif(WIN32)

add_definitions(-DUNICODE)

include(${wxWidgets_USE_FILE})

add_executable(${TARGET} WIN32 ${SRCS})
set_target_properties(${TARGET} PROPERTIES DEBUG_POSTFIX "_debug")
target_link_libraries(${TARGET} ${wxWidgets_LIBRARIES})
	
# Installation
install(TARGETS ${TARGET} RUNTIME DESTINATION bin/${TARGET})