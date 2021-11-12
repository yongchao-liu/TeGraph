#include <stdio.h>
#include <iostream>
#include <fstream>

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
	
	while (!input_file.eof()) {
		if (!input_file.getline(buf, 256) || buf[0] == '#')
			continue;
		int n,m;
		if (count == 0) {
			sscanf(buf, "%d %d\n", &n,&m);
			output_file.write((char *)&n, sizeof(int));
			output_file.write((char *)&m, sizeof(int));
			printf("%d %d\n",n,m);
			count++;
			continue;
		}
		
		int a,b,c,d;
		sscanf(buf, "%d %d %d\n", &a, &b, &c);
//		printf("%d %d %d %d\n",a,b,c,c);
		output_file.write((char *)&a, sizeof(int));
		output_file.write((char *)&b, sizeof(int));
		output_file.write((char *)&c, sizeof(int));
		output_file.write((char *)&c, sizeof(int));

		count++;
	}

	printf("Total edges is %ld\n", count-1);
	return 0;
}
