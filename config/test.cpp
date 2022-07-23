#include "ReadConfig.hpp"

int main(int argc, char **argv){
	ReadConfig readconfig;
	std::vector<std::string> tokens;
	try {
		tokens = readconfig.filereader(argv[1]);
		for (int i = 0; i < tokens.size(); i++)
			std::cout << tokens.at(i) << ' ';
		std::cout << std::endl;
	}
	catch (std::exception &ex) {
		std::cerr << ex.what() << std::endl;
	}
}