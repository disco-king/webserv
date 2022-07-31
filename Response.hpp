#pragma once
#include "header.hpp"
#include "Request.hpp"
#include "./config/RequestConfig.hpp"

class Response
{
	private:
		std::string _response;
		//bool _is_error;
		std::map<int, std::string> _codes;
		std::string _content_type;
		std::string _body;
		std::vector<std::string> _allowed_methods;
		int _content_length;
		int _response_code;
	public:
		Response();
		Response(std::string content_type, int content_length, std::string body);
		~Response();
		Response( Response const &other );
		Response & operator=( Response const &other );

		/*http status codes*/
		void FillCodes();
		void SetMethods();

		int GetResponseCode();
		void SetResponseCode(int code);
		void CheckMethod(const std::string &method);

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

		/*Methods*/
		void GETMethod(Request &Req, RequestConfig &ReqConf);
		void GetFileFromServer(const std::string &file_name);
		bool IsFile(const std::string &file_name);

		void POSTMethod(Request &Req, RequestConfig &ReqConf);
		void SaveFile(const std::string &body, Request &Req);

		void DELETEMethod(Request &Req);
};
