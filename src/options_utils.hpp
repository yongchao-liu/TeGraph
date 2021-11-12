#ifndef _OPTIONS_UTILS_
#define _OPTIONS_UTILS_
#include "src/cmdline.hpp"

static cmdline::parser setup_options(int argc, char ** argv){	
	cmdline::parser cmd;

	cmd.add<std::string>("input", 'i', "Input file path", true, "");
	cmd.add<float>("memory", 'm', "Physical memory", false, 8);
	cmd.add<int>("processors", 'p', "Number of processors", false, 16);
	cmd.add<int>("iterations", 'n', "Number of iterations(pagerank)", false, 20);
	cmd.add<int>("start", 's', "Start vertex(SSSP,BFS)", false, 0);
	cmd.add<int>("mrt", 'c', "Max reentry number(SSSP,BFS,Pagerank)", false, 5);
	cmd.add<int>("vertices", 'v', "number of vertices", false, 0);


	cmd.parse_check(argc, argv);

	return cmd;
}
#endif
