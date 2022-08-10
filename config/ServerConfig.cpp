#include "ServerConfig.hpp"

//Constructors, destructor, operator overload

ServerConfig::ServerConfig(): _root(""), _client_buff_size(1000),
_autoindex(false), _alias_set(false) {
	_server_name.push_back("default_server");
}

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
	t_listen	listener;
	std::string	port;
	size_t		sep;

	if (tokens.size() != 1)
		throw ServerConfigException("Invalid listen ip");
	if ((sep = tokens[0].find(':')) == std::string::npos){
		if (onlyDigits(tokens[0])){
			listener.host = 0;
			listener.port = atoi(tokens[0].c_str());
			for (std::vector<t_listen>::const_iterator it = _listen.begin();
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
	for (size_t i = 0; i < tokens.size(); i++)
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
					throw ServerConfigException("Parse Error");
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

std::vector<t_listen>				ServerConfig::getListen() const {return _listen;}
std::string							ServerConfig::getRoot() const {return _root;}
std::vector<std::string>   			ServerConfig::getServerName() const {return _server_name;}
std::map<int, std::string>			ServerConfig::getErrorPage() const { return _error_page;}
int									ServerConfig::getClientBodyBufferSize() const {return _client_buff_size;}
std::map<std::string, std::string>	ServerConfig::getCgiParam() const {return _cgi_param;}
std::string							ServerConfig::getCgiPass() const {return _cgi_pass;}
std::map<std::string, ServerConfig> ServerConfig::getLocation() const {return _location;}
std::set<std::string>				ServerConfig::getAllowedMethods() const {return _methods;}
std::vector<std::string>			ServerConfig::getIndex() const{return _index;}
bool								ServerConfig::getAutoIndex() const {return _autoindex;}
std::string							ServerConfig::getAlias() const {return _alias;}
bool								ServerConfig::getAliasSet() const {return _alias_set;}
ServerConfig						&ServerConfig::getDefaultConfig() {return ServerConfig::_default_config;}

ServerConfig ServerConfig::getRequestLoc(std::string const &path, std::string &ret){
	std::string::size_type len = path.length();
	std::map<std::string, ServerConfig>::iterator it;
	std::string loc;

	if (!len)
		return (*this);
	if(!this->_location.empty()){
		do {
			loc = path.substr(0, len);
			it = this->_location.find(loc);
			if (it != this->_location.end() && it->first[0] != '*'){
				ret = loc;
				return it->second.getRequestLoc(path, ret);
			}
			len--;
		} while(len);
		for (std::map<std::string, ServerConfig>::iterator i = this->_location.begin();
			i != this->_location.end(); i++){
			if (i->first[0] == '*'){
				std::string tmp = i->first.substr(1);
				if (path.length() > tmp.length() && !path.compare(path.length() - tmp.length(),
					tmp.length(), tmp))
					return i->second.getRequestLoc(path, ret);
			}
		}
	}
	return (*this);
}

std::ostream &operator<<(std::ostream &out, ServerConfig const &serv){
	std::vector<t_listen> listen = serv.getListen();
	std::vector<std::string> name = serv.getServerName();
	std::map<int, std::string> err_page = serv.getErrorPage();
	std::map<std::string, std::string> cgi_param = serv.getCgiParam();
	std::set<std::string> methods = serv.getAllowedMethods();
	std::vector<std::string> index = serv.getIndex();
	std::map<std::string, ServerConfig> location = serv.getLocation();

	out << "Listen:\n";
	for (size_t i = 0; i < listen.size(); i++)
		out << "\thost: " << listen[i].host << "\n\tport: " << listen[i].port << std::endl;
	out<<std::endl;
	out << "root: " << serv.getRoot() << std::endl;
	out<<std::endl;
	out << "server name:";
	for (size_t i = 0; i < name.size(); i++)
		out << " " << name[i];
	out << std::endl << std::endl;
	out << "error page:\n";
	for (std::map<int, std::string>::const_iterator it = err_page.begin();
		it != err_page.end(); it++)
		out << "\t" << it->first << " " << it->second << std::endl;
	out<<std::endl;
	out << "client body buffer size: " << serv.getClientBodyBufferSize() << std::endl;
	out<<std::endl;
	out << "allowed methods:";
	for (std::set<std::string>::iterator it = methods.begin(); it != methods.end(); it++)
		out << " " << *it << std::endl;
	out<<std::endl;
	out << "autoindex: " << (serv.getAutoIndex() ? "on": "off") << std::endl;
	out<<std::endl;
	out << "index:\n";
	for(std::vector<std::string>::const_iterator it = index.begin(); it != index.end(); it++)
		out << "\t" << *it << std::endl;
	
	out << "alias: " << serv.getAlias() << std::endl;
	out<<std::endl;
	for (std::map<std::string, ServerConfig>::const_iterator it = location.begin();
		it != location.end(); it++){
		out << std::endl << "location " << it->first << std::endl;
		out << it->second << std::endl;
	}
	return out;
}