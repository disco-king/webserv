#include "Server.hpp"


int main(int argc, char**argv)
{
	Config conf;
	if (argv[1])
		conf.setPath(argv[1]);
	conf.parse();

	Server server(conf);

	server.select();
}



// int main()
// {
// 	std::vector<short> port;
// 	Server server;

// 	port.push_back(8000);

// 	server.init(port);
// 	server.select();
// }