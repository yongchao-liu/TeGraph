/*************************************************************************
    > File Name: preprocess.cpp
    > Author: huanzhizun
    > Mail: huznzhiun888@126.com 
    > Created Time: Fri 09 Mar 2018 03:32:33 PM CST
 ************************************************************************/
#include "src/core.hpp"
#include<queue>
#include<vector>
using namespace std;
const int inf=1000000000;
int concurrency=1;
int *dis;
int *head;
bool *vis;
ut vertices,edges;

struct TimeEdge {
	int v;  
    int w;
	int next;
};
struct cmp{
    int u,w;
    //cmp(int a,int b): u(a),w(b){};
    friend bool operator<(const cmp &a,const cmp &b){
        if(a.w>b.w) return 1;
        return 0;
    }
};
priority_queue<cmp>que;
queue<ut>qq;
TimeEdge *edge;
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
	printf("Read time: %.2lf seconds\n", get_time() - start_time);
	
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

	edge=(TimeEdge *)malloc(sizeof(TimeEdge)*edges);
    for(long long i=0;i<edges;i++){
        ut u,v,w;
        ut *p=(ut *)(buffers+3*i*sizeof(ut));
        u=*p;
        p=(ut *)(buffers+(3*i+1)*sizeof(ut));
        v=*p;
        p=(ut *)(buffers+(3*i+2)*sizeof(ut));
        w=*p;
        if(w<0) w=-w;
        edge[i].v=v;    
        edge[i].w=w;
        edge[i].next=head[u];
        head[u]=i;
    }
}
void init_vertice(string input_vertice){
/* 	int index_fd = open(input_vertice.c_str(), O_RDONLY);
    mmap_index = (char *)mmap(0, sizeof(ut) * vertices, PROT_WRITE, MAP_PRIVATE, index_fd, 0);
    label = (ut *)mmap_index;
    close(index_fd);
    madvise(mmap_index, sizeof(ut) *vertices, MADV_WILLNEED)
	*/
	dis=(int *)malloc(sizeof(int)*(vertices+1));
//	qq=(int *)malloc(sizeof(int)*(vertices+1));
	head=(int *)malloc(sizeof(int)*(vertices+1));
//	vis=(bool *)malloc(sizeof(bool)*(vertices+1));
	for(int i=0;i<=vertices;i++){
        dis[i]=inf;
        head[i]=-1;
  //      vis[i]=0;
    }
}
vector<ut>gg;
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
    /*
    ut source1;
	if(read(fin, (char*)&source, sizeof(ut)) != sizeof(ut)) {
		printf("Format error\n");
		exit(-1);
	}
	if(read(fin, (char*)&source1, sizeof(ut)) != sizeof(ut)) {
		printf("Format error\n");
		exit(-1);
	}*/
	printf("%d %d %d %d\n",vertices,edges,source,end);
	init_vertice(input_vertice);
	init_edge(input_edge);
    int no=0,all=0;
    /*
    for(int i=source;i<=source;i++){
        dis[i]=0;
        cmp p1;
        p1.u=i;
        p1.w=0;
        que.push(p1);
    }*/
    /*
    dis[source]=0;
    vis[source]=1;
    qq.push(source);
	double start_time = get_time();
    while(!qq.empty()){
        ut p=qq.front();
        qq.pop();
        vis[p]=0;
        for(int i=head[p];i!=-1;i=edge[i].next){
            if(dis[edge[i].v]>dis[p]+edge[i].w){
                dis[edge[i].v]=dis[p]+edge[i].w;
                if(!vis[edge[i].v]){
                    qq.push(edge[i].v);
                    vis[edge[i].v]=1;
                }
            } 
        }
    }*/
    dis[source]=0;
    cmp p1;
    p1.u=source;
    p1.w=0;
    que.push(p1);
	double start_time = get_time();
    while(!que.empty()){
        cmp p=que.top();
        que.pop();
        if(dis[p.u]!=p.w){
            continue;
        }
        for(int i=head[p.u];i!=-1;i=edge[i].next){
            if(dis[edge[i].v]>dis[p.u]+edge[i].w){
                dis[edge[i].v]=dis[p.u]+edge[i].w;
                p1.u=edge[i].v;
                p1.w=dis[edge[i].v];
                que.push(p1);
            }
        }
    }
	printf("result & time : %d %lf\n",dis[end],get_time()-start_time);
	return 0;
}
