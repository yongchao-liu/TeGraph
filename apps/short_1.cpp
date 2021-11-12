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
ut * label;
struct cmp{
    bool operator()(const int &a,const int &b){
        if(label[a]>label[b]) return 1;
        return 0;
    }
};
priority_queue<int,vector<int>,cmp>que;
const int inf=1000000000;
int concurrency=1;
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
	edge=(ut *)malloc(4*sizeof(ut)*edges);
    label=(ut *)malloc(sizeof(ut)*(vertices+10));
    for(int i=0;i<=vertices;i++){
        s1 ss;
        ss.clear();
        g.push_back(ss);
        label[i]=inf;
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

    ut *p=(ut *)(buffers);
    vertices=*p;
    p=(ut *)(buffers+sizeof(ut));
    edges=*p;
    init();
    for(long long i=0;i<4*edges;i++){
        p=(ut *)(buffers+(i+2)*sizeof(ut));
        edge[i]=*p;
    }
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
    ut k = cmd.get<int>("num"); 

	printf("Begin \n");
	string input_edge=input;
//	string input_vertice=input+"-vertice.info";
//	string input_info=input+".info";
/*
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
	}*/
	double start_time = get_time();
	init_edge(input_edge);
	printf("%d %d %d %d\n",vertices,edges,source);
	//init_vertice(input_vertice);
    start_time = get_time();
    double in_t=0,er_t=0;
    double ee=0;
    int n1=0,n2=0,nn=0;
    for(int i=0;i<edges;i++){
        ut u,v,t,b;
        u=edge[4*i];
        v=edge[4*i+1];
        t=edge[4*i+2];
        b=edge[4*i+3];
        if(u==source){
            if(g[u].find(make_pair(t,0))==g[u].end()){
                g[u].insert(make_pair(t,0));
            }
            label[u]=0;
        }
        if(g[u].begin()==g[u].end()) continue;
//        double no_time=get_time();
        it=g[u].lower_bound(make_pair(t,inf));
  //      ee+=get_time()-no_time;
        nn++;
        if(it==g[u].begin()) continue;
        it--;
        ut up=it->second;
        up+=b-t;
        if(up<label[v]) label[v]=up;
        it=g[v].lower_bound(make_pair(v,0));
        if(it==g[v].end()){
            if(it!=g[v].begin()){
                it--;
                if(it->second<=up){
                    continue;
                }
            }
            g[v].insert(make_pair(v,up));
        }
        else{
            if(it->first==t){
                if(it->second<=up){
                    continue;
                }
            }
            else{
                if(it!=g[v].begin()){
                    it--;
                    if(it->second<=up) continue;
                    it++;
                }
            }
            it1=it;
            while(it1!=g[v].end()){
                if(it1->second>=up){
                    it1++;
                }
                else break;
            }
            if(it!=it1){
                g[v].erase(it,it1);
            }
            g[v].insert(make_pair(v,up));
        }  
    //    er_t+=get_time()-no_time;
    }
	double end_time=get_time();
	printf("result & time : %lf\n",end_time-start_time);
/*    long long sum=0;
    for(int i=1;i<=vertices;i++){
        sum+=2;
    }
    printf("close time : %lf\n",get_time()-end_time);
    end_time=get_time();
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
    printf("top-k time : %lf\n",get_time()-end_time);
*/	return 0;
}
