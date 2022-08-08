#include "Response.hpp"

Response::Response()
{
}

Response::Response(std::string content_type, int content_length, std::string body) :
_response_code(0)
{
	_content_type = content_type;
	_content_length = content_length;
	_body = body;
	_is_cgi = false;
	SetMethods();
	FillCodes();
}
Response::~Response()
{
}
Response::Response( Response const &other )
{
	*this = other;
}
Response & Response::operator=( Response const &other )
{
	this->_codes = other._codes;
	return(*this);
}

bool Response::GetIsCGI()
{
	return _is_cgi;
}

void Response::SetIsCGI(bool value)
{
	_is_cgi = value;
}

void Response::SetDefaultErrorPages(const std::map<int, std::string> &pages)
{
	_error_pages = pages;
}

std::map<int, std::string> Response::GetDefaultErrorPages()
{
	return _error_pages;
}

void Response::FillCodes()
{
	/*informational 100 - 103 */
	_codes.insert(std::make_pair(100, "Continue"));
	_codes.insert(std::make_pair(101, "Switching Protocols"));
	_codes.insert(std::make_pair(102, "Processing"));
	_codes.insert(std::make_pair(103, "Early Hints"));

	/*success 200-299*/
	_codes.insert(std::make_pair(200, "OK"));
	_codes.insert(std::make_pair(201, "Created"));
	_codes.insert(std::make_pair(202, "Accepted"));
	_codes.insert(std::make_pair(203, "Non-Authoritative Information"));
	_codes.insert(std::make_pair(204, "No Content"));
	_codes.insert(std::make_pair(205, "Reset Content"));
	_codes.insert(std::make_pair(206, "Partial Content"));
	_codes.insert(std::make_pair(207, "Multi-Status"));
	_codes.insert(std::make_pair(208, "Already Reported"));
	_codes.insert(std::make_pair(226, "IM Used"));

	/*redirection messages*/
	_codes.insert(std::make_pair(300, "Multiple Choices"));
	_codes.insert(std::make_pair(301, "Moved Permanently"));
	_codes.insert(std::make_pair(302, "Found"));
	_codes.insert(std::make_pair(303, "See Other"));
	_codes.insert(std::make_pair(304, "Not Modified"));
	_codes.insert(std::make_pair(305, "Use Proxy"));
	_codes.insert(std::make_pair(306, "unused")); //reserved???
	_codes.insert(std::make_pair(307, "Temporary Redirect"));
	_codes.insert(std::make_pair(308, "Permanent Redirect"));

	/*client error responses*/
	_codes.insert(std::make_pair(400, "Bad Request"));
	_codes.insert(std::make_pair(401, "Unauthorized"));
	_codes.insert(std::make_pair(402, "Payment Required"));
	_codes.insert(std::make_pair(403, "Forbidden"));
	_codes.insert(std::make_pair(404, "Not Found"));
	_codes.insert(std::make_pair(405, "Method Not Allowed"));
	_codes.insert(std::make_pair(406, "Not Acceptable"));
	_codes.insert(std::make_pair(407, "Proxy Authentication Required"));
	_codes.insert(std::make_pair(408, "Request Timeout"));
	_codes.insert(std::make_pair(409, "Conflict"));
	_codes.insert(std::make_pair(410, "Gone"));
	_codes.insert(std::make_pair(411, "Length Required"));
	_codes.insert(std::make_pair(412, "Precondition Failed"));
	_codes.insert(std::make_pair(413, "Payload Too Large"));
	_codes.insert(std::make_pair(414, "URI Too Long"));
	_codes.insert(std::make_pair(415, "Unsupported Media Type"));
	_codes.insert(std::make_pair(416, "Range Not Satisfiable"));
	_codes.insert(std::make_pair(417, "Expectation Failed"));
	_codes.insert(std::make_pair(418, "I'm a teapot"));
	_codes.insert(std::make_pair(421, "Misdirected Request"));
	_codes.insert(std::make_pair(422, "Unprocessable Entity"));
	_codes.insert(std::make_pair(423, "Locked"));
	_codes.insert(std::make_pair(424, "Failed Dependency"));
	_codes.insert(std::make_pair(425, "Too Early"));
	_codes.insert(std::make_pair(426, "Upgrade Required"));
	_codes.insert(std::make_pair(428, "Precondition Required"));
	_codes.insert(std::make_pair(429, "Too Many Requests"));
	_codes.insert(std::make_pair(431, "Request Header Fields Too Large"));
	_codes.insert(std::make_pair(451, "Unavailable For Legal Reasons"));

	/*server error responses*/
	_codes.insert(std::make_pair(500, "Internal Server Error"));
	_codes.insert(std::make_pair(501, "Not Implemented"));
	_codes.insert(std::make_pair(502, "Bad Gateway"));
	_codes.insert(std::make_pair(503, "Service Unavailable"));
	_codes.insert(std::make_pair(504, "Gateway Timeout"));
	_codes.insert(std::make_pair(505, "HTTP Version Not Supported"));
	_codes.insert(std::make_pair(506, "Variant Also Negotiates"));
	_codes.insert(std::make_pair(507, "Insufficient Storage"));
	_codes.insert(std::make_pair(508, "Loop Detected"));
	_codes.insert(std::make_pair(510, "Not Extended"));
	_codes.insert(std::make_pair(511, "Network Authentication Required"));
}

