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
ut *label;
long long  vertices,edges;
ut *edge;
char *mmap_index;
char *mmap_edges;
struct cmp{
    bool operator()(const int &a,const int &b){
        if(label[a]>label[b]) return 1;
        return 0;
    }
};
priority_queue<int,vector<int>,cmp>que;
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
	
/*
	edge=(TimeEdge *)malloc(sizeof(TimeEdge)*edges);
	for(ut i=0;i<edges;i++){
		TimeEdge * p = (TimeEdge *)(buffers +  i * sizeof(TimeEdge));
        edge[i]=*p;
		//printf("%d %d\n",edges[i].a,edges[i].b);
	}*/

	edge=(ut *)malloc(3*sizeof(ut)*edges);
    for(long long i=0;i<3*edges;i++){
        ut *p=(ut *)(buffers+i*sizeof(ut));
        edge[i]=*p;
    }
	printf("Read time: %.2lf seconds\n", get_time() - start_time);
}
void init_vertice(string input_vertice){
/* 	int index_fd = open(input_vertice.c_str(), O_RDONLY);
    mmap_index = (char *)mmap(0, sizeof(ut) * vertices, PROT_WRITE, MAP_PRIVATE, index_fd, 0);
    label = (ut *)mmap_index;
    close(index_fd);
    madvise(mmap_index, sizeof(ut) *vertices, MADV_WILLNEED)
	*/
	label=(ut *)malloc(sizeof(ut)*(vertices+10));
	for(int i=0;i<=vertices;i++) label[i]=2000000000;
}
int main(int argc,char **argv)
{
	cmdline::parser cmd;
	cmd.add<std::string>("input", 'i', "input file", true, "");
	cmd.add<int>("source", 's', "number of levels", true, 0);
	cmd.add<int>("end", 'e', "number of levels", true, 0);
	cmd.add<int>("num", 'k', "number of levels", true, 0);

	cmd.parse_check(argc, argv);
	string input = cmd.get<string>("input");
	ut source = cmd.get<int>("source");
	ut end = cmd.get<int>("end");
    ut k=cmd.get<int>("num");

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
	for(long long  i=0;i<3*edges;i+=3){
		int no=label[edge[i]]+edge[i+2];
//        label[edge[i].v]=min(label[edge[i].v],no);
		if(label[edge[i+1]]>no){
			label[edge[i+1]]=no;
		}
	}
    double no_time=get_time();
    int no_size=0;
    for(int i=0;i<vertices;i++){
        if(no_size<k){
            que.push(i);
            no_size++;
        }
        else{
            que.pop();
            que.push(i);
        }
    }
	double end_time=get_time();
    printf("top-k time: %lf\n",end_time-no_time);
	printf("result & time : %d %lf\n",label[end],end_time-start_time);
	return 0;
}
