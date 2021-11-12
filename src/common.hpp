#ifndef COMMON_H
#define COMMON_H
#include <thread>

#include "src/type.hpp"

inline int get_concurrency(){
	return std::thread::hardware_concurrency();
	//return 8;
}

inline int get_partition_id(const VertexNum vertices, const int partitions, const VertexID vertex_id ){
	int partition_size = vertices / partitions;
	int partition_just = partition_size * partitions;
	int partition_rest = vertices % partition_just;

	return vertex_id < partition_just ? vertex_id / partition_size : vertex_id / partition_size - 1;
}

inline std::pair<VertexID, VertexID> get_partition_range(const VertexNum vertices, const int partitions, const int partition_id){
	int partition_size = vertices / partitions;

	VertexID source = partition_id * partition_size;
	VertexID target = (partition_id + 1) * partition_size - 1;
	if (partition_id == partitions - 1) target = vertices - 1;

	return std::make_pair(source, target);
}

inline int get_partition_vertices(const VertexNum vertices, const int partitions, const int partition_id){
	int partition_size = vertices / partitions;
	if ( partition_id != partitions - 1) return partition_size;
	else return partition_size + (vertices - partition_size * partitions);
}

#endif
