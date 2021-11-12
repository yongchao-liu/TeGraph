/*************************************************************************
    > File Name: preprocess.cpp
    > Author: huanzhizun
    > Mail: huznzhiun888@126.com 
    > Created Time: Fri 09 Mar 2018 03:32:33 PM CST
 ************************************************************************/
#include "src/core.hpp"
#include<queue>
using namespace std;
int concurrency=16;
long long *label;
ut vertices,edges;
/*
struct TimeEdge {
	ut u, v;
	ut s;
};
TimeEdge *edge;
*/
ut* edge;
char *mmap_index;
char *mmap_edges;
long long mmap_edges_size;
int *deg;
void init_edge(string edge_file_name){
	ut edge_fd = open(edge_file_name.c_str(), O_RDONLY);
    struct stat sb;
    fstat(edge_fd, &sb);
    mmap_edges_size = sb.st_size;
    mmap_edges = (char *)mmap(0, mmap_edges_size, PROT_READ, MAP_PRIVATE, edge_fd, 0);
    edge = (ut *)mmap_edges;
    close(edge_fd);
    madvise(mmap_edges, mmap_edges_size, MADV_WILLNEED);
}
void init_vertice(string input_vertice){
	label=(long long *)malloc(sizeof(long long)*(vertices+10));
	for(int i=0;i<=vertices;i++) label[i]=2000000000000;
	deg=(ut *)malloc(sizeof(ut)*(vertices+10));
	double start_time = get_time();
	long bytes = file_size(input_vertice);
	int fd = open(input_vertice.c_str(), O_RDONLY);
	posix_fadvise(fd, 0, 0, POSIX_FADV_SEQUENTIAL);
	char * buffers = (char *)mmap(0, bytes, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	long read_bytes = 0;
	while(true) {
		long r_bytes = read(fd, buffers + read_bytes, IOBUFFER);
		if (r_bytes <= 0) break;
		read_bytes += r_bytes;
	}
	close(fd);
	assert(read_bytes == bytes);
    for(int i=0;i<vertices;i++){
        ut *p=(ut *)(buffers+(i)*sizeof(ut));
        deg[i+1]=*p;
    }
	printf("read vertex: %lf\n",get_time()-start_time);
    for(int i=1;i<=10;i++) printf("%d ",deg[i]);
    printf("fuck\n");
}
queue<int>q;
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
	double start_time = get_time();
	label[source]=0;
	#pragma omp parallel for schedule(static, concurrency)  
	for(long long i=0;i<edges;i++){
        int a=edge[3*i],b=edge[3*i+1],c=edge[3*i+2];
        while(deg[a]>0){
            
        }
        #pragma omp atomic write
        label[b]=min(label[b],label[a]+c);
        #pragma omp atomic
        deg[b]--;
	}
	double end_time=get_time();
	printf("result & time : %lld %lf\n",label[end],end_time-start_time);
return 0;
}
