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
long long  *label;
long long vertices,edges;
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
void init_edge(string edge_file_name){
	ut edge_fd = open(edge_file_name.c_str(), O_RDONLY);
    struct stat sb;
    fstat(edge_fd, &sb);
    mmap_edges_size = sb.st_size;
    mmap_edges = (char *)mmap(0, mmap_edges_size, PROT_READ, MAP_PRIVATE, edge_fd, 0);
    edge = (ut *)mmap_edges;
    close(edge_fd);
    madvise(mmap_edges, mmap_edges_size, MADV_WILLNEED);
	
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
/*
    for(long long i=0;i<3*edges;i++){
        ut *p=(ut *)(buffers+i*sizeof(ut));
        edge[i]=*p;
    }
	printf("read : %lf\n",get_time()-start_time);
    */
}
void init_vertice(string input_vertice){
/* 	int index_fd = open(input_vertice.c_str(), O_RDONLY);
    mmap_index = (char *)mmap(0, sizeof(ut) * vertices, PROT_WRITE, MAP_PRIVATE, index_fd, 0);
    label = (ut *)mmap_index;
    close(index_fd);
    madvise(mmap_index, sizeof(ut) *vertices, MADV_WILLNEED)
	*/
	label=(long long *)malloc(sizeof(long long)*(vertices+10));
	for(int i=0;i<=vertices;i++) label[i]=2000000000000;
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
	double start_time = get_time();
	label[source]=0;
	for(long long i=0;i<3*edges;i+=3){
		long long no=label[edge[i]]+edge[i+2];
//        label[edge[i].v]=min(label[edge[i].v],no);
		if(label[edge[i+1]]>no){
			label[edge[i+1]]=no;
		}
	}
	double end_time=get_time();
	printf("result & time : %lld %lf\n",label[end],end_time-start_time);
    munmap(edge, mmap_edges_size);
	return 0;
}
