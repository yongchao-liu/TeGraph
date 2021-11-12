/*************************************************************************
    > File Name: preprocess.cpp
    > Author: huanzhizun
    > Mail: huznzhiun888@126.com 
    > Created Time: Fri 09 Mar 2018 03:32:33 PM CST
 ************************************************************************/
#include  "src/core.hpp"

using namespace std;
typedef pair<int,int>p1;
p1 *all;
ut *rver;
ut *be,*en;
struct pi{
	ut a,b,t_begin,t_end;
};
typedef pair<ut,ut> pr;
queue<ut>que;
pi *edges,*redges;
int cmp(p1 a, p1 b){
	if(a.first!=b.first){
		return a.first<b.first;
	}
	return a.second<b.second;
}
int cmp1(pi a,pi b){
	if(a.t_begin!=b.t_begin){
		return a.t_begin<b.t_begin;
	}
	return a.a<b.a;
}
int find(int a,int b){
	int le=be[a];
	int ri=en[a];
	while(le<=ri){
		int mid=(le+ri)/2;
		if(b<all[mid].second) ri=mid-1;
		else le=mid+1;
	}
	return ri;
}
int main(int argc,char ** argv)
{
	cmdline::parser cmd;
	cmd.add<std::string>("input", 'i', "input file", true, "");
	cmd.add<std::string>("output", 'o', "output file", true, "");

	cmd.parse_check(argc, argv);
	string input = cmd.get<string>("input");
	string output = cmd.get<string>("output");

	long bytes = file_size(input);
	int fd = open(input.c_str(), O_RDONLY);
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
	ut vertice = *(ut *)buffers;
	ut edge = *(ut *)(buffers + sizeof(ut));
	printf("Vertices: %lld, Edges %lld\n", vertice, edge);


//	int w=fscanf(fp,"%lld",&edge);
//	printf("%d\n",w);
//	cout<<vertice<<" "<<edge<<endl;

//	fflush(stdout);
	
	rver=(ut *)malloc((edge*2)*sizeof(ut));
	be=(ut *)malloc((edge*2)*sizeof(ut));
	en=(ut *)malloc((edge*2)*sizeof(ut));
	all=(p1 *)malloc((edge*2)*sizeof(p1));
	edges=(pi *)malloc(edge*sizeof(pi));
	redges=(pi *)malloc(3*edge*sizeof(pi));
	memset(en,-1,(edge*2)*sizeof(ut));
	double begin_time=get_time();
	for(ut i=0;i<edge;i++){
		pi * p = (pi *)(buffers + 2 * sizeof(ut) + i * sizeof(pi));
		edges[i].a = p->a;
		edges[i].b = p->b;
		edges[i].t_begin = p->t_begin;
		edges[i].t_end = p->t_end;
		all[2*i]=make_pair(p->a,p->t_begin);
		all[2*i+1]=make_pair(p->b,p->t_end);
		//printf("%d %d\n",edges[i].a,edges[i].b);
	}
	sort(all,all+2*edge,cmp);
	ut all_vertices=0;
	for(int i=0;i<2*edge;i++){
		if(i==0||all[i]!=all[i-1]){
			all[all_vertices++]=all[i];
		}
	}
	for(int i=0;i<all_vertices;i++){
		be[all[i].first]=i;
		int p=i;
		while(p<all_vertices&&all[p].first==all[i].first){
			en[all[i].first]=p;
			p++;
		}
		i=p-1;
	}	
	ut all_edges=0;
    int b1=0,e1=-1,b_1=0,e_1=0,ee=0;
    for(int i=0;i<all_vertices;i++){
        e_1=i;
        if(all[i].first!=all[b_1].first){
            b_1=i;
        }
        if(e_1-b_1+1>e1-b1+1){
            e1=e_1;
            b1=b_1;
            ee=all[i].first;
        }
    }
    double no_time=get_time();
	for(int i=0;i<edge;i++){
		pi ne;
		ne.a=find(edges[i].a,edges[i].t_begin);
		ne.b=find(edges[i].b,edges[i].t_end);
		ne.t_begin=edges[i].t_begin;
		ne.t_end=edges[i].t_end;
		redges[all_edges++]=ne;
	}
	double end_time=get_time();
	printf("time : %lf %lf\n",end_time-begin_time,end_time-no_time);
	printf("%d %d %d %d %d\n",all_vertices,all_edges,b1,e1,ee);
	/*for(int i=0;i<all_vertices;i++){
		printf("%d %d\n",all[i].first,all[i].second);
	}
	for(int i=0;i<all_edges;i++){
		printf("%d %d %d %d\n",redges[i].a,redges[i].b,redges[i].t_begin,redges[i].t_end);
	}*/
	string output_ver=output+"-vertice.info";
	string output_edge=output+"-edge.info";
	string output_info=output+".info";
	ofstream out_ver(output_ver.c_str(),ios::binary);
	ofstream out_edge(output_edge.c_str(),ios::binary);
	ofstream out_info(output_info.c_str(),ios::binary);
    start_time=get_time();
	for(ut i=0;i<all_vertices;i++){
		out_ver.write((char *)&all[i].first,sizeof(ut));
		out_ver.write((char *)&all[i].second,sizeof(ut));
	}
	out_info.write((char *)&all_vertices,sizeof(ut));
	out_info.write((char *)&all_edges,sizeof(ut));
	out_info.write((char *)&b1,sizeof(ut));
	out_info.write((char *)&e1,sizeof(ut));
	for(ut i=0;i<all_edges;i++){
		out_edge.write((char *)&redges[i].a,sizeof(ut));
		out_edge.write((char *)&redges[i].b,sizeof(ut));
		out_edge.write((char *)&redges[i].t_begin,sizeof(ut));
		//printf("%d %d\n",redges[i].a,redges[i].b);
	}
    printf("write time: %lf\n",get_time()-start_time);
	/*
	string output_ver=output+"-vertice.info";
	string output_edge=output+"-edge.info";
	ofstream out_ver(output_ver.c_str(),ios::binary);
	ofstream out_edge(output_edge.c_str(),ios::binary);
	out_ver.write((char *)&all_vertices,sizeof(ut));
	for(ut i=0;i<all_vertices;i++){
		out_ver.write((char *)&all[i].first,sizeof(ut));
		out_ver.write((char *)&all[i].second,sizeof(ut));
	}
	out_edge.write((char *)&all_edges,sizeof(ut));
	for(ut i=0;i<all_edges;i++){
		out_edge.write((char *)&redges[i].a,sizeof(ut));
		out_edge.write((char *)&redges[i].b,sizeof(ut));
		out_edge.write((char *)&redges[i].t_begin,sizeof(ut));
		out_edge.write((char *)&redges[i].t_end,sizeof(ut));
		//printf("%d %d\n",redges[i].a,redges[i].b);
	}*/
	return 0;
}
