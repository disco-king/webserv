#include "RequestConfig.hpp"
#include "unistd.h"

RequestConfig::RequestConfig(ServerConfig &config, Request const &request, std::string &location) :
_body(request.getBody()),
_method(request.getMethod()),
_errorPages(config.getErrorPage()),
_clientBodyBufferSize(config.getClientBodyBufferSize()),
_allowedMethods(config.getAllowedMethods()),
_autoIndex(config.getAutoIndex()),
_index(config.getIndex()),
_code(request.getCode()),
_server_name(config.getServerName()[0])
{
	std::string root = config.getRoot();

	if(config.getAliasSet() && location[0] != '*'){
		_contentLocation = config.getAlias() + request.getPath().substr(location.size());
		_path = root + _contentLocation;
	}
	else{
		_contentLocation = request.getPath();
		_path = root + _contentLocation;
	}
	removeExtraSlashes(_path);
	removeExtraSlashes(_contentLocation);

	if(!is_filename(_path) && request.getMethod() == "GET"){
		_addIndex();
		config = config.getRequestLoc(_path, location);
	}
}

void RequestConfig::_addIndex()
{
	std::vector<std::string>::const_iterator it = _index.begin();
	std::vector<std::string>::const_iterator end = _index.end();
	if(_path[_path.size() - 1] != '/')
		_path.push_back('/');
	std::string path = _path;
	for (; it < end; ++it){
		if(!is_filename(path + *it))
			continue;
		_path += *it;
		_contentLocation += "/" + *it;
		removeExtraSlashes(_path);
		removeExtraSlashes(_contentLocation);
		return;
	}
}

std::string const &RequestConfig::getContentLocation() const
{
	return _contentLocation;
}

std::string const &RequestConfig::getPath() const
{
	return _path;
}

std::map<int, std::string> const &RequestConfig::getErrorPages() const
{
	return _errorPages;
}

unsigned long RequestConfig::getClientBodyBufferSize() const
{
	return _clientBodyBufferSize;
}

std::set<std::string>	const &RequestConfig::getAllowedMethods() const
{
	return _allowedMethods;
}

t_listen const &RequestConfig::getAddrData() const
{
	return _addrData;
}

bool RequestConfig::getAutoIndex() const
{
	return _autoIndex;
}

void RequestConfig::setAddrData(t_listen const &data)
{
	_addrData = data;
}

int RequestConfig::getCode() const
{
	return _code;
}

std::string const &RequestConfig::getBody() const
{
	return _body;
}

std::string RequestConfig::getMethod() const
{
	return _method;
}

void RequestConfig::setCode(int code)
{
	_code = code;
}

std::string RequestConfig::getServerName() const
{
	return _server_name;
}