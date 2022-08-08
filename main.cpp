#include "Server.hpp"


int main(int argc, char**argv)
{
	try {Config conf;
		if (argv[1])
			conf.setPath(argv[1]);
		conf.parse();
		conf.print_config();


		Server server(conf);

		server.select();
	}

	catch (std::exception &ex){
		std::cout << ex.what() << std::endl;
	}
}



// int main()
// {
// 	std::vector<short> port;
// 	Server server;

// 	port.push_back(8000);

// 	server.init(port);
// 	server.select();
// }