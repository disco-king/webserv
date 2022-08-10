#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include "ServerConfig.hpp"
#include "../request/Request.hpp"
#include "../utils/utils.hpp"
#include "ConfigUtils.hpp"

class RequestConfig
{

private:
	std::string const &_body;
	std::string _method;
	std::string _contentLocation;
	std::string _path;
	std::map<int, std::string> _errorPages;
	unsigned long _clientBodyBufferSize;

	std::map<std::string, std::string> _CGIParams;
	std::string _CGIPass;
	std::set<std::string> _allowedMethods;
	t_listen _addrData;
	bool _autoIndex;
	std::vector<std::string> _index;
	int _code;
	std::string _server_name;



	void _addIndex();

	RequestConfig();

public:
	RequestConfig(ServerConfig &config, Request const &request, std::string &location);

	int getCode() const;
	std::string const &getBody() const;
	std::string getMethod() const;
	std::string const &getContentLocation() const;
	std::string const &getPath() const;
	std::map<int, std::string> const &getErrorPages() const;
	unsigned long getClientBodyBufferSize() const;
	std::set<std::string>	const &getAllowedMethods() const;
	t_listen const &getAddrData() const;
	bool getAutoIndex() const;
	std::string getServerName() const;

	void setAddrData(t_listen const &data);
	void setCode(int code);
};