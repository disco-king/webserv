#include "Config.hpp"

int main(int argc, char **argv){
	Config conf = Config();
	try {
		if (argv[1])
			conf.setPath(argv[1]);
		conf.parse();
		conf.print_config();
	}
	catch (std::exception &ex) {
		std::cerr << ex.what() << std::endl;
	}
}