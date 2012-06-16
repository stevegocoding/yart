#include "kd_accel.h"

struct kd_node 
{
public:
	void init_leaf(uint32_t *prim_indices, int np, MemoryArena& mem_arena)
	{
		flags = 3;		// 0x11 = leaf node
		num_prims |= (np << 2);		

		// Store primitives ids for leaf node
		if (np == 0)
			one_prim = 0;
		else if (np == 1)
			one_prim = prim_indices[0];
		else 
		{
			prims = mem_arena.Alloc<uint32_t>(np);
			std::copy(prim_indices, prim_indices+np, prims); 
		}
	}
	
	void init_interior(uint32_t axis, uint32_t a_child, float s)
	{
		split = s;
		flags = axis; 
		above_child |= a_child << 2; 
	}

	float get_split_pos() const 
	{
		return split; 
	}
	
	uint32_t get_num_prims() const 
	{
		return num_prims >> 2; 
	}
	
	uint32_t get_split_axis() const 
	{
		return flags & 0x11; 
	}
	
	bool is_leaf() const 
	{
		return (flags & 0x11) == 3;
	}
	
	uint32_t get_above_child() const 
	{
		return above_child >> 2;
	}
	
	union 
	{
		float split;			// split plane for interior node 
		uint32_t one_prim;		// leaf node 
		uint32_t *prims;		// leaf node
	};

private:
	union 
	{
		/** 
		flags:
			lower 2 bits : axis of split, 00 - x, 01 - y, 10 - z, 11 - leaf node
			upper 30 bits : number of primitives if it is a leaf node
		*/
		uint32_t flags;			// both 
		uint32_t num_prims;		// leaf
		uint32_t above_child;   // interior 
	};
};

struct bound_edge 
{
	bound_edge() : t(0), prim_idx(0) {}
	bound_edge(float _t, int np, bool starting) 
		: t(_t)
		, prim_idx(np)
		, type(starting? e_start : e_end)
	{}

	bool operator < (const bound_edge& e) const 
	{
		if (t == e.t)
			return (int)type < (int)e.type; 
		else 
			return t < e.t; 
	}

	float t;
	int prim_idx; 
	enum {e_start, e_end} type;
};

//////////////////////////////////////////////////////////////////////////

c_kdtree_accel::c_kdtree_accel(const scene_prims_array& scene_prims, 
	int isect_cost /* = 80 */, 
	int trav_cost/* = 1 */, 
	float ebonus /* = 0.5f */, 
	int max_prims /* = 1 */,
	int max_depth /* = -1 */)
	: m_isect_cost(isect_cost)
	, m_travesal_cost(trav_cost)
	, m_max_prims(max_prims)
	, m_max_depth(max_depth)
	, m_empty_bonus(ebonus)
	, m_next_free_node(0)
	, m_num_alloced_nodes(0)
{
	m_scene_primitives = scene_prims; 

	// Compute the max depth
	if (max_depth < 0)
		m_max_depth = round2int(8 + 1.3f * log2int(float(scene_prims.size())));
	
	// Compute the bounding box for kd-tree construction
	std::vector<c_aabb> prims_bounds; 
	prims_bounds.reserve(scene_prims.size());
	for (uint32_t i = 0; i < scene_prims.size(); ++i)
	{
		c_aabb b  = scene_prims[i]->get_world_aabb();
		m_bounds = union_aabb(m_bounds, b);
		prims_bounds.push_back(b);
	}

	// Allocate working memory for kd-tree construction
	bound_edge *edges[3];				// bound edges for 3 axis
	for (int i = 0; i < 3; ++i)
		edges[i] = new bound_edge[2*scene_prims.size()];
	
	uint32_t *prims0 = new uint32_t[scene_prims.size()]; 
	uint32_t *prims1 = new uint32_t[(m_max_depth+1) * scene_prims.size()]; 
	
	// Initialize _primNums_ for kd-tree construction
	uint32_t *prims_indices = new uint32_t[scene_prims.size()];
	for (uint32_t i = 0; i < scene_prims.size(); ++i)
		prims_indices[i] = i;

	build_tree(0, m_bounds, prims_bounds, prims_indices, scene_prims.size(), m_max_depth, edges, prims0, prims1);
	
	// Free working memory for kd-tree construction	
	
	delete[] prims_indices; 
	for (int i = 0; i < 3; ++i)
		delete[] edges[i]; 
	delete[] prims1;
	delete[] prims0; 
}

c_kdtree_accel::~c_kdtree_accel()
{
	FreeAligned(m_nodes);
}

bool c_kdtree_accel::intersects(const c_ray& ray, c_intersection *isect) const 
{
	return false; 
}

