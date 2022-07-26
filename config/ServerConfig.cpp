#include "ServerConfig.hpp"

// 

//Constructors, destructor, operator overload

ServerConfig::ServerConfig(): _root(""), _client_buff_size(0),
_autoindex(false), _alias_set(false) {}

ServerConfig::ServerConfig(ServerConfig const &ref){
	if (this != &ref){
		this->_listen = ref._listen;
		this->_root = ref._root;
		this->_server_name = ref._server_name;
		this->_error_page = ref._error_page;
		this->_client_buff_size = ref._client_buff_size;
		this->_cgi_param = ref._cgi_param;
		this->_cgi_pass = ref._cgi_pass;
		this->_location = ref._location;
		this->_methods = ref._methods;
		this->_autoindex = ref._autoindex;
		this->_index = ref._index;
		this->_alias = ref._alias;
		this->_alias_set = ref._alias_set;
	}
}

ServerConfig::~ServerConfig(){}

ServerConfig &ServerConfig::operator=(ServerConfig const &ref){
		if (this != &ref){
		this->_listen = ref._listen;
		this->_root = ref._root;
		this->_server_name = ref._server_name;
		this->_error_page = ref._error_page;
		this->_client_buff_size = ref._client_buff_size;
		this->_cgi_param = ref._cgi_param;
		this->_cgi_pass = ref._cgi_pass;
		this->_location = ref._location;
		this->_methods = ref._methods;
		this->_autoindex = ref._autoindex;
		this->_index = ref._index;
		this->_alias = ref._alias;
		this->_alias_set = ref._alias_set;
	}
	return *this;
}

ServerConfig ServerConfig::_default_config = ServerConfig();

//ServerConfigException 

ServerConfig::ServerConfigException::ServerConfigException(std::string const &msg) {
	_msg = "ServerConfig Exception: ";
	_msg.append(msg);
}

const char *ServerConfig::ServerConfigException::what() const throw() {
	return _msg.c_str();
}

// init maps

configmap ServerConfig::initServMap(){
	configmap servermap;

	servermap["listen"] = &ServerConfig::addListen;
	servermap["root"] = &ServerConfig::addRoot;
	servermap["server_name"] = &ServerConfig::addServerName;
	servermap["error_page"] = &ServerConfig::addErrorPage;
	servermap["client_body_buff_size"] = &ServerConfig::addClientBuffSize;
	servermap["cgi_param"] = &ServerConfig::addCgiParam;
	servermap["cgi_pass"] = &ServerConfig::addCgiPass;
	servermap["allow_methods"] = &ServerConfig::addMethods;
	servermap["index"] = &ServerConfig::addIndex;
	servermap["autoindex"] = &ServerConfig::addAutoIndex;
	return servermap;
}

configmap ServerConfig::initLocMap(){
	configmap locationmap;

	locationmap["root"] = &ServerConfig::addRoot;
	locationmap["error_page"] = &ServerConfig::addErrorPage;
	locationmap["client_body_buff_size"] = &ServerConfig::addClientBuffSize;
	locationmap["cgi_param"] = &ServerConfig::addCgiParam;
	locationmap["cgi_pass"] = &ServerConfig::addCgiPass;
	locationmap["allow_methods"] = &ServerConfig::addMethods;
	locationmap["index"] = &ServerConfig::addIndex;
	locationmap["autoindex"] = &ServerConfig::addAutoIndex;
	locationmap["alias"] = &ServerConfig::addAlias;
	return locationmap;
}

configmap ServerConfig::_servermap = ServerConfig::initServMap();
configmap ServerConfig::_locmap = ServerConfig::initLocMap();

// add functions

void ServerConfig::addListen(std::vector<std::string> tokens){
	t_listener	listener;
	std::string	port;
	size_t		sep;

	if (tokens.size() != 1)
		throw ServerConfigException("Invalid listen ip");
	if ((sep = tokens[0].find(':')) == std::string::npos){
		if (onlyDigits(tokens[0])){
			listener.host = 0;
			listener.port = atoi(tokens[0].c_str());
			for (std::vector<t_listener>::const_iterator it = _listen.begin();
				it != _listen.end(); it++) {
					if (it->port == listener.port)	
						throw ServerConfigException("Invalid listen ip");
				}
			_listen.push_back(listener);
		}
		else
			throw ServerConfigException("Invalid listen ip");
	}
	else {
		listener.host = strtoip(tokens[0].substr(0, sep));
		sep++;
		port = tokens[0].substr(sep);
		if (onlyDigits(port)){
			listener.port = atoi(port.c_str());
			_listen.push_back(listener);
		}
		else
			throw ServerConfigException("Invalid listen port");
	}
}

