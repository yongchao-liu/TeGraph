/*************************************************************************
    > File Name: preprocess.cpp
    > Author: huanzhizun
    > Mail: huznzhiun888@126.com 
    > Created Time: Fri 09 Mar 2018 03:32:33 PM CST
 ************************************************************************/
#include "src/core.hpp"
#include<queue>
using namespace std;
const int inf=1000000000;
int concurrency=1;
bool *label;
int *head,*en;
int *que;
int *deg;
int *ans;
ut vertices,edges;

struct TimeEdge {
	int u, v;
    int w;
};
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
        ut u,v;
        ut *p=(ut *)(buffers+3*i*sizeof(ut));
        u=*p;
        edge[i].u=u;
        p=(ut *)(buffers+(3*i+1)*sizeof(ut));
        v=*p;
        edge[i].v=v;
        p=(ut *)(buffers+(3*i+2)*sizeof(ut));
        v=*p;
        edge[i].w=v;
    }
}
void init_vertice(string input_vertice){
/* 	int index_fd = open(input_vertice.c_str(), O_RDONLY);
    mmap_index = (char *)mmap(0, sizeof(ut) * vertices, PROT_WRITE, MAP_PRIVATE, index_fd, 0);
    label = (ut *)mmap_index;
    close(index_fd);
    madvise(mmap_index, sizeof(ut) *vertices, MADV_WILLNEED)
	*/
	label=(bool *)malloc(sizeof(bool)*(vertices+1));
	head=(int *)malloc(sizeof(int)*(vertices+1));
	en=(int *)malloc(sizeof(int)*(vertices+1));
	que=(int *)malloc(sizeof(int)*(vertices+1));
	deg=(int *)malloc(sizeof(int)*(vertices+1));
	ans=(int *)malloc(sizeof(int)*(vertices+1));
}
int cmp(TimeEdge a,TimeEdge b){
    if(a.u!=b.u)
    return a.u<b.u;
    return a.v<b.v;
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
	}/*
    ut source1;
	if(read(fin, (char*)&source, sizeof(ut)) != sizeof(ut)) {
		printf("Format error\n");
		exit(-1);
	}
	if(read(fin, (char*)&source1, sizeof(ut)) != sizeof(ut)) {
		printf("Format error\n");
		exit(-1);
	}*/
	printf("%d %d\n",vertices,edges);
	init_vertice(input_vertice);
	init_edge(input_edge);
    sort(edge,edge+edges,cmp);
	double start_time = get_time();
	for(int i=0;i<=vertices;i++){
        label[i]=0;
        deg[i]=0;
        head[i]=-1;
    }
    for(int i=0;i<edges;i++){
        if(head[edge[i].u]==-1){
            head[edge[i].u]=i;
        }
        en[edge[i].u]=i;
        deg[edge[i].v]++;
    }/*
    for(int i=0;i<edges;i++){
        edge[i].next=head[edge[i].u];
        head[edge[i].u]=i;
    }*/
    int no=0,all=0;
/*    for(int i=1;i<=vertices;i++){
        printf("%d %d\n",head[i],en[i]);
    }*/
/*    for(int i=edges-10;i<edges;i++){
        printf("%d %d\n",edge[i].u,edge[i].v);
    }*/
    for(int i=0;i<=vertices;i++){
        ans[i]=1000000000;
    }
    double dd=0;
    for(int j=0;j<=vertices;j++){
        if(deg[j]==0){
            que[all++]=j;
            ans[j]=0;
        }
    }
        while(no!=all){
            ut p=que[no++];
            if(label[p]) continue;
            label[p]=1;
//            for(int i=head[p];i!=-1;i=edge[i].next){
            for(int i=head[p];i!=-1&&i<=en[p];i++){
                ans++;
                if(!label[edge[i].v]){
                    //label[edge[i].v]=1;
                    deg[edge[i].v]--;
                    ans[edge[i].v]=min(ans[edge[i].v],ans[edge[i].u]+edge[i].w);
                    if(deg[edge[i].v]==0) que[all++]=edge[i].v;
                }
            }
        }
    printf("%d %d %lf\n",all,ans,dd);
	printf("result & time : %d %lf\n",label[end],get_time()-start_time);
	return 0;
}