void c_kdtree_accel::build_tree(int node_idx, 
	const c_aabb& cur_bounds, 
	const std::vector<c_aabb>& prim_bounds, 
	uint32_t *prims_indices, 
	int num_prims, 
	int depth, 
	bound_edge *edges[3], 
	uint32_t *prims0, uint32_t *prims1,
	int bad_refines)
{
	assert(node_idx == m_next_free_node);

	// Get next free node from _nodes_ array
	if (m_next_free_node == m_num_alloced_nodes)
	{
		int n_alloc = max(2 * m_num_alloced_nodes, 512); 
		kd_node *new_nodes_buf = AllocAligned<kd_node>(n_alloc);
		if (n_alloc > 0)
		{
			std::copy(m_nodes, m_nodes+m_num_alloced_nodes, new_nodes_buf);
			FreeAligned(m_nodes); 
		}
		m_nodes = new_nodes_buf; 
		m_num_alloced_nodes = n_alloc; 
	}
	++m_next_free_node;

	// Create leaf node if termination criteria met
	if (num_prims <= m_max_prims || depth == 0)
	{
		m_nodes[node_idx].init_leaf(prims_indices, num_prims, m_mem_arena);
		return;
	}
	
	// Initialize interior node and continue recursion
	
	// Choose split axis position for interior node
	int best_axis = -1; 
	int best_offset = -1; 
	float best_cost = INFINITY; 
	float old_cost = m_isect_cost * float(num_prims);
	float total_sa = cur_bounds.surface_area();
	float inv_total_sa = 1.0f / total_sa; 
	vector3f d = cur_bounds.pt_max - cur_bounds.pt_min;
	
	// Choose which axis to split along
	uint32_t axis = cur_bounds.max_extend();
	int retries = 0; 
	
	// Try sweeping each axis to find an optimal split
	do 
	{
		// Initialize and sort the edges on the current axis
		for (int i = 0; i < num_prims; ++i)
		{
			int prim_idx = prims_indices[i]; 
			const c_aabb& box = prim_bounds[prim_idx]; 
			edges[axis][2*i] = bound_edge(box.pt_min[axis], prim_idx, true);
			edges[axis][2*i+1] = bound_edge(box.pt_max[axis], prim_idx, false); 
		}
		std::sort(&edges[axis][0], &edges[axis][2*num_prims]);
		
		// Compute the cost of all splits for current axis to find best
		int num_below = 0; 
		int num_above = num_prims;
		for (int i = 0; i < 2 * num_prims; ++i)
		{
			if (edges[axis][i].type == bound_edge::e_end) 
				-- num_above; 
			float edge_pos = edges[axis][i].t; 
			if (edge_pos > cur_bounds.pt_min[axis] && edge_pos < cur_bounds.pt_max[axis])
			{
				// Compute the cost at ith edge
				uint32_t other_axis0 = (axis + 1) % 3; 
				uint32_t other_axis1 = (axis + 2) % 3; 
				
				float below_sa = 2 * (d[other_axis0] * d[other_axis1] + 
									  (edge_pos - cur_bounds.pt_min[axis]) * (d[other_axis0] + d[other_axis1]));
				float above_sa = 2 * (d[other_axis0] * d[other_axis1] + 
									  (cur_bounds.pt_max[axis] - edge_pos) * (d[other_axis0] + d[other_axis1]));
				float p_below = below_sa * inv_total_sa; 
				float p_above = above_sa * inv_total_sa; 
				float eb = (num_above == 0 || num_below == 0) ? m_empty_bonus : 0.0f; 
				float cost = m_travesal_cost + m_isect_cost * (1.0f - eb) * (p_below * num_below + p_above * num_above); 
				
				// Update the best cost split so far
				if (cost < best_cost)
				{
					best_cost = cost; 
					best_axis = axis; 
					best_offset = i; 
				}
			}
			if (edges[axis][i].type == bound_edge::e_start)
				++ num_below; 
		}
		assert(num_below == num_prims && num_above == 0); 
		
		if (best_axis != -1) 
			break; 
		else 
			axis = (axis + 1) % 3; 
		
	}while(retries++ < 2); 

	if (best_cost > old_cost)
		++ bad_refines; 
	if (best_cost > 4.0f * old_cost && num_prims < 16 || best_axis == -1 || bad_refines == 3)
	{
		m_nodes[node_idx].init_leaf(prims_indices, num_prims, m_mem_arena); 
		return; 
	}
	
	// Classify primitives with respect to split
	int n0 = 0, n1 = 0; 
	
	for (int i = 0; i < best_offset; ++i)
	{
		if (edges[best_axis][i].type == bound_edge::e_start)
			prims0[n0++] = edges[best_axis][i].prim_idx;
	}
	for (int i = best_offset+1; i < 2 * num_prims; ++i)
	{
		if (edges[best_axis][i].type == bound_edge::e_end)
			prims1[n1++] = edges[best_axis][i].prim_idx;
	}

	// Recursively initialize children nodes
	float split_pos = edges[best_axis][best_offset].t; 
	c_aabb bounds0 = cur_bounds; 
	c_aabb bounds1 = cur_bounds; 
	bounds0.pt_max[best_axis] = bounds1.pt_min[best_axis] = split_pos; 
	build_tree(node_idx+1, bounds0, prim_bounds, prims0, n0, depth-1, edges, prims0, prims1+num_prims, bad_refines); 
	
	uint32_t above_child = m_next_free_node;
	m_nodes[node_idx].init_interior(best_axis, above_child, split_pos);
	build_tree(above_child, bounds1, prim_bounds, prims1, n1, depth-1, edges, prims0, prims1+num_prims, bad_refines);
}