void ServerConfig::addRoot(std::vector<std::string> tokens){
	if (tokens.size() != 1 || _root != "")
		throw ServerConfigException("Invalid root");
	_root = tokens[0];
}

void ServerConfig::addServerName(std::vector<std::string> tokens){
	if (tokens.size() == 0)
		throw ServerConfigException("Invalid server name");
	for (int i = 0; i < tokens.size(); i++)
		_server_name.push_back(tokens[0]);
}

void ServerConfig::addErrorPage(std::vector<std::string> tokens){
	std::vector<int> status_codes;
	std::string uri = "";
	size_t		len = tokens.size();

	for (size_t i = 0; i < len; i++){
		if (onlyDigits(tokens[i]))
			status_codes.push_back(atoi(tokens[i].c_str()));
		else if (status_codes.empty())
			throw ServerConfigException("Invalid status codes");
		else if (i == len - 1)
			uri = tokens[i];
		else
			throw ServerConfigException("Invalid status codes");
	}
	if (uri == "")
		throw ServerConfigException("Invalid URI");
	for (std::vector<int>::iterator it = status_codes.begin();
		it != status_codes.end(); it++)
		_error_page[*it] = uri;
}

void ServerConfig::addClientBuffSize(std::vector<std::string> tokens){
	if (tokens.size() != 1 || !onlyDigits(tokens[0]))
		throw ServerConfigException("Invalid Client Body Buffer Size");
	_client_buff_size = atoi(tokens[0].c_str());
}

void ServerConfig::addCgiParam(std::vector<std::string> tokens){
	if (tokens.size() != 2)
		throw ServerConfigException("Invalid CGI parameters");
	_cgi_param[tokens[0]] = tokens[1];
}

void ServerConfig::addCgiPass(std::vector<std::string> tokens){
	if (tokens.empty())
		throw ServerConfigException("Invalid CGI password");
	_cgi_pass = tokens[0];
}

void ServerConfig::addMethods(std::vector<std::string> tokens){
	if (tokens.empty())
		throw ServerConfigException("Invalid Allowed Methods");
	_methods.clear();
	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++)
		_methods.insert(*it);
}

void ServerConfig::addIndex(std::vector<std::string> tokens){
	if (tokens.empty())
		throw ServerConfigException("Invalid Index");
	_index.insert(_index.end(), tokens.begin(), tokens.end());
}

void ServerConfig::addAutoIndex(std::vector<std::string> tokens){
	if (tokens.size() != 1)
		throw ServerConfigException("Invalid Auto Index");
	if (tokens[0] == "on")
		_autoindex = true;
	else if (tokens[0] == "off")
		_autoindex = false;
	else
		throw ServerConfigException("Invalid Autoindex");
}

void ServerConfig::addAlias(std::vector<std::string> tokens){
	if (tokens.size() > 1)
		throw ServerConfigException("Invalid Alias");
	if (tokens.size())
		_alias = tokens[0];
	_alias_set = true;
}

ServerConfig ServerConfig::DefaultServerConfig(std::vector<std::string> &file){
	ServerConfig serv;
	unsigned int i = 2;
	
	if (file.empty())
		throw ServerConfigException("Invalid default configuration file");
	std::vector<std::string> start;
	start.push_back("server");
	start.push_back("{");
	file.insert(file.begin(), start.begin(), start.end());
	file.insert(file.end(), "}");
	serv.parse_server(i, file);
	ServerConfig::_default_config = serv;
	return serv;
}

// parse methods

void ServerConfig::pass_members(ServerConfig &serv) const{
	if (this != &serv){
		if (serv._listen.empty()){
			serv._listen.insert(serv._listen.begin(), this->_listen.begin(),
				this->_listen.end());
		}
		if (serv._root == "")
			serv._root = this->_root;
		serv._server_name.insert(serv._server_name.end(), 
			this->_server_name.begin(), this->_server_name.end());
		for (std::map<int, std::string>::const_iterator it = this->_error_page.begin();
			it != this->_error_page.end(); it++){
			if (serv._error_page.find(it->first) == serv._error_page.end())
				serv._error_page[it->first] = it->second;
		}
		if (serv._client_buff_size == 0)
			serv._client_buff_size = this->_client_buff_size;
		for (std::map<std::string, std::string>::const_iterator it = this->_cgi_param.begin();
			it != this->_cgi_param.end(); it++){
			if (serv._cgi_param.find(it->first) == serv._cgi_param.end())
				serv._cgi_param[it->first] = it->second;
		}
		if (serv._cgi_pass == "")
			serv._cgi_pass = this->_cgi_pass;
		if (serv._methods.empty())
			serv._methods = this->_methods;
		serv._index.insert(serv._index.begin(), this->_index.begin(), this->_index.end());
	}
	for (std::map<std::string, ServerConfig>::iterator it = serv._location.begin();
		it != serv._location.end(); it++)
		serv.pass_members(it->second);
}	

