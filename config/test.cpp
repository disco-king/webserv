#include "Config.hpp"

int main(int argc, char **argv){
	Config conf = Config();
	try {
		if (argv[1])
			conf.setPath(argv[1]);
		conf.setFile();
		conf.print_file();
	}
	catch (std::exception &ex) {
		std::cerr << ex.what() << std::endl;
	}
}