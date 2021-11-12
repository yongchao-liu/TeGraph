#ifndef VF2_GRAPH_H
#define VF2_GRAPH_H

#include <cstring>
#include <cstdio>
#include <algorithm>
#include <vector>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <assert.h>
#include "src/u_size.hpp"
using namespace std;

const int MAXN = 305;
struct Edge_pure{
	ut from,to,begin_time,end_time;
	Edge_pure(ut a,ut b,ut c,ut d){
		from=a;
		to=b;
		begin_time=c;
		end_time=d;
	}
	Edge_pure(){
	}
};
struct Edge_path{
	Edge_pure edge;
	ut len;
	Edge_path *pre;
	ut frequence;
	Edge_path(){
		frequence=0;
		pre=NULL;
	}
	void free_memory(){
		Edge_path *tmp;
		tmp=pre;
		while(tmp!=NULL){
			tmp->frequence--;
			if(tmp->frequence==0){
				Edge_path *no=tmp->pre;
				free(tmp);
				tmp=no;
			}
			else
			tmp=tmp->pre;
		}
	}
};
class Graph {
public:
    ut n, e;
	ut en;
    void addedge(ut u, ut v, ut begin_t,ut end_t);
    void eraseedge(ut u, ut v, ut begin_t,ut end_t);
    ut getSize();
	void free_memory();
	Time getoutside(ut a,ut b);
	map<pair<ut,ut>,Time>outside;
	multimap<ut,pair<ut,Time> >edge,redge;
	map<ut,ut>vertice;
    Graph();
    Graph(ut _n, ut _e);
};

void Graph::addedge(ut u, ut v, ut t_begin,ut t_end) {
	Time time=make_pair(t_begin,t_end);
	outside[make_pair(u,v)]=time;
	edge.insert(make_pair(u,make_pair(v,time)));
	redge.insert(make_pair(v,make_pair(u,time)));
	vertice[u]++;
	vertice[v]--;
}
void Graph::eraseedge(ut u,ut v,ut t_begin,ut t_end){
	Time time=make_pair(t_begin,t_end);
	outside[make_pair(u,v)]=time;
	multimap<ut,pair<ut,Time> >::iterator it;
	it=edge.find(u);
	if(it!=edge.end()){
		edge.erase(u);
	}
	it=redge.find(v);
	if(it!=redge.end()){
		redge.erase(it);
	}
	if(vertice[u]==1) vertice.erase(u);
	else vertice[u]--;
	if(vertice[v]==1) vertice.erase(v);
	else vertice[v]--;
}
Graph::Graph(ut _n, ut _e) {
    n = _n;
    e = _e;
	outside.clear();
	vertice.clear();
	edge.clear();
	redge.clear();
}

Graph::Graph() {
	outside.clear();
	vertice.clear();
	edge.clear();
	redge.clear();
}
Time Graph::getoutside(ut n,ut m){
	map<pair<ut,ut>,Time>::iterator it;
	it=outside.find(make_pair(n,m));
	if(it==outside.end()){
		return make_pair(-1,-1);
	}
	return it->second;
}
ut Graph::getSize() {
    return n;
}
void Graph::free_memory(){
	outside.clear();
	edge.clear();
	redge.clear();
}
#endif //VF2_GRAPH_H
