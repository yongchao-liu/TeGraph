/*************************************************************************
    > File Name: preprocess.cpp
    > Author: huanzhizun
    > Mail: huznzhiun888@126.com 
    > Created Time: Fri 09 Mar 2018 03:32:33 PM CST
 ************************************************************************/
#include "src/core.hpp"
#include<queue>
#include<set>
using namespace std;
typedef pair<ut,ut>p1;
typedef set<p1> s1;
vector<s1 >g;
set<p1>::iterator it,it1;
const int inf=1000000000;
int concurrency=1;
ut *label,*label1,*dis;
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
void init(){
    label=(ut *)malloc(sizeof(ut)*vertices);
    label1=(ut *)malloc(sizeof(ut)*vertices);
    dis=(ut *)malloc(sizeof(ut)*vertices);
	edge=(ut *)malloc(3*sizeof(ut)*edges);
    for(int i=0;i<vertices;i++){
        s1 ss;
        ss.clear();
        g.push_back(ss);
        dis[i]=inf;
    }
}
void init_edge(string edge_file_name){
/*	ut edge_fd = open(edge_file_name.c_str(), O_RDONLY);
    ut edges_size = sizeof(TimeEdge);
    mmap_edges = (char *)mmap(0, edges_size * edges, PROT_READ, MAP_PRIVATE, edge_fd, 0);
    edge = (TimeEdge *)mmap_edges;
    close(edge_fd);
    madvise(mmap_edges, edges_size * edges, MADV_WILLNEED);
*/
    double start_time=get_time();
	long bytes = file_size(edge_file_name);
	int fd = open(edge_file_name.c_str(), O_RDONLY);
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

    for(long long i=0;i<3*edges;i++){
        ut *p=(ut *)(buffers+i*sizeof(ut));
        edge[i]=*p;
    }
}
void init_vertice(string input_vertice){
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
        ut *p=(ut *)(buffers+2*i*sizeof(ut));
        label1[i]=*p;
        p=(ut *)(buffers+(2*i+1)*sizeof(ut));
        label[i]=*p;

    }
    /*
 	int index_fd = open(input_vertice.c_str(), O_RDONLY);
    mmap_index = (char *)mmap(0, sizeof(ut) * vertices, PROT_WRITE, MAP_PRIVATE, index_fd, 0);
    label = (ut *)mmap_index;
    close(index_fd);
    madvise(mmap_index, sizeof(ut) *vertices, MADV_WILLNEED)
*/
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
    ut source1;
	if(read(fin, (char*)&source, sizeof(ut)) != sizeof(ut)) {
		printf("Format error\n");
		exit(-1);
	}
	if(read(fin, (char*)&source1, sizeof(ut)) != sizeof(ut)) {
		printf("Format error\n");
		exit(-1);
	}
	printf("%d %d %d %d\n",vertices,edges,source,source1);
    init();
	double start_time = get_time();
	init_vertice(input_vertice);
	init_edge(input_edge);
    for(ut i=source;i<=source1;i++){
        dis[i]=-label[i];
        g[label1[i]].insert(make_pair(label[i],-label[i]));
    }
    start_time = get_time();
    for(long long i=0;i<edges;i++){
        ut u,v;
        u=edge[3*i];
        v=edge[3*i+1];
        if(dis[u]==inf) continue;
        it=g[label1[u]].lower_bound(make_pair(label[u],-inf));
        ut up=0;
        if(it==g[label1[u]].end()){
            it--;
            up=it->second;
        }
        else if(it==g[label1[u]].begin()){
            if(it->first>label[u]) continue;
            else {
                up=it->second;
            }
        }
        else{
            if(it->first>label[u]){
                it--;
                up=it->second;
            }
            else{
                up=it->second;
            }
        }
        if(dis[v]==inf||dis[v]>up){
            dis[v]=up;
            it=g[label1[v]].lower_bound(make_pair(label[v],-inf));
            if(it==g[label1[v]].end()){
                g[label1[v]].insert(make_pair(label[v],dis[v]));
            }
            else{
                if(it->first==label[v]){
                    g[label1[v]].erase(it);
                    g[label1[v]].insert(make_pair(label[v],dis[v]));
                }
                else{
                    if(it!=g[label1[v]].begin()){
                        it--;
                        if(it->second<=dis[v]){
                            continue;
                        }
                        it++;
                    }
                    it1=it;
                    while(it1!=g[label1[v]].end()){
                        if(it1->second>=dis[v]){
                            it1++;
                        }
                        else break;
                    }
                    if(it!=it1){
                        g[label1[v]].erase(it,it1);
                    }
                    g[label1[v]].insert(make_pair(label[v],dis[v]));
                }
            }
        }
    }
	double end_time=get_time();
	printf("result & time : %d %lf\n",label[end],end_time-start_time);
	return 0;
}
