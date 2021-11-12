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
ut *tmp;
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
int find(int a,int b){
	int le=be[a];
	int ri=en[a];
	if(b>all[en[a]].second) return ri;//大的点删掉
	while(le<=ri){
		int mid=(le+ri)/2;
		if(b>all[mid].second) le=mid+1;
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
	
	rver=(ut *)malloc((edge)*sizeof(ut));
	be=(ut *)malloc((edge)*sizeof(ut));
	tmp=(ut *)malloc((edge)*sizeof(ut));
	en=(ut *)malloc((edge)*sizeof(ut));
	all=(p1 *)malloc((edge)*sizeof(p1));
	edges=(pi *)malloc(edge*sizeof(pi));
	redges=(pi *)malloc(2*edge*sizeof(pi));
	memset(en,-1,edge*sizeof(ut));
	memset(tmp,-1,edge*sizeof(ut));
	double begin_time=get_time();
	for(ut i=0;i<edge;i++){
		pi * p = (pi *)(buffers + 2 * sizeof(ut) + i * sizeof(pi));
		edges[i].a = p->a;
		edges[i].b = p->b;
		edges[i].t_begin = p->t_begin;
		edges[i].t_end = p->t_end;
		all[i]=make_pair(p->a,p->t_begin);
		//printf("%d %d\n",edges[i].a,edges[i].b);
	}
	sort(all,all+edge,cmp);
	ut all_vertices=0;
	for(int i=0;i<edge;i++){
		if(i==0||all[i]!=all[i-1]){
			all[all_vertices++]=all[i];
		}
	}
	ut all_edges=0;
	for(int i=0;i<all_vertices;i++){
		be[all[i].first]=i;
		tmp[all[i].first]=i;
		int p=i;
		while(p<all_vertices&&all[p].first==all[i].first){
			en[all[i].first]=p;
			p++;
		}
		i=p-1;
	}	
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
	for(int i=0;i<edge;i++){
		if(tmp[edges[i].b]==-1){
			nn++;
			continue;
		}
		pi ne;
		if(tmp[edges[i].a]>en[edges[i].a]||edges[i].t_begin<all[tmp[edges[i].a]].second){
			ne.a=tmp[edges[i].a]-1;
		}
		else{
			ne.a=tmp[edges[i].a];
			if(tmp[edges[i].a]!=be[edges[i].a]){
				pi nn;
				nn.a=tmp[edges[i].a]-1;
				nn.b=tmp[edges[i].a];
				redges[all_edges++]=nn;
		//		printf("1 ");
			}
			tmp[edges[i].a]++;
		}
		ne.b=find(edges[i].b,edges[i].t_end);
		redges[all_edges++]=ne;
	}
	double end_time=get_time();
	printf("time %lf\n",end_time-begin_time);
	printf("%d %d\n",all_vertices,all_edges);
	/*
	for(int i=0;i<all_vertices;i++){
		printf("%d %d\n",all[i].first,all[i].second);
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
