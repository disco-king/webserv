#include "Server.hpp"
#include <signal.h>


int main(int argc, char**argv)
{
	(void) argc;
	Config conf;
	if (argv[1])
		conf.setPath(argv[1]);
	try {
		conf.parse();
	}
	catch (std::exception &ex){
		std::cout << ex.what() << std::endl;
	}

	// conf.print_config();

	signal(SIGPIPE, SIG_IGN);

	Server server(conf);
	server.run();
}
