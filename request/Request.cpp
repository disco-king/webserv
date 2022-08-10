#include <iostream>
#include "Request.hpp"


Request::Request(std::string const &request) : 
_request(request), _code(400)
{}

int Request::verifyHeader(std::string &header)
{
	to_lower(header);
	if(header == "host")
		return 0;
	if(header == "content-length")
		return 0;
	if(header == "transfer-encoding")
		return 0;
	if(header == "cgi-test-header")
		return 0;
	return 1;
}

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
	std::vector<std::string> tokens = split(line);

	if(tokens.size() != 3){
		std::cerr << "Error: invalid start-line\n";
		return;
	}

	_method = strip(tokens[0]);
	_path = strip(tokens[1]);
	_version = strip(tokens[2]);
	
	if(_version != "HTTP/1.0" && _version != "HTTP/1.1")
		std::cerr << "Error: wrong HTTP version\n";
	else if(verifyMethod()){
		_code = 405;
		std::cerr << "Error: invalid request method\n";
	}
	else
		_code = 200;
}

int Request::parseHeader(std::string const &line)
{
	size_t delim = line.find(':');
	if(delim == std::string::npos)
		return 1;
	std::string key = line.substr(0, delim);
	std::string value = line.substr(delim + 1);
	if(strip(value).size() == 0 || strip(key).size() == 0)
		return 1;
	if(!verifyHeader(key))
		_headers[key] = value;
	_code = 200;
	return 0;
}

void Request::parseRequest()
{
	size_t begin, end = _request.find('\n');

	startLine(_request.substr(0, end));
	if(_code >= 400)
		return ;

	begin = end + 1;
	end = _request.find('\n', begin);

	while(_request[begin] != '\r' || begin + 1 != end){
		if(parseHeader(_request.substr(begin, end - begin)))
			break;
		begin = end + 1;
		end = _request.find('\n', begin);
	}

	checkRequiredHeaders();

	if(_code == 200)
		_body = _request.substr(end + 1);
}

void Request::checkRequiredHeaders()
{
	if(!_headers.count("host"))
		_code = 400;
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