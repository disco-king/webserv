#include "../Request.hpp"

#include <iostream>

int main()
{
	// std::string str = "GET / HTTP/1.1\r\nHost: www.example.com\r\n\r\n";
	// std::string str = "GET / HTTP/1.1\r\nHost: localhost:8000\r\nConnection: keep-alive\r\nsec-ch-ua: \".Not/A)Brand\";v=\"99\", \"Google Chrome\";v=\"103\", \"Chromium\";v=\"103\"\r\nsec-ch-ua-mobile: ?0\r\nsec-ch-ua-platform: \"Windows\"\r\nUpgrade-Insecure-Requests: 1\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) \r\nAppleWebKit/537.36 (KHTML, like Gecko) Chrome/103.0.0.0 \r\nSafari/537.36\r\nAccept-Language: ru-RU,ru;q=0.9,en-US;q=0.8,en;q=0.7,zh-CN;q=0.6,zh;q=0.5\r\n\r\n";
	std::string str = "POST /postplace HTTP/1.1\r\nHost: localhost:8000\r\n\r\n{\r\n  content: \"some content\"\r\n}";
	Request req(str);

	req.parseRequest();

	std::map<std::string, std::string> &hdrs = req.getHeaders();

	std::cout << "METHOD: " << req.getMethod() << '\n';
	std::cout << "PATH: " << req.getPath() << '\n';
	std::cout << "VERSION: " << req.getVersion() << '\n';
	std::cout << "HEADERS:\n";
	for (std::map<std::string, std::string>::iterator i = hdrs.begin(); i != hdrs.end(); ++i)
		std::cout << "  <" << i->first << ">: " << '[' << i->second << ']' << '\n';
	std::cout << "BODY:\n" << req.getBody() << '\n';
	std::cout << "CODE: " << req.getCode() << '\n';
}