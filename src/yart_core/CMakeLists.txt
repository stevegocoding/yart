set(TARGET yart)

set(FILM_DIR film)
set(GEO_DIR geometry)
set(INTEGRATORS_DIR integrators)
set(LIGHTS_DIR lights)
set(RENDERERS_DIR renderers)

# Dependencies
set(DEP_LIBS_DEBUG
	gtestd_dll.lib
	)

set(DEP_LIBS_RELEASE
	gtest_dll.lib
	)

set(CORE_FILES
	pch.h
	prerequisites.h
	rt_main.cpp
	camera.h
	camera.cpp
	film.h
	geometry.h
	geometry.cpp
	transform.h
	transform.cpp
	sampler.h
	)
set(FILM_FILES		
	film_image.h
	film_image.cpp
	)
set(GEO_FILES
	sphere.h
	sphere.cpp
	)
set(INTEGRATORS_FILES
	whitted.h
	whitted.cpp
	)
set(LIGHT_FILES
	point_light.h
	point_light.cpp
	)
set(RENDERER_FILES
	sampler_renderer.h
	sampler_renderer.cpp
	)

add_definitions(-DUNICODE)

source_group("core" FILES ${CORE_FILES})
source_group("film" FILES ${FILM_FILES})
source_group("geometry" FILES ${GEO_FILES})
source_group("integrators" FILES ${INTEGRATORS_FILES})
source_group("lights" FILES ${LIGHT_FILES})
source_group("renderers" FILES ${RENDERER_FILES})

set(SRC_FILES 
	${CORE_FILES} 
	${FILM_FILES} 
	${GEO_FILES} 
	${INTEGRATORS_FILES} 
	${LIGHT_FILES} 
	${RENDERER_FILES} 
	)

add_executable(${TARGET} WIN32 ${SRC_FILES})
SET_TARGET_PROPERTIES(${TARGET} PROPERTIES DEBUG_POSTFIX "_debug")
	
target_link_libraries(${TARGET} debug "${DEP_LIBS_DEBUG}" optimized "${DEP_LIBS_RELEASE}")

# Installation
install(TARGETS ${TARGET} RUNTIME DESTINATION bin/${TARGET})
install(DIRECTORY ${DEP_DLL_DEBUG_DIR} ${DEP_DLL_REL_DIR} DESTINATION bin/${TARGET})