void ServerConfig::parse_loc(unsigned int &i, std::vector<std::string> &file){
	std::vector<std::string> tokens;
	configmap::iterator	it;
	std::string	cmd = "";

	if (file[i++] != "{")
		throw ServerConfigException("Parse Error");
	for (; i < file.size() && file[i] != "}"; i++){
		if ((it = _locmap.find(file[i])) == _locmap.end()){
			if (file[i] == "location"){
				ServerConfig loc;
				std::string name;
				if (cmd != ""){
					(this->*ServerConfig::_locmap[cmd])(tokens);
					tokens.clear();
					cmd = "";
				}
				i++;
				if (file[i] == "{" || file[i] == "}")
					throw ServerConfigException("Parse Error");
				name = file[i];
				i++;
				loc.parse_loc(i, file);
				this->_location[name] = loc;
				if (file[i] == "}")
					continue;
			}
			else if (cmd == ""){
				if (file[i] != "}")
					throw ServerConfigException("Parse Here Error");
				return;
			}
			else
				tokens.push_back(file[i]);
		}
		else {
			if (cmd != ""){
				(this->*ServerConfig::_locmap[cmd])(tokens);
				tokens.clear();
			}
			cmd = it->first;
		}
	}
	if (cmd != "")
		(this->*ServerConfig::_locmap[cmd])(tokens);
	if (!file[i].compare("}"))
		return;
	throw ServerConfigException("Parse Error");
}

void ServerConfig::parse_server(unsigned int &i, std::vector<std::string> &file){
	std::vector<std::string> tokens;
	configmap::iterator	it;
	std::string	cmd;

	for (; i < file.size() && file[i] != "}"; i++){
		if ((it = _servermap.find(file[i])) == _servermap.end()){
			if (file[i] == "location"){
				ServerConfig	loc;
				std::string		name;
				if (cmd != ""){
					(this->*ServerConfig::_servermap[cmd])(tokens);
					tokens.clear();
					cmd = "";
				}
				i++;
				if (file[i] == "{" || file[i] == "}")
					throw ServerConfigException("Parse Server Error");
				name = file[i];
				i++;
				loc.parse_loc(i, file);
				this->_location[name] = loc;
				if (file[i] == "}")
					continue;
			}
			else if (!cmd.compare("")){
				if (file[i] != "}")
					throw ServerConfigException("Parse Server Error");
				return;
			}
			else
				tokens.push_back(file[i]);
		}
		else {
			if (cmd != ""){
				(this->*ServerConfig::_servermap[cmd])(tokens);
				tokens.clear();
			}
			cmd = it->first;
		}
	}
	if (cmd != "")
		(this->*ServerConfig::_servermap[cmd])(tokens);
	if (!file[i].compare("}")){
		_default_config.pass_members(*this);
		for (std::map<std::string, ServerConfig>::iterator it = this->_location.begin();
			it != this->_location.end(); it++)
			this->pass_members(it->second);
		return ;
	}
	throw ServerConfigException("Parse Server Error");
}

std::vector<t_listener>				ServerConfig::getListen() const {return _listen;}
std::string							ServerConfig::getRoot() const {return _root;}
std::vector<std::string>   			ServerConfig::getServerName() const {return _server_name;}
std::map<int, std::string>			ServerConfig::getErrorPage() const {return _error_page;}
int									ServerConfig::getClientBodyBufferSize() const {return _client_buff_size;}
std::map<std::string, std::string>	ServerConfig::getCgiParam() const {return _cgi_param;}
std::string							ServerConfig::getCgiPass() const {return _cgi_pass;}
std::map<std::string, ServerConfig> ServerConfig::getLocation() const {return _location;}
std::set<std::string>				ServerConfig::getAllowedMethods() const {return _methods;}
std::vector<std::string>			ServerConfig::getIndex() const{return _index;}
bool								ServerConfig::getAutoIndex() const {return _autoindex;}
std::string							ServerConfig::getAlias() const {return _alias;}
bool								ServerConfig::getAliasSet() const {return _alias_set;}
ServerConfig						&ServerConfig::getDefaultConfig() {return _default_config;}