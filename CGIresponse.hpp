#pragma once
#include "header.hpp"

class CGIResponse
{
private:
	std::map<std::string, std::string> _envp_map;
	std::string _name;
	std::string _firstHeader;
	std::string _cgiresponse;
	char **_envp;
	char **_argv;
	bool _is_CGI;
public:
	CGIResponse(std::string name);
	~CGIResponse();
	void ExecuteCGIAndRedirect();
	void SetFirstHeader();
	std::string GetFirstHeader();
	std::string GetName();
	bool GetIsCGI();

	void SetEnvp(RequestConfig &conf);
	char **EnvpToChar();

	void MakeResponse();
	std::string GetCGIResponse();
};
