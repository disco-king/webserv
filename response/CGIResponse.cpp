#include "CGIresponse.hpp"

CGIResponse::CGIResponse(const RequestConfig &conf)
{
	ScanForScripts();
	_name = conf.getPath();
	_is_python = false;
	_failed = false;
}

CGIResponse::CGIResponse(std::string name) :
	_name(name), _is_CGI(true)
{
	ScanForScripts();
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
		_failed = true;
		return ;
	}
	pid = fork();
	
	if (pid < 0)
	{
		std::cerr << "Fork failed\n";
		_failed = true;
		return ;
	}
	else if (pid == 0)
	{
		if (dup2(fdOut, STDOUT_FILENO) < 0)
			std::cerr << "can't dup\n";
		try
		{
			char **argv = new char*[3];
			if (_is_python)
			{
				argv[0] = new char[strlen(_scripts[_name].c_str()) + 1];
				argv[1] = new char[_name.size() + 1];
				strcpy(argv[0], _scripts[_name].c_str());
				strcpy(argv[1], _name.c_str());
			}
			else
			{
				argv[0] = new char[strlen(_name.c_str()) + 1];
				strcpy(argv[0], _name.c_str());
				argv[1] = NULL;
			}
			argv[2] = NULL;
			_envp = EnvpToChar();
			if (execve(argv[0], argv, _envp) < 0)
			{
				std::cerr << "execve failed\n";
			}
			delete[] argv[1];
			delete[] argv[0];
			delete[] argv;
		}
		catch (std::bad_alloc &e)
		{
			std::cerr << "new failed\n";
		}
		Clear();
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
	char *absolute_path = new char[512];
	struct stat file_stats;

	cgi_file.open("temp_fileOut");
	stat("temp_fileOut", &file_stats);

	buffer2.append(_firstHeader);
	buffer2.append("Content-type:text/html\r\n");
	buffer2.append("Content-length: ");
	std::stringstream ss;
	ss << file_stats.st_size;
	buffer2.append(ss.str());
	buffer2.append("\r\n\r\n");
	buffer2.append("file_abs_path:");
	buffer2.append(getcwd(absolute_path, 512));
	buffer2.append("/temp_fileOut");
	_cgiresponse.append(buffer2);
	delete[] absolute_path;
	cgi_file.close();
}

std::string CGIResponse::GetCGIResponse()
{
	return _cgiresponse;
}

void CGIResponse::SetEnvp(RequestConfig &conf)
{
	_envp_map["PATH_INFO"] = conf.getPath();
	_envp_map["SCRIPT_NAME"] = _name; //should be virtual path of script
	_envp_map["SCRIPT_FILENAME"] = _name;
	_envp_map["SCRIPT_URI"] = conf.getPath();
	_envp_map["REQUEST_METHOD"] = conf.getMethod();
	_envp_map["SERVER_NAME"] = conf.getServerName();
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
	catch (std::bad_alloc &e)
	{
		_failed = true;
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
		catch(std::bad_alloc &e)
		{
			_failed = true;
			return NULL;
		}
		_envp[i] = strcpy(_envp[i], variable.c_str());
		i++;
		_envp[i] = NULL;
		it++;
	}
	return _envp;
}

void CGIResponse::ScanForScripts()
{
	DIR *dir;
	struct dirent *ent;
	try
	{
		char *buff = new char[1000];
		std::string path;
		getcwd(buff, 1000);

		path.append(buff);
		path.append("/cgi-bin");
		dir = opendir(path.c_str());
		if (dir)
		{
			while ((ent = readdir(dir)))
			{
				if (ent->d_type == DT_REG && IsPythonScript(ent->d_name))
				{
					_scripts[path + "/" + ent->d_name] = "/usr/local/bin/python3";
				}
				else if (ent->d_type == DT_REG || ent->d_type == DT_UNKNOWN)
				{
					_scripts[path + "/" + ent->d_name] = "";
				}
			}
		}
		closedir(dir);
		delete[] buff;
	}
	catch (std::bad_alloc &e)
	{
		std::cerr << "new failed\n";
		_failed = true;
		return ;
	}


}

bool CGIResponse::IsPythonScript(const std::string &file_name)
{
	if (file_name.substr(file_name.find_last_of(".") + 1) == "py")
	{
		_is_python = true;
		_scripts[file_name] = "/usr/local/bin/python3";
		return true;
	}
	return false;
}

bool CGIResponse::HasSuchScript(const std::string &script_name)
{
	std::map<std::string, std::string>::iterator it = _scripts.begin();
	std::string str = script_name;
	// if (str[str.size() - 1] == '/')
	// 	str[str.size() - 1] = 0;
	while (it != _scripts.end())
	{
		std::cout << str << std::endl;
		std::cout << it->first << std::endl;
		if (it->first == str)
		{
			if (_scripts[str].size() > 0)
			{
				std::cout << "here\n";
				_is_python = true;
			}
			else
				_name = str;

			return true;		
		}

		it++;
	}
	return false;
}

void CGIResponse::Clear()
{
	for (int i = 0; _envp[i] != NULL; i++)
	{
		delete[] _envp[i];
	}
	delete[] _envp;
}

bool CGIResponse::GetFailed()
{
	return _failed;
}