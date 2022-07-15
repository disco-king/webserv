#include "Server.hpp"

int main()
{
	std::vector<short> port;
	Server server;

	port.push_back(8000);

	server.init(port);
	server.select();
}