void Response::CreateAndSetErrorBody()
{
	_body.append("<p style=\"text-align:center;\">");
	_body.append("<b>");
	_body.append("Sorry! Can't load page. Please, contact administrator. Error: ");
	_body.append(_codes[_response_code]);
	_body.append("<b>");
	_body.append("</p>");
	_body.append("\n");
}

void Response::MakeHTTPResponse(int code)
{
	if (!_response_code)
		_response_code = code;
	_response.append("HTTP/1.1 " + CodeToString(_response_code) + " " + _codes[_response_code] + "\n");
	_response.append("Server: server\n");
	_response.append("Date: " + GetDateAndTime());
	if (_response_code >= 400 && _response_code <= 500 )
	{
		_body.clear();
		std::string path_to_default;
		//path_to_default = "./webpages/default_error_pages/" + CodeToString(_response_code) + ".html";
		path_to_default = _error_pages[_response_code];
		std::cout << "error page " << _error_pages[_response_code] << std::endl;
			std::ifstream file;
			std::stringstream buffer;

			file.open(path_to_default.c_str(), std::ifstream::in);
			if (!file.is_open())
			{
				CreateAndSetErrorBody();
			}
			else
			{
				buffer << file.rdbuf();
				_body = buffer.str();
				file.close();
			}
		_content_length = _body.size() + 1;
		SetContentType("text/html");
	}
	_response.append("Content-Type: " + GetContentType() + "\n");
	SetContentLength(_body.size() + 1);
	_response.append("Content-Length: " + GetContentLength() + "\n");
	_response.append("Connection: keep-alive\n");
	_response.append("Accept-Ranges: bytes\n");
	_response.append("\r\n");

	std::cout << "GOT RESPONSE HEAD\n\n"
	<< _response << "\n\n";

	_response.append(_body);
	_response.append("\r\n");
}
std::string Response::CodeToString(int code)
{
	int temp = 0;
	std::string ret = "";
	while (code > 0)
	{
		temp = code % 10;
		ret.insert(ret.begin(), temp + '0');
		code /= 10;
	}
	return ret;
}

std::string Response::GetDateAndTime()
{
	std::string	date_and_time;
	std::time_t current_time = std::time(NULL);

	date_and_time = std::string(std::asctime(std::localtime(&current_time)));
	return date_and_time;
}

std::string Response::GetContentType()
{
	return _content_type;
}

std::string Response::GetContentLength()
{
	return CodeToString(_content_length);
}

std::string Response::GetResponse()
{
	return _response;
}

void Response::SetContentType(const std::string &content_type)
{
	this->_content_type = content_type;
}

void Response::ClearContents()
{
	_response.clear();
	_content_type.clear();
	_content_length = 0;
	_body.clear();
	_response_code = 0;
}

void Response::SetMethods()
{
	_allowed_methods.push_back("GET");
	_allowed_methods.push_back("POST");
	_allowed_methods.push_back("DELETE");
}

void Response::SetContentLength(int length)
{
	_content_length = length;
}


void Response::CheckMethod(RequestConfig &conf)
{
	std::map<std::string, HTTPMethods> MethodMap;
	MethodMap.insert(std::make_pair("GET", &Response::GETMethod));
	MethodMap.insert(std::make_pair("POST", &Response::POSTMethod));
	MethodMap.insert(std::make_pair("DELETE", &Response::DELETEMethod));
	(this->*MethodMap[conf.getMethod()])(conf);
	//HTTPMethods Method[3] = {&Response::GETMethod, &Response::POSTMethod, &Response::DELETEMethod};
	// for (size_t i = 0; i < _allowed_methods.size(); i++)
	// {
	// 	if (!conf.getMethod().compare(_allowed_methods[i]))
	// 	{
	// 		//std::cout << "nethod is" << conf.getMethod() << std::endl;
	// 		(this->*Method[i])(conf);
	// 		return ;
	// 	}
	// }
	// _response_code = 405;
}

void Response::SetResponseCode(int code)
{
	_response_code = code;
}

void Response::SetBody(const std::string &body)
{
	_body = body;
}

int Response::GetResponseCode()
{
	return _response_code;
}

void Response::GETMethod(RequestConfig &ReqConf)
{
	if (ReqConf.getCGIPass().size() > 0)
	{
		/*insert cgi here*/
	}
	else
	{
		GetFileFromServer(ReqConf.getPath());
	}
}

void Response::GetFileFromServer(const std::string &file_name)
{
	std::ifstream file;
	std::stringstream buffer;

	if (IsFile(file_name))
	{
		file.open(file_name.c_str(), std::ifstream::in);
		if (!file.is_open())
		{
			_response_code = 403;
			return ;
		}
		buffer << file.rdbuf();
		_body = buffer.str();
		_response_code = 200;
		file.close();
	}
	else
	{
		_response_code = 404;
		return ;
	}
}

bool Response::IsDir(const std::string &dir)
{
	struct stat stats;
	if (!stat(dir.c_str(), &stats))
	{
		if (stats.st_mode & S_IFDIR)
			return true;
	}
	return false;
}


bool Response::IsFile(const std::string &file_name)
{
	struct stat file_stats;
	if (!stat(file_name.c_str(), &file_stats))
	{
		if (file_stats.st_mode & S_IFREG)
			return true;
	}
	return false;
}


void Response::POSTMethod(RequestConfig &ReqConf)
{
	std::string body_to_save;

	if (ReqConf.getCGIPass().size() > 0)
	{
		/*insert cgi here*/
	}
	else
	{
		body_to_save = ReqConf.getBody();
		_body.clear();
		SaveFile(body_to_save, ReqConf);
	}


}

void Response::SaveFile(const std::string &body, RequestConfig &ReqConf)
{
	std::ofstream file;
	std::string path = ReqConf.getPath();

	
	file.open(path.c_str(), std::ofstream::out | std::ofstream::trunc);
	if (!file.is_open())
	{
		std::cout << path << std::endl;
		_response_code = 403;
	}
	else
	{
		file << body;
		_response_code = 201;
		file.close();
	}
}


void Response::DELETEMethod(RequestConfig &ReqConf)
{
	std::string path = ReqConf.getPath();

	if (IsFile(path))
	{
		if (!remove(path.c_str()))
			_response_code = 204;
		else
			_response_code = 403;
	}
	else
		_response_code = 404;
}


void Response::StartThings(RequestConfig &conf)
{
	SetResponseCode(conf.getCode());
	if (_response_code < 400 && _response_code > 0)
	{
		SetBody(conf.getBody());
		CheckMethod(conf);
	}
	std::cout << conf.getPath() << "error code" << _response_code << std::endl;
	MakeHTTPResponse(GetResponseCode());
	//GetDirectoryListing(conf);
}

void Response::GetDirectoryListing(RequestConfig &conf)
{
	struct stat file_stats;
	DIR *dir;
	struct dirent *ent;

	_path_to_files = conf.getPath();
	dir = opendir(_path_to_files.c_str());
	if (dir)
	{
		while ((ent = readdir(dir)))
		{
			if (ent->d_type == DT_DIR)
				_dirs.push_back(ent->d_name);
			else
				_files.push_back(ent->d_name);
		}
		closedir(dir);
		_response_code = 200;
	}
	else
	{
		_response_code = 403;
		MakeHTTPResponse(_response_code);
	}
}

void Response::ShowDirectoryListing()
{
	std::sort(_dirs.begin(), _dirs.end());
	std::sort(_files.begin(),_files.end());
	_response.clear();
	_response.append("HTTP/1.1 " + CodeToString(_response_code) + " " + _codes[_response_code] + "\n");
	_response.append("Server: server\n");
	_response.append("Date: " + GetDateAndTime());
	SetContentType("text/html");
	_response.append("Content-Type: " + GetContentType() + "\n");

	_body.append("<style type=\"text/css\">\n");
	_body.append(".button{\nborder: none;\nbackground-color: inherit;\npadding: 0px;\nfont-size: 16px;\ncursor: pointer;\ndisplay: inline-block;\n}\n</style>");

	_body.append("<b>");
	_body.append("<h1>Index of files</h1><hr></hr>");
	for (int i = 0; i < _dirs.size(); i++)
	{
		_body.append("<button class=\"button\"<br>");
		_body.append(_dirs[i]);
		_body.append("</button>");
		_body.append("<br>");
	}
	for (int i = 0; i < _files.size(); i++)
	{
		_body.append("<button class=\"button\"<br>");
		_body.append(_files[i]);
		_body.append("</button>");
		_body.append("<br>");
	}
	_body.append("<b>");
	_body.append("</p>");
	_body.append("\n");
	std::cout << _body;
	_content_length = _body.size();
	_response.append("Content-Length: " + GetContentLength() + "\n");
	_response.append("Connection: keep-alive\n");
	_response.append("Accept-Ranges: bytes\n");
	_response.append("\r\n");
	_response.append(_body);
}