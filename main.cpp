#include "Server.hpp"


int main(int argc, char**argv)
{
	Config conf;
	if (argv[1])
		conf.setPath(argv[1]);
	conf.parse();
	conf.print_config();

	std::cout << "\nConfig output:\n";
	std::vector<t_listen> listeners = conf.getListeners();
	std::vector<t_listen>::const_iterator end = listeners.end();
	for (std::vector<t_listen>::const_iterator it = listeners.begin(); it != end; ++it){
		std::cout << "host " << it->host << " port " << it->port << '\n';
	}
	std::cout << "\nServer output:\n";

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