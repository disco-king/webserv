#pragma once
#include "header.hpp"
#include "../config/RequestConfig.hpp"

class CGIResponse
{
private:
	std::map<std::string, std::string> _envp_map;
	std::map<std::string, std::string> _scripts;
	std::string _name;
	std::string _firstHeader;
	std::string _cgiresponse;
	char **_envp;
	char **_argv;
	bool _is_CGI;
	bool _is_python;
	bool _failed;

	CGIResponse();
public:
	CGIResponse(const RequestConfig &conf);
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

	void ScanForScripts();
	bool IsPythonScript(const std::string &file_name);
	bool HasSuchScript(const std::string &script_name);
	bool GetFailed();
	void Clear();
};
