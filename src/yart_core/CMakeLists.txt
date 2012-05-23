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
	assimp_d.lib
	)

set(DEP_LIBS_RELEASE
	gtest_dll.lib
	assimp.lib
	)

set(CORE_FILES
	camera.cpp
	camera.h
	color.cpp
	color.h
	filter.cpp
	filter.h
	geometry.cpp
	geometry.h
	integrator.cpp
	integrator.h
	light.cpp
	light.h
	math_utils.cpp
	math_utils.h
	monte_carlo.cpp
	monte_carlo.h
	pch.h
	prerequisites.h
	ray.cpp
	ray.h
	reflection.cpp
	reflection.h
	render_target.cpp
	render_target.h
	renderer.cpp
	renderer.h
	rng.cpp
	rng.h
	rt_main.cpp
	sampler.cpp
	sampler.h
	scene.cpp
	scene.h
	scene_obj.cpp
	scene_obj.h
	shape.cpp
	shape.h
	transform.cpp
	transform.h
	)
set(GEO_FILES
	sphere.h
	sphere.cpp
	triangle_mesh.h
	triangle_mesh.cpp
	triangle_mesh_impl.h
	triangle_mesh_impl.cpp
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

set(FILTER_FILES
	box_filter.h
	box_filter.cpp
	gaussian_filter.h
	gaussian_filter.cpp
	)

add_definitions(-DUNICODE)

source_group("core" FILES ${CORE_FILES})
source_group("geometry" FILES ${GEO_FILES})
source_group("integrators" FILES ${INTEGRATORS_FILES})
source_group("lights" FILES ${LIGHT_FILES})
source_group("renderers" FILES ${RENDERER_FILES})
source_group("samplers" FILES ${SAMPLER_FILES})
source_group("filters" FILES ${FILTER_FILES})

set(SRC_FILES 
	${CORE_FILES} 
	${GEO_FILES} 
	${INTEGRATORS_FILES} 
	${LIGHT_FILES} 
	${RENDERER_FILES} 
	${SAMPLER_FILES}
	${FILTER_FILES}
	)

add_library(${LIB_TARGET} ${SRC_FILES})
set_target_properties(${LIB_TARGET} PROPERTIES DEBUG_POSTFIX "_d") 
target_link_libraries(${LIB_TARGET} debug "${DEP_LIBS_DEBUG}")
target_link_libraries(${LIB_TARGET} optimized "${DEP_LIBS_RELEASE}")
install(TARGETS ${LIB_TARGET} ARCHIVE DESTINATION lib/)