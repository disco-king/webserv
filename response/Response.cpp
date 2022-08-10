#include "Response.hpp"
#include "../utils/utils.hpp"

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
	this->_path_to_files = other._path_to_files;
	this->_response = other._response;
	this->_is_cgi = other._is_cgi;
	this->_codes = other._codes;
	this->_content_type = other._content_type;
	this->_body = other._body;
	this->_allowed_methods = other._allowed_methods;
	this->_files = other._files;
	this->_dirs = other._dirs;
	this->_error_pages = other._error_pages;
	this->_path_to_file = other._path_to_file;
	this->_file_length = other._file_length;
	this->_error_string = other._error_string;
	this->_server_name = other._server_name;
	this->_content_length = other._content_length;
	this->_response_code = other._response_code;
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
	_error_string.append("<p style=\"text-align:center;\">");
	_error_string.append("<b>");
	_error_string.append("Sorry! Can't load page. Please, contact administrator. Error: ");
	_error_string.append(_codes[_response_code]);
	_error_string.append("<b>");
	_error_string.append("</p>");
	_error_string.append("\n");
}

void Response::MakeHTTPResponse(int code)
{
	if (!_response_code)
		_response_code = code;
	_response.append("HTTP/1.1 " + CodeToString(_response_code) + " " + _codes[_response_code] + "\n");
	_response.append("Server: " + GetServerName() + "\n");
	_response.append("Date: " + GetDateAndTime());
	struct stat file_stat;
	stat(_path_to_file.c_str(), &file_stat);
	_file_length = file_stat.st_size;
	if (_response_code >= 400 && _response_code <= 500 )
	{
		_body.clear();
		std::string path_to_default;
		path_to_default = _error_pages[_response_code];
		std::ifstream file;
		struct stat file_stat;
		char *buffer2 = new char[512];
		file.open(path_to_default.c_str(), std::ifstream::in);
		if (!file.is_open())
		{
			std::ofstream outFile;
			std::string tmp_path = "./temp_files/error_page.html";
			outFile.open(tmp_path.c_str());
			
			CreateAndSetErrorBody();
			outFile << _error_string;
			getcwd(buffer2, 512);
			tmp_path.clear();
			tmp_path.append(buffer2);
			tmp_path.append("/temp_files/error_page.html");
			outFile.close();
			stat(tmp_path.c_str(), &file_stat);
			_file_length = file_stat.st_size;

			_body.append("file_abs_path:");
			_body.append(buffer2);
			_body.append("/temp_files/error_page.html");

		}
		else
		{
			file.close();
			stat(path_to_default.c_str(), &file_stat);
			_file_length = file_stat.st_size;
			getcwd(buffer2, 512);
			_body.append("file_abs_path:");
			_body.append(buffer2);
			_body.append("/");
			_body.append(path_to_default);
		}
		delete[] buffer2;
		SetContentType("text/html");
	}
	_response.append("Content-Type: " + GetContentType() + "\n");
	SetContentLength(_file_length);
	_response.append("Content-Length: " + GetContentLength() + "\n");
	_response.append("Connection: keep-alive\n");
	_response.append("Accept-Ranges: bytes\r\n");
	_response.append("\r\n");

	_response.append(_body);
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
	GetFileFromServer(ReqConf.getPath());
}

void Response::GetFileFromServer(const std::string &file_name)
{
	std::ifstream file;

	if (is_filename(file_name))
	{
		file.open(file_name.c_str(), std::ifstream::in);
		if (!file.is_open())
		{
			_response_code = 403;
			return ;
		}

		_body.append("file_abs_path:");
		_body.append(_path_to_file);
		_response_code = 200;
		file.close();
	}
	else
	{
		_response_code = 404;
		return ;
	}
}


void Response::POSTMethod(RequestConfig &ReqConf)
{
	std::string body_to_save;
	body_to_save = ReqConf.getBody();
	_body.clear();
	SaveFile(body_to_save, ReqConf);
}

void Response::SaveFile(const std::string &body, RequestConfig &ReqConf)
{
	std::ofstream file;
	std::string path = ReqConf.getPath();

	
	file.open(path.c_str(), std::ofstream::out | std::ofstream::trunc);
	if (!file.is_open())
	{
		_response_code = 403;
	}
	else
	{
		file << body;
		_response_code = 201;
		_file_length = 0;
		file.close();
	}
}


void Response::DELETEMethod(RequestConfig &ReqConf)
{
	std::string path = ReqConf.getPath();

	if (is_filename(path))
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
	MakeHTTPResponse(GetResponseCode());
}

void Response::GetDirectoryListing(RequestConfig &conf)
{
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
	_response.append("Server: " + GetServerName() + "\n");
	_response.append("Date: " + GetDateAndTime());
	SetContentType("text/html");
	_response.append("Content-Type: " + GetContentType() + "\n");
	_body.append("<style type=\"text/css\">\n");
	_body.append(".button{\nborder: none;\nbackground-color: inherit;\npadding: 0px;\nfont-size: 16px;\ncursor: pointer;\ndisplay: inline-block;\n}\n</style>");

	_body.append("<b>");
	_body.append("<h1>Index of files</h1><hr></hr>");
	for (size_t i = 0; i < _dirs.size(); i++)
	{
		_body.append("<button class=\"button\"<br>");
		_body.append(_dirs[i]);
		_body.append("</button>");
		_body.append("<br>");
	}
	for (size_t i = 0; i < _files.size(); i++)
	{
		_body.append("<button class=\"button\"<br>");
		_body.append(_files[i]);
		_body.append("</button>");
		_body.append("<br>");
	}
	_body.append("<b>");
	_body.append("</p>");
	_body.append("\n");
	_content_length = _body.size();
	_response.append("Content-Length: " + GetContentLength() + "\n");
	_response.append("Connection: keep-alive\n");
	_response.append("Accept-Ranges: bytes\r\n");
	_response.append("\r\n");
	_response.append(_body);
}

void Response::SetPathToFile(const std::string &path)
{
	_path_to_file = path;
}

void Response::SetServerName(const std::string &name)
{
	_server_name = name;
}

std::string Response::GetServerName()
{
	return _server_name;
}

std::string Response::getCodeRep(int code)
{
	if(!_codes.count(code))
		return "OK";
	return _codes[code];
}