#pragma once

class RequestConfig;

#include <string>
#include <map>
#include <set>
#include "ServerConfig.hpp"
#include "../Request.hpp"
#include "../utils.hpp"

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
	listen_socket _AddrData;
	bool _autoIndex;

public:
	RequestConfig(ServerConfig &config,
					Request const &request,
					std::string &location);
	RequestConfig(RequestConfig const &src);

	std::string const &getContentLocation() const;
	std::string const &getPath() const;
	std::map<int, std::string> const &getErrorPages() const;
	unsigned long const &getClientBodyBufferSize() const;
	std::map<std::string, std::string> const &getCGIParams() const;
	std::string const &getCGIPass() const;
	std::set<std::string>	const &getAllowedMethods() const;
	t_listener const &getAddrData() const;// t_listener here, you can change it later
	bool getAutoIndex() const;

	void setPath(int code);
	void setContentLocation(std::string const &path);
	void setAddrData(t_listener const &data); // here too
	std::string addIndex(Request const &request);

};