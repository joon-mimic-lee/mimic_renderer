/*
 * Written by Gyuhyun Lee
 */

#ifndef HB_SHARED_WITH_SHADER_H
#define HB_SHARED_WITH_SHADER_H

struct PerFrameData
{
#if INSIDE_METAL_SHADER
    float4x4 proj_view;
#elif INSIDE_VULKAN_SHADER
#else
    alignas(16) m4x4 proj_view;
#endif
};

struct PerObjectData
{
#if INSIDE_METAL_SHADER
    float4x4 model;
    // TODO(gh) should we use packed_float3?
    float3 color;
#elif INSIDE_VULKAN_SHADER
#else 
    alignas(16) m4x4 model;
    alignas(16) v3 color;
#endif
};

// TODO(gh) any way to handle these better?
/*
    TERMINOLOGY
    thread : a single instance of the shader. Includes registers, ID.
    threadgroup : Equivalent to the workgroup. bunch of threads. Only one CU will take the threadgroup and do the work. 
                  For example, two CU cannot split a single threadgroup. Threads in the same thread group shares same thread memory.
    SIMDgroup : Equivalent to wavefront or warp. Threads in the same SIMDgroup will be executed with same instruction set 
                in lock-step. Threads in the same SIMDgroup cannot diverge, and CU cannot switch to individual thread -
                it can only switch to entirely new SIMDgroup to hide latency. When the GPU collects the threads into SIMDgroup,
                it can try to do that based on the divergence(for example, one for the A routine of the if statement
                and another one for the b routine)
*/

#define grass_high_lod_vertex_count 15
#define grass_high_lod_triangle_count 13
#define grass_high_lod_index_count (grass_high_lod_triangle_count*3)
#define grass_high_lod_divide_count 7

#define grass_low_lod_vertex_count 7
#define grass_low_lod_triangle_count 5
#define grass_low_lod_index_count (grass_low_lod_triangle_count*3)
#define grass_low_lod_divide_count 3

// NOTE(gh) The way we setup how many threads there should be in one object threadgroup
// is based on the payload size limit(16kb) and the simd group width(32).
// The threadgruop count per grid will be determined by the thread count, and it's up to the platform layer
// to decide the size of one wavefront.
// TODO(gh) It seems like the minimum thread count should be at least 128, wonder if there's something 
// to do with the command buffer size inside the GPU?, or maybe because
#define object_thread_per_threadgroup_count_x 8
#define object_thread_per_threadgroup_count_y 4

struct GrassObjectFunctionInput
{
#if INSIDE_METAL_SHADER
    packed_float2 floor_left_bottom_p; // in world unit
    packed_float2 one_thread_worth_dim; // In world unit
    // packed_float2 floor_center;
    // packed_float2 floor_half_dim;
#elif INSIDE_VULKAN_SHADER
#else
    alignas(8) v2 floor_left_bottom_p; // in world unit
    alignas(8) v2 one_thread_worth_dim; // in world unit
    // alignas(8) v2 floor_center;
    //alignas(8) v2 floor_half_dim;
#endif
};


#endif
