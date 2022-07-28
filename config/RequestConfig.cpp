#include "RequestConfig.hpp"

RequestConfig::RequestConfig(ServerConfig &config, Request const &request, std::string &location) :
_errorPages(config.getErrorPage()),
_clientBodyBufferSize(config.getClientBodyBufferSize()),
_CGIParams(config.getCgiParam()),
_CGIPass(config.getCgiPass()),
_allowedMethods(config.getAllowedMethods()),
_autoIndex(config.getAutoIndex()),
_index(config.getIndex().begin(), config.getIndex().end())
{
	std::string root = config.getRoot();
	std::string alias = config.getAlias();

	if(config.getAliasSet() && location[0] != '*'){
		_contentLocation = removeSlashes(alias + request.getPath().substr(location.size()));
		_path = removeSlashes(root + _contentLocation);
	}
	else{
		_contentLocation = removeSlashes(request.getPath());
		_path = removeSlashes(root + _contentLocation);
	}

	if(!is_filename(_path) && request.getMethod() == "GET"){
		_path = removeSlashes(_path + "/index.html");
		config = config.getRequestLoc(_path, location);
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

std::map<std::string, std::string> const &RequestConfig::getCGIParams() const
{
	return _CGIParams;
}

std::string const &RequestConfig::getCGIPass() const
{
	return _CGIPass;
}

std::set<std::string>	const &RequestConfig::getAllowedMethods() const
{
	return _allowedMethods;
}

listen_socket const &RequestConfig::getAddrData() const
{
	return _addrData;
}

bool RequestConfig::getAutoIndex() const
{
	return _autoIndex;
}

void RequestConfig::setAddrData(t_listener const &data)
{
	_addrData.host = data.host;
	_addrData.port = data.port;
}