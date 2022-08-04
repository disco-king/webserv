#include "CGIresponse.hpp"

CGIResponse::CGIResponse(std::string name) :
	_name(name), _is_CGI(true)
{
}

CGIResponse::~CGIResponse()
{
}

void CGIResponse::SetFirstHeader()
{
	_firstHeader = "HTTP/1.1 200 OK\n";
}

std::string CGIResponse::GetFirstHeader()
{
	return (_firstHeader);
}

void CGIResponse::ExecuteCGIAndRedirect()
{
	SetFirstHeader();
	int pid;
	int fdOut;
	int OldFds[2];

	OldFds[0] = dup(STDIN_FILENO);
	OldFds[1] = dup(STDOUT_FILENO);

	if ((fdOut = open("temp_fileOut", O_CREAT | O_TRUNC | O_RDWR, 0777)) < 0)
	{
		std::cerr << "can't open or create file\n";
		return ;
	}
	pid = fork();

	if (pid < 0)
	{
		std::cerr << "Fork failed\n";
	}
	else if (pid == 0)
	{
		if (dup2(fdOut, STDOUT_FILENO) < 0)
			std::cerr << "can't dup\n";
		std::cout << _firstHeader;
		std::cout << "Content-Length: 13\n";
		std::cout << "\r\n";
		char **argv = NULL;

		EnvpToChar();
		if (execve(_name.c_str(), argv, _envp) < 0)
		{
			std::cerr << "execve failed\n";
		}
	}
	else
	{
		waitpid(-1, NULL, 0);
	}
	dup2(OldFds[1], STDOUT_FILENO);
	close(fdOut);
}

std::string CGIResponse::GetName()
{
	return _name;
}

bool CGIResponse::GetIsCGI()
{
	return _is_CGI;
}
void CGIResponse::MakeResponse()
{
	std::ifstream cgi_file;
	std::stringstream buffer;

	cgi_file.open("temp_fileOut");
	buffer << cgi_file.rdbuf();
	_cgiresponse = buffer.str();
}

std::string CGIResponse::GetCGIResponse()
{
	return _cgiresponse;
}

void CGIResponse::SetEnvp(RequestConfig &conf)
{
	_envp_map["PATH_INFO"] = conf.getPath();
	//_envp_map["PATH_TRANSLATED"] = conf.getPath(); //actually should be a virtual path (what the hell is virual path)
	_envp_map["SCRIPT_NAME"] = _name; //should be virtual path of script
	_envp_map["SCRIPT_FILENAME"] = _name;
	_envp_map["SCRIPT_URI"] = conf.getPath();
	_envp_map["REQUEST_METHOD"] = conf.getMethod();
	_envp_map["SERVER_NAME"] = "Server";
	_envp_map["GATEWAY_INTERFACE"] = "CGI/1.1";
	_envp_map["SERVER_PROTOCOL"] = "HTTP/1.1";
}

void CGIResponse::EnvpToChar()
{
	int i = 0;
	try
	{
		_envp = new char*[_envp_map.size() + 1];
	}
	catch (std::bad_alloc)
	{
		return ;
	}
	std::map<std::string, std::string>::iterator it = _envp_map.begin();
	while (it != _envp_map.end())
	{
		std::string variable = it->first + "=" + it->second;
		try
		{
			_envp[i] = new char[variable.size() + 1];
		}
		catch(std::bad_alloc)
		{
			return ;
		}
		strcpy(_envp[i], variable.c_str());
		i++;
		it++;
	}
}