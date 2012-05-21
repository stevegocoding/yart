set(TARGET test_wxwidget)
project(test_wxwidget)

set(SRC_FILES
	wx_yart_app.h 
	wx_yart_app.cpp)

if(WIN32)
  set(SRC_FILES ${SRC_FILES} minimal.rc)
endif(WIN32)

add_definitions(-DUNICODE)

include(${wxWidgets_USE_FILE})

link_directories(${CMAKE_BINARY_DIR}/lib)

add_executable(${TARGET} WIN32 ${SRC_FILES})
set_target_properties(${TARGET} PROPERTIES DEBUG_POSTFIX "_debug")

target_link_libraries(${TARGET} debug libyart_d.lib ${wxWidgets_LIBRARIES})
target_link_libraries(${TARGET} optimized libyart.lib ${wxWidgets_LIBRARIES})
	
# Installation
install(TARGETS ${TARGET} RUNTIME DESTINATION bin/${TARGET})
install(DIRECTORY ${DEP_DLL_DEBUG_DIR} ${DEP_DLL_REL_DIR} DESTINATION bin/${TARGET})