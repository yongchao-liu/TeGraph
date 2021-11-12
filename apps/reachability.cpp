/*************************************************************************
    > File Name: preprocess.cpp
    > Author: huanzhizun
    > Mail: huznzhiun888@126.com 
    > Created Time: Fri 09 Mar 2018 03:32:33 PM CST
 ************************************************************************/
#include "src/core.hpp"
#include<queue>
using namespace std;
int concurrency=1;
bool *label;
long long vertices,edges;
/*
struct TimeEdge {
	ut u, v;
};
TimeEdge *edge;*/
ut *edge;
char *mmap_index;
char *mmap_edges;
void init_edge(string edge_file_name){
/*	ut edge_fd = open(edge_file_name.c_str(), O_RDONLY);
    ut edges_size = sizeof(TimeEdge);
    mmap_edges = (char *)mmap(0, edges_size * edges, PROT_READ, MAP_PRIVATE, edge_fd, 0);
    edge = (TimeEdge *)mmap_edges;
    close(edge_fd);
    madvise(mmap_edges, edges_size * edges, MADV_WILLNEED);
*/
	long bytes = file_size(edge_file_name);
	int fd = open(edge_file_name.c_str(), O_RDONLY);
	posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL);
	char * buffers = (char *)mmap(0, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	double start_time = get_time();
	long read_bytes = 0;
	while(true) {
		long r_bytes = read(fd, buffers + read_bytes, IOBUFFER);
		if (r_bytes <= 0) break;
		read_bytes += r_bytes;
	}

	close(fd);
	assert(read_bytes == bytes);
	
	//Begin Operation
//	int w=fscanf(fp,"%lld",&edge);
//	printf("%d\n",w);
//	cout<<vertice<<" "<<edge<<endl;

//	fflush(stdout);
/*
	edge=(TimeEdge *)malloc(sizeof(TimeEdge)*edges);
	for(ut i=0;i<edges;i++){
		TimeEdge * p = (TimeEdge *)(buffers +  i * sizeof(TimeEdge));
        edge[i]=*p;
		//printf("%d %d\n",edges[i].a,edges[i].b);
	}*/

	edge=(ut *)malloc(2*sizeof(ut)*edges);
    for(long long i=0;i<2*edges;i++){
        ut *p=(ut *)(buffers+i*sizeof(ut));
        edge[i]=*p;
    }
	printf("Read time: %.2lf seconds\n", get_time() - start_time);
}
void init_vertice(string input_vertice){
 	/*int index_fd = open(input_vertice.c_str(), O_RDONLY);
    mmap_index = (char *)mmap(0, sizeof(bool) * vertices, PROT_WRITE, MAP_PRIVATE, index_fd, 0);
    label = (bool *)mmap_index;
    close(index_fd);
    madvise(mmap_index, sizeof(bool) *vertices, MADV_WILLNEED);
	*/
	label=(bool *)malloc(sizeof(bool)*(vertices+10));
}
int main(int argc,char **argv)
{
	cmdline::parser cmd;
	cmd.add<std::string>("input", 'i', "input file", true, "");
	cmd.add<int>("source", 's', "number of levels", true, 0);
	cmd.add<int>("end", 'e', "number of levels", true, 0);

	cmd.parse_check(argc, argv);
	string input = cmd.get<string>("input");
	ut source = cmd.get<int>("source");
	ut end = cmd.get<int>("end");

	printf("Begin \n");
	string input_edge=input+"-edge.info";
	string input_vertice=input+"-vertice.info";
	string input_info=input+".info";
	int fin = open(input_info.c_str(), O_RDONLY);
	if(read(fin, (char*)&vertices, sizeof(ut)) != sizeof(ut)) {
		printf("Format error\n");
		exit(-1);
	}
	if(read(fin, (char*)&edges, sizeof(ut)) != sizeof(ut)) {
		printf("Format error\n");
		exit(-1);
	}
	printf("%d %d\n",vertices,edges);
	init_edge(input_edge);
	init_vertice(input_vertice);
	label[source]=1;
	double start_time = get_time();
    /*
	for(int i=0;i<edges;i++){
        //label[edge[i].v]|=label[edge[i].u];
		if(label[edge[i].u]){
			label[edge[i].v]=1;
		}
	}*/
    for(long long i=0;i<2*edges;i=i+2){
        label[edge[i+1]]|=label[edge[i]];
//        if(label[edge[i]]){
  //          label[edge[i+1]]=1;
    //    }
    }
	printf("result & time : %d %lf\n",label[end],get_time()-start_time);
	return 0;
}
