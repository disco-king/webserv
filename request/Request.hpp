#pragma once

#include <string>
#include <map>

#include "../utils/utils.hpp"

class Request
{

private:
	std::string const &_request;
	int _code;
	std::string _path;
	std::string _method;
	std::string _body;
	std::string _version;
	std::map<std::string, std::string> _headers;

	void initHeaders();
	void startLine(std::string const &line);
	int verifyMethod();
	int verifyHeader(std::string &header);
	int parseHeader(std::string const &line);
	void checkRequiredHeaders();

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