#include "Server.hpp"


int main(int argc, char**argv)
{
	Config conf;
	if (argv[1])
		conf.setPath(argv[1]);
	conf.parse();
	conf.print_config();

	std::cout << "Config output:\n";
	std::vector<t_listen>::const_iterator end = conf.getListeners().end();
	for (std::vector<t_listen>::const_iterator it = conf.getListeners().begin(); it != end; ++it){
		std::cout << "host " << it->host << " port " << it->port << '\n';
	}
	std::cout << "Server output:\n";

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