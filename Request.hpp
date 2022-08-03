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
	int verifyHeader(std::string header);
	int parseHeader(std::string const &line);

	Request();
	Request(Request const&);

public:
	Request(std::string const &request);


	void parseRequest();

	std::string const &getPath() const;
	std::string const &getMethod() const;
	std::string const &getBody() const;
	std::string const &getVersion() const;
	std::map<std::string, std::string> &getHeaders();
	int getCode() const;

	void setBody(std::string const &body);
	void setCode(int code);

};