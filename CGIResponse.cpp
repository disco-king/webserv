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
		char **argv = new char*[3];
		argv[0] = new char[_name.size()];
		argv[1] = new char[strlen("./cgi-bin/mycalendar.py") + 1];
		argv[3] = NULL;
		strcpy(argv[0], _name.c_str());
		strcpy(argv[1], "./cgi-bin/mycalendar.py");
		if (execve(_name.c_str(), argv, EnvpToChar()) < 0)
		{
			std::cerr << "execve failed\n";
		}
		delete[] argv[1];
		delete[] argv[0];
		delete[] argv;
		exit(0);
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
	std::string buffer2;
	int size = 0;

	cgi_file.open("temp_fileOut");
	buffer << cgi_file.rdbuf();

	size = buffer.str().size();

	buffer2.append(_firstHeader);
	buffer2.append("Content-type:text/html\r\n");
	buffer2.append("Content-length: ");
	std::stringstream ss;
	ss << size;
	buffer2.append(ss.str());
	buffer2.append("\r\n\r\n");
	buffer2.append(buffer.str());
	_cgiresponse.append(buffer2);
	std::cout << _cgiresponse;
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

char **CGIResponse::EnvpToChar()
{
	int i = 0;
	try
	{
		_envp = new char*[_envp_map.size() + 1];
	}
	catch (std::bad_alloc)
	{
		return NULL;
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
			return NULL;
		}
		strcpy(_envp[i], variable.c_str());
		i++;
		_envp[i] = NULL;
		it++;
	}
	return _envp;
}

void CGIResponse::Clear()
{
	for (int i = 0; _envp[i]; i++)
	{
		std::cout << "here\n";
		delete[] _envp[i];
	}
	delete[] _envp;
}