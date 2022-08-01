#include <iostream>
#include "Request.hpp"


Request::Request(std::string const &request) : 
_request(request)
{}


int Request::verifyMethod()
{
	if(_method == "GET")
		return 0;
	if(_method == "PUT")
		return 0;
	if(_method == "POST")
		return 0;
	if(_method == "DELETE")
		return 0;
	return 1;
}


void Request::startLine(std::string const &line)
{
	_code = 400;

	std::vector<std::string> tokens = split(line);

	if(tokens.size() != 3)
		std::cerr << "error: invalid start-line\n";
	
	_method = strip(tokens[0]);
	_path = strip(tokens[1]);
	_version = strip(tokens[2]);
	
	if(_version != "HTTP/1.0" && _version != "HTTP/1.1")
		std::cerr << "error: wrong HTTP version\n";
	else if(verifyMethod())
		std::cerr << "error: invalid request method\n";
	else
		_code = 200;
}

void Request::parseHeader(std::string const &line)
{
	size_t delim = line.find(':');
	if(delim == std::string::npos)
		return;
	std::string key = line.substr(0, delim);
	std::string value = line.substr(delim + 1);
	// Will add header verification here.
	// For now we'll just assume all headers are correct.
	_headers[strip(key)] = strip(value);
}



void Request::parseRequest()
{
	size_t begin, end = _request.find('\n');

	startLine(_request.substr(0, end));
	if(_code == 400)
		return ;

	begin = end + 1;
	end = _request.find('\n', begin);

	while(_request[begin] != '\r' || begin + 1 != end){
		parseHeader(_request.substr(begin, end - begin));
		begin = end + 1;
		end = _request.find('\n', begin);
	}
	// We'll also (potentially) set language
	// and query text here. I suppose we don't
	// need that stuff right now.
	_body = _request.substr(end + 1);
}

void Request::setBody(std::string const &body)
{
	_body = body;
}

void Request::setCode(int code)
{
	_code = code;
}

std::string const &Request::getPath() const
{
	return _path;
}

std::string const &Request::getMethod() const
{
	return _method;
}

std::string const &Request::getBody() const
{
	return _body;
}

std::string const &Request::getVersion() const
{
	return _version;
}

std::map<std::string, std::string> &Request::getHeaders()
{
	return _headers;
}


int Request::getCode() const
{
	return _code;
}