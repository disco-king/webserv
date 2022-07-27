#pragma once

#include <string>
#include <map>

#include "utils.hpp"

class Request
{

private:
	int _code;
	std::string const &_request;
	std::string _path;
	std::string _method;
	std::string _body;
	std::string _version;
	std::map<std::string, std::string> _headers;

	void initHeaders();
	void startLine(std::string const &line);
	int verifyMethod();
	void parseHeader(std::string const &line);

	Request();
	Request(Request const&);

public:
	Request(std::string const &request);


	void parseRequest();

	std::string &getPath();
	std::string &getMethod();
	std::string &getBody();
	std::string &getVersion();
	std::map<std::string, std::string> &getHeaders();
	int getCode();

	void setBody(std::string const &body);
	void setCode(int code);

};