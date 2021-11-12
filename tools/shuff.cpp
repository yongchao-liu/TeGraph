#include <stdio.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;
struct pi{
	int a,b,c,d;
};
pi *gg;
int *g;
int main(int argc, char ** argv) {
	if (argc < 3) {
		printf("Usage: graph_conver [input file] [output file]\n");
		exit(-1);
	}
	std::string input = argv[1];
	std::string output = argv[2];

	long count = 0;

	std::ifstream input_file(input.c_str());
	std::ofstream output_file(output.c_str(), std::ofstream::binary);

	if (!input_file) {
		printf("Can't open input file\n");
		exit(-1);
	} else if (!output_file) {
		printf("Can't open output file\n");
		exit(-1);
	}

	char buf[256];
	int tot=0;
	int n,m;
	while (!input_file.eof()) {
		if (!input_file.getline(buf, 256) || buf[0] == '#')
			continue;
		if (count == 0) {
			sscanf(buf, "%d %d\n", &n,&m);
			output_file.write((char *)&n, sizeof(int));
			output_file.write((char *)&m, sizeof(int));
			g=(int *)malloc(sizeof(int)*(m+1));
			gg=(pi *)malloc(sizeof(pi)*(m+1));
			count++;
			continue;
		}
		pi pp;
		sscanf(buf, "%d %d %d %d\n", &pp.a, &pp.b, &pp.c,&pp.d);
		g[tot]=pp.d;
		gg[tot++]=pp;
	}
	sort(g,g+tot);
	printf("%d %d\n",n,m);
	for(int i=0;i<m;i++){
		pi pp=gg[i];
		pp.c=sqrt(i+1);
		pp.d=lower_bound(g,g+tot,pp.d)-g;
		output_file.write((char *)&pp.a, sizeof(int));
		output_file.write((char *)&pp.b, sizeof(int));
		output_file.write((char *)&pp.c, sizeof(int));
		output_file.write((char *)&pp.d, sizeof(int));
		if(i<=10)
		printf("%d %d %d %d\n",pp.a,pp.b,pp.c,pp.d);
	}
	printf("Total edges is %ld\n", m);
	return 0;
}
