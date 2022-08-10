#pragma once
#include "header.hpp"
#include "../request/Request.hpp"
#include "../config/RequestConfig.hpp"

class Response
{
	private:
		std::string _response;
		std::string _path_to_files;
		bool _is_cgi;
		std::map<int, std::string> _codes;
		std::string _content_type;
		std::string _body;
		std::vector<std::string> _allowed_methods;
		std::vector<std::string> _files;
		std::vector<std::string> _dirs;
		std::map<int, std::string> _error_pages;
		std::string _path_to_file;
		size_t _file_length;
		std::string _error_string;
		std::string _server_name;
		int _content_length;
		int _response_code;
		Response();
	public:
		Response(std::string content_type, int content_length, std::string body);
		~Response();
		Response( Response const &other );
		Response & operator=( Response const &other );

		/*http status codes*/
		void FillCodes();
		void SetMethods();

		int GetResponseCode();
		void SetResponseCode(int code);
		void CheckMethod(RequestConfig &conf);

		void MakeHTTPResponse(int code);
		std::string CodeToString(int code);
		std::string GetDateAndTime();
		std::string GetContentType();
		std::string GetContentLength();

		void SetContentLength( int length );
		void SetContentType(const std::string &content_type);
		void SetBody(const std::string &body);

		std::string GetResponse();
		void ClearContents();
		std::string getCodeRep(int code);

		/*Methods*/
		void GETMethod(RequestConfig &ReqConf);
		void GetFileFromServer(const std::string &file_name);

		void POSTMethod(RequestConfig &ReqConf);
		void SaveFile(const std::string &body, RequestConfig &ReqConfig);

		void DELETEMethod(RequestConfig &ReqConf);

		void StartThings(RequestConfig &conf);

		void GetDirectoryListing(RequestConfig &conf);
		void ShowDirectoryListing();

		bool GetIsCGI();
		void SetIsCGI(bool value);

		void CreateAndSetErrorBody();

		void SetDefaultErrorPages(const std::map<int, std::string> &pages);
		std::map<int, std::string> GetDefaultErrorPages();

		void SetPathToFile(const std::string &path);

		void SetServerName(const std::string &name);
		std::string GetServerName();
		
};

typedef void (Response::*HTTPMethods)(RequestConfig &conf);