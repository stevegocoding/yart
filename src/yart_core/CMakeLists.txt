set(TARGET yart)
set(LIB_TARGET libyart)
set(${YART_CORE_TARGET} ${LIB_TARGET})

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
	math_utils.h
	rt_main.cpp
	camera.h
	camera.cpp
	film.h
    color.h
    color.cpp
	geometry.h
	geometry.cpp
	transform.h
	transform.cpp
	sampler.h
	sampler.cpp
	rng.h
	rng.cpp
	monte_carlo.h
	monte_carlo.cpp
    shape.h
    shape.cpp
	reflection.h
	reflection.cpp
	light.h
	light.cpp
	)
set(FILM_FILES		
	film_image.h
	film_image.cpp
	)
set(GEO_FILES
	sphere.h
	sphere.cpp
	triangle_mesh.h
	triangle_mesh.cpp
	assimp_api_wrap.h
	assimp_api_wrap.cpp
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

set(SAMPLER_FILES
	stratified_sampler.h
	stratified_sampler.cpp
	)

add_definitions(-DUNICODE)

source_group("core" FILES ${CORE_FILES})
source_group("film" FILES ${FILM_FILES})
source_group("geometry" FILES ${GEO_FILES})
source_group("integrators" FILES ${INTEGRATORS_FILES})
source_group("lights" FILES ${LIGHT_FILES})
source_group("renderers" FILES ${RENDERER_FILES})
source_group("samplers" FILES ${SAMPLER_FILES})

set(SRC_FILES 
	${CORE_FILES} 
	${FILM_FILES} 
	${GEO_FILES} 
	${INTEGRATORS_FILES} 
	${LIGHT_FILES} 
	${RENDERER_FILES} 
	${SAMPLER_FILES}
	)

add_library(${LIB_TARGET} ${SRC_FILES})
set_target_properties(${LIB_TARGET} PROPERTIES DEBUG_POSTFIX "_d")
target_link_libraries(${LIB_TARGET} debug "${DEP_LIBS_DEBUG}" optimized "${DEP_LIBS_RELEASE}")
install(TARGETS ${LIB_TARGET} ARCHIVE DESTINATION lib/)

# add_executable(${TARGET} WIN32 ${SRC_FILES})
# Installation
#install(TARGETS ${TARGET} RUNTIME DESTINATION bin/${TARGET})
#install(DIRECTORY ${DEP_DLL_DEBUG_DIR} ${DEP_DLL_REL_DIR} DESTINATION bin/${TARGET})
