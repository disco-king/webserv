#pragma once

#include <string>
#include <vector>
#include <map>
#include <set>
#include "ServerConfig.hpp"
#include "../Request.hpp"
#include "../utils.hpp"
#include "ConfigUtils.hpp"

class RequestConfig
{

private:
	std::string _contentLocation;
	std::string _path;
	unsigned long _clientBodyBufferSize;
	std::map<int, std::string> _errorPages;
	std::map<std::string, std::string> _CGIParams;
	std::string _CGIPass;
	std::set<std::string> _allowedMethods;
	t_listen _addrData;
	std::vector<std::string> _index;
	bool _autoIndex;

	// std::string addIndex(Request const &request);

public:
	RequestConfig(ServerConfig &config, Request const &request, std::string &location);
	RequestConfig(RequestConfig const &src);

	std::string const &getContentLocation() const;
	std::string const &getPath() const;
	std::map<int, std::string> const &getErrorPages() const;
	unsigned long getClientBodyBufferSize() const;
	std::map<std::string, std::string> const &getCGIParams() const;
	std::string const &getCGIPass() const;
	std::set<std::string>	const &getAllowedMethods() const;
	t_listen const &getAddrData() const;
	bool getAutoIndex() const;

	// void setPath(int code);
	// void setContentLocation(std::string const &path);
	void setAddrData(t_listen const &data);

};