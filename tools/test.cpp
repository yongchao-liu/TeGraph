/*************************************************************************
    > File Name: preprocess.cpp
    > Author: huanzhizun
    > Mail: huznzhiun888@126.com 
    > Created Time: Fri 09 Mar 2018 03:32:33 PM CST
 ************************************************************************/
#include  "src/core.hpp"

using namespace std;
typedef pair<int,int>p1;
ut *all;
ut *rver;
ut *be,*en;
ut *ma;

struct pi{
	ut a,b,t_begin,t_end;
};
typedef pair<ut,ut> pr;
queue<ut>que;
pi *edges,*redges;
int find(int a,int b){
	int le=be[a];
	int ri=en[a]-1;
	if(b>all[ri]) return ri;//大的点删掉
	while(le<=ri){
		int mid=(le+ri)/2;
		if(b>all[mid]) le=mid+1;
		else ri=mid-1;
	}
	return le;
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
	
	be=(ut *)malloc((vertice+10)*sizeof(ut));
	ma=(ut *)malloc((vertice+10)*sizeof(ut));
	en=(ut *)malloc((vertice+10)*sizeof(ut));
	all=(ut *)malloc((edge)*sizeof(p1));
	edges=(pi *)malloc(edge*sizeof(pi));
	redges=(pi *)malloc(2*edge*sizeof(pi));
	memset(be,0,(vertice+10)*sizeof(ut));
	memset(ma,0,(vertice+10)*sizeof(ut));
	double begin_time=get_time();
	for(ut i=0;i<edge;i++){
		pi * p = (pi *)(buffers + 2 * sizeof(ut) + i * sizeof(pi));
		edges[i].a = p->a;
		edges[i].b = p->b;
		edges[i].t_begin = p->t_begin;
		edges[i].t_end = p->t_end;
		//printf("%d %d\n",edges[i].a,edges[i].b);
	}
	for(int i=0;i<edge;i++){
		if(edges[i].t_begin>ma[edges[i].a]){
			be[edges[i].a]++;
			ma[edges[i].a]=edges[i].t_begin;
		}
	}
	en[0]=be[0]=0;
	ut all_vertices=0;
	memset(ma,0,(vertice+10)*sizeof(ut));
	for(int i=1;i<=vertice;i++){
		ut nn=all_vertices+be[i];
		be[i]=all_vertices;
		en[i]=be[i];
		all_vertices=nn;
	}
	for(int i=0;i<edge;i++){
		if(edges[i].t_begin>ma[edges[i].a]){
			all[en[edges[i].a]++]=edges[i].t_begin;	
			ma[edges[i].a]=edges[i].t_begin;
		}
	}
	memset(ma,-1,(vertice+10)*sizeof(ut));
	/*
	for(int i=1;i<=vertice;i++){
		if(tmp[i]==-1){
			be[i]=all_vertices;
			en[i]=all_vertices;
			tmp[i]=all_vertices;
			all[all_vertices++]=make_pair(i,2000000000);
		}
	}*/
	int nn=0;
	ut all_edges=0;
	{
		
		#pragma omp parallel for  num_threads(32)
		for(int i=0;i<edge;i++){
			if(en[edges[i].b]==be[edges[i].b]){
				edges[i].b=-1;
			}
			else edges[i].b=find(edges[i].b,edges[i].t_end);
		}
	}
	for(int i=0;i<edge;i++){
		if(edges[i].b==-1){
			nn++;
			continue;
		}
		pi ne;
		if(ma[edges[i].a]!=-1&&edges[i].t_begin==all[ma[edges[i].a]]){
			ne.a=ma[edges[i].a];
		}
		else{
			if(ma[edges[i].a]==-1){
				ne.a=be[edges[i].a];
				ma[edges[i].a]=ne.a;
			}
			else{
				ne.a=++ma[edges[i].a];
				pi nn;
				nn.a=ma[edges[i].a]-1;
				nn.b=ma[edges[i].a];
				nn.t_begin=all[nn.b]-all[nn.a];
				redges[all_edges++]=nn;
				printf("1 ");
			}
		}
		ne.b=edges[i].b;
		ne.t_begin=edges[i].t_end-edges[i].t_begin;
		redges[all_edges++]=ne;
	}
	double end_time=get_time();
	printf("time %lf\n",end_time-begin_time);
	printf("%d %d\n",all_vertices,all_edges);
/*	
	for(int i=1;i<=all_vertices;i++){
		printf("%d %d\n",be[i],en[i]);
	}
	for(int i=0;i<all_edges;i++){
		printf("%d %d %d %d\n",redges[i].a,redges[i].b,redges[i].t_begin,redges[i].t_end);
	}*/
	//sort(redges,redges+all_edges,cmp1)
	/*
	ofstream out_edge(output,ios::binary); 
	out_edge.write((char *)&all_vertices,sizeof(ut));
	out_edge.write((char *)&all_edges,sizeof(ut));
	for(ut i=0;i<edge;i++){
		out_edge.write((char *)&edges[i].a,sizeof(ut));
		out_edge.write((char *)&edges[i].b,sizeof(ut));
		out_edge.write((char *)&edges[i].t_begin,sizeof(ut));
		out_edge.write((char *)&edges[i].t_end,sizeof(ut));
		//printf("%d %d\n",redges[i].a,redges[i].b);
	}
	*/
	
	string output_ver=output+"-vertice.info";
	string output_edge=output+"-edge.info";
	string output_info=output+".info";
	ofstream out_ver(output_ver.c_str(),ios::binary);
	ofstream out_edge(output_edge.c_str(),ios::binary);
	ofstream out_info(output_info.c_str(),ios::binary);
	for(ut i=0;i<all_vertices;i++){
		int no=0;
		out_ver.write((char *)&no,sizeof(ut));
	}
	out_info.write((char *)&all_vertices,sizeof(ut));
	out_info.write((char *)&all_edges,sizeof(ut));
	for(ut i=0;i<all_edges;i++){
		out_edge.write((char *)&redges[i].a,sizeof(ut));
		out_edge.write((char *)&redges[i].b,sizeof(ut));
		//printf("%d %d\n",redges[i].a,redges[i].b);
	}
	return 0;
}
