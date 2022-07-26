#include "Config.hpp"

Config::Config(): _path(DEFAULT_CONFIG_PATH){}

Config::Config(std::string const &path): _path(path) {}

Config::~Config(){}

void Config::setPath(std::string const &path){
	_path = path;
}

Config::ConfigException::ConfigException(std::string const &msg){
	_msg = "Config Exception: ";
	_msg.append(msg);
}

const char *Config::ConfigException::what() const throw(){
	return _msg.c_str();
}

std::vector<std::string> Config::splitline(std::string line){
	std::string delim = " \n\t";
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

// for testing purposes

void Config::print_file() const{
	for (int i = 0; i < _file.size(); i++)
		std::cout << _file.at(i) << '\n';
}