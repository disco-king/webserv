#include "Config.hpp"
#include "RequestConfig.hpp"


Config::Config(): _path(DEFAULT_CONFIG_PATH){}

Config::Config(std::string const &path): _path(path) {}

Config::~Config(){}

void Config::setPath(std::string const &path){
	_path = path;
}

std::vector<ServerConfig> Config::getServers(){
	return _servers;
}

Config::ConfigException::ConfigException(std::string const &msg){
	_msg = "Config Exception: ";
	_msg.append(msg);
}

const char *Config::ConfigException::what() const throw(){
	return _msg.c_str();
}

std::vector<std::string> Config::splitline(std::string line){
	std::string delim = " \r\n\t";
	std::vector<std::string> file;

	line += delim[0];
	std::string::size_type start = line.find_first_not_of(delim, 0);
	std::string::size_type end = 0;
	while(1){
		end = line.find_first_of(delim, start);
		if (end == std::string::npos)
			break;
		std::string subline = line.substr(start, end - start);
		file.push_back(subline);
		if ((start = line.find_first_not_of(delim, end)) == std::string::npos)
			break;
	}
	return file;
}

std::vector<std::string> Config::filereader(const char *fname){
	int		ret = BUFF_SIZE;
	char 	buff[BUFF_SIZE + 1];
	std::string line = "";
	int fd;
	std::vector<std::string> file;

	for (int i=0; i < BUFF_SIZE + 1 ; i++)
		buff[i] = '\0';
	if ((fd = open(fname, O_RDONLY)) <= 0)
		throw ConfigException("Could not read a file");
	for (ret = BUFF_SIZE; ret > 0; ret = read(fd, buff, BUFF_SIZE)){
		buff[ret] = '\0';
		line.append(buff);
	}
	if (ret == -1)
		throw ConfigException("Could not parse a file");
	file = Config::splitline(line);
	close(fd);
	return file;
}

void Config::parse(){
	_file = filereader(_path.c_str());
	unsigned int fsize = _file.size();
	for (unsigned int i = 0; i < fsize; i++){
		if (_file[i] == "server") {
			i++;
			ServerConfig serv;
			if (_file[i] != "{")
				throw ConfigException("Expected a '{' after 'server'");
			i++;
			serv.parse_server(i, _file);
			_servers.push_back(serv);
		}
		else
			throw ConfigException("Unsupported directive ");
	}
}

RequestConfig Config::getConfigForRequest(t_listen const listen, Request &req) const{
	ServerConfig serv;
	std::string uri = req.getPath();
	std::string host;
	if(req.getCode() >= 400)
		host = "default_server";
	else
		host = req.getHeaders().at("host");
	
	std::string method = req.getMethod();
	std::string locpath;

	host = host.substr(0, host.find_last_of(':'));
	this->getServerForRequest(serv, listen, host);
	serv = serv.getRequestLoc(uri, locpath);
	if (*(--locpath.end()) == '/')
		locpath.resize(locpath.size() - 1);
	RequestConfig reqConf(serv, req, locpath);
	reqConf.setAddrData(listen);
	return reqConf;
}

void Config::getServerForRequest(ServerConfig &ret, t_listen const address,
	std::string const hostName) const{
	
	std::vector<ServerConfig>	possibleServers;


	for (std::vector<ServerConfig>::const_iterator serversIter = _servers.begin() ; serversIter != _servers.end(); serversIter++) {
		std::vector<t_listen>	listens = serversIter->getListen();
		for (std::vector<t_listen>::iterator listenIter = listens.begin(); listenIter != listens.end(); listenIter++) {
			if (address.host == (*listenIter).host && address.port == (*listenIter).port) {
				possibleServers.push_back((*serversIter));
			}
		}
	}
	if (possibleServers.empty())
		return ;
	for (std::vector<ServerConfig>::iterator serversIter = possibleServers.begin() ; serversIter != possibleServers.end(); serversIter++) {
		std::vector<std::string>	serverNames = serversIter->getServerName();
		for (std::vector<std::string>::iterator servNameIter = serverNames.begin() ; servNameIter != serverNames.end(); servNameIter++) {
			if (*servNameIter == hostName) {
				ret = *serversIter;
				return;
			}
		}
	}
	ret = possibleServers[0];
}

std::vector<t_listen> Config::getListeners() const {
	std::vector<t_listen> listeners;

	for (std::vector<ServerConfig>::const_iterator it = _servers.begin();
			it != _servers.end(); ++it){
		std::vector<t_listen> vec = it->getListen();
		listeners.insert(listeners.end(), vec.begin(), vec.end());
	}
	return listeners;
}

// for testing purposes

void Config::print_file() const{
	for (size_t i = 0; i < _file.size(); i++)
		std::cout << _file.at(i) << '\n';
}

void Config::print_config() const{
	for (std::vector<ServerConfig>::const_iterator it = _servers.begin();
		it != _servers.end(); it++)
		std::cout << *it;
}	