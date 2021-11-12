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
	int a,b,t_begin,t_end;
};
struct ppi{
	ut a,b;
    int t_begin;

};
typedef pair<ut,ut> pr;
queue<ut>que;
pi *edges;
ppi *redges;
int cmp(p1 a, p1 b){
	if(a.first!=b.first){
		return a.first<b.first;
	}
	return a.second<b.second;
}
int cmp1(ppi a,ppi b){
	if(a.a!=b.a){
		return a.a<b.a;
	}
	return a.b<b.b;
}
ut find(int a,int b){
	ut le=be[a];
	ut ri=en[a];
	while(le<=ri){
		ut mid=(le+ri)/2;
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
	ut vertice = *(int *)buffers;
	ut edge = *(int *)(buffers + sizeof(int));
	printf("Vertices: %lld, Edges %lld\n", vertice, edge);


//	int w=fscanf(fp,"%lld",&edge);
//	printf("%d\n",w);
//	cout<<vertice<<" "<<edge<<endl;

//	fflush(stdout);
	
	be=(ut *)malloc((vertice+10)*sizeof(ut));
	en=(ut *)malloc((vertice+10)*sizeof(ut));
	all=(p1 *)malloc((edge*2)*sizeof(p1));
	edges=(pi *)malloc(edge*sizeof(pi));
	redges=(ppi *)malloc(3*edge*sizeof(ppi));
	memset(en,-1,(vertice+10)*sizeof(ut));
	double begin_time=get_time();
	for(ut i=0;i<edge;i++){
		pi * p = (pi *)(buffers + 2 * sizeof(int) + i * sizeof(pi));
		edges[i].a = p->a;
		edges[i].b = p->b;
		edges[i].t_begin = p->t_begin;
		edges[i].t_end = p->t_end;
		all[2*i]=make_pair(p->a,p->t_begin);
		all[2*i+1]=make_pair(p->b,p->t_end);
		//printf("%d %d\n",edges[i].a,edges[i].b);
	}
    double srt_b=get_time();
	sort(all,all+2*edge,cmp);
    printf("sort time : %lf\n",get_time()-srt_b);
	ut all_vertices=0;
	for(ut i=0;i<2*edge;i++){
		if(i==0||all[i]!=all[i-1]){
			all[all_vertices++]=all[i];
		}
	}
    printf("sssss %lld\n",all_vertices);
	ut all_edges=0;
	for(ut i=0;i<all_vertices;i++){
		be[all[i].first]=i;
		ut p=i;
		while(p<all_vertices&&all[p].first==all[i].first){
			if(p!=i){
				ppi ne;
				ne.a=p-1;
				ne.b=p;
				ne.t_begin=all[p-1].second;
				redges[all_edges++]=ne;
			}
			en[all[i].first]=p;
			p++;
		}
		i=p-1;
	}	
/*    int b1=0,e1=-1,b_1=0,e_1=0;
    for(long long i=0;i<all_vertices;i++){
        e_1=i;
        if(all[i].first!=all[b_1].first){
            b_1=i;
        }
        if(e_1-b_1+1>e1-b1+1){
            e1=e_1;
            b1=b_1;
        }
    }*/
    printf("fuckkk\n");
	for(int i=0;i<edge;i++){
		ppi ne;
		ne.a=find(edges[i].a,edges[i].t_begin);
		ne.b=find(edges[i].b,edges[i].t_end);
		ne.t_begin=edges[i].t_begin;
		redges[all_edges++]=ne;
	}
	double end_time=get_time();
	printf("time : %lf\n",end_time-begin_time);
	printf("%lld %lld\n",all_vertices,all_edges);
	/*for(int i=0;i<all_vertices;i++){
		printf("%d %d\n",all[i].first,all[i].second);
	}
	for(int i=0;i<all_edges;i++){
		printf("%d %d %d %d\n",redges[i].a,redges[i].b,redges[i].t_begin,redges[i].t_end);
	}*/
	sort(redges,redges+all_edges,cmp1);
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
//	out_info.write((char *)&b1,sizeof(ut));
//	out_info.write((char *)&e1,sizeof(ut));
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
