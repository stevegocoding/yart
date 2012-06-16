#pragma once

#include "prerequisites.h"
#include "accel_structure.h"
#include "memory.h"

struct kd_node;
struct bound_edge;

class c_kdtree_accel : public c_accel_structure
{

public:
	c_kdtree_accel(const scene_prims_array& scene_prims, 
		int isect_cost = 80, int trav_cost = 1, float ebonus = 0.5f, int max_prims = 1, int max_depth = -1);

	~c_kdtree_accel();
	
	bool intersects(const c_ray& ray, c_intersection *isect) const;
	
	
private:

	void build_tree(int node_idx, 
		const c_aabb& cur_bounds, 
		const std::vector<c_aabb>& prim_bounds, 
		uint32_t *prims_indices, 
		int num_prims, 
		int depth, 
		bound_edge *edges[3], 
		uint32_t *prims0, 
		uint32_t *prims1,
		int bad_refines = 0); 
	
	int m_max_depth;
	int m_max_prims;
	int m_isect_cost; 
	int m_travesal_cost; 
	float m_empty_bonus;
	
	int m_next_free_node, m_num_alloced_nodes; 
	
	kd_node *m_nodes;
	c_aabb m_bounds; 

	MemoryArena m_mem_arena;
}; 