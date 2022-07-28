#include <cstdlib>
#include <iostream>

int decodeChunks(std::string &request)
{
	size_t pos = request.find("\r\n\r\n");
	std::string processed = request.substr(0, pos + 4);

	size_t chunk_size = std::strtoll(&(request[pos + 4]), 0, 16);
	pos = request.find("\r\n", pos + 4);
	size_t overall_size = request.size() - pos - 7;//max potential sum of chunk bodies size left
	while(chunk_size)
	{
		if(chunk_size > overall_size)
			break;

		std::cout << "overall size left: " << overall_size << '\n';
		std::cout << "adding <" << request.substr(pos + 2, chunk_size)
		<< "> with pos " << pos << " and size " << chunk_size << '\n';

		processed += request.substr(pos + 2, chunk_size);
		pos += chunk_size + 4;
		chunk_size = std::strtoll(&(request[pos]), 0, 16);
		pos = request.find("\r\n", pos);
		overall_size = request.size() - pos - 7;
	}
	request = processed + "\r\n\r\n";
	return chunk_size;
}

int main()
{
	std::string req = "POST / HTTP/1.1\r\nHost: localhost:8000\r\nTransfer-Encoding: chunked\
\r\n\r\n9\r\nChunk one\r\n9\r\nChunk two\r\nB\r\nChunk three\r\n0\r\n\r\n";
	int ret = decodeChunks(req);
	std::cout << ret << "\n\n" << "[" << req << "]\n";
}