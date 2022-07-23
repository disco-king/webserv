#include "ReadConfig.hpp"

ReadConfig::ReadConfig(){}

ReadConfig::ReadConfig(ReadConfig const &ref){
	if (this != &ref)
		*this = ref;
}

ReadConfig::~ReadConfig(){}

ReadConfig &ReadConfig::operator=(ReadConfig const &ref){
	if (this != &ref)
		*this = ref;
	return *this;
}

const char *ReadConfig::ReadConfigException::what() const throw(){
	return "ReadConfig Exception: Can't open configuration file";
}

std::vector<std::string> ReadConfig::splitline(std::string line){
	std::string delim = " \n\t";
	std::vector<std::string> tokens;

	line.append(delim);
	std::string::size_type start = line.find_last_not_of(delim, 0);
	std::string::size_type end = 0;
	while(1){
		end = line.find_first_of(delim, start);
		if (end == std::string::npos)
			break;
		std::string subline = line.substr(start, end - start);
		tokens.push_back(subline);
		if ((start = line.find_first_not_of(delim, end)) == std::string::npos)
			break;
	}
	return tokens;
}

std::vector<std::string> ReadConfig::filereader(const char *fname){
	int		ret = BUFF_SIZE;
	char 	buff[BUFF_SIZE + 1];
	std::string line = "";
	int fd;
	std::vector<std::string> file;

	for (int i=0; i < BUFF_SIZE + 1 ; i++)
		buff[i] = '\0';
	if ((fd = open(fname, O_RDONLY)) <= 0)
		throw ReadConfig::ReadConfigException();
	for (ret = BUFF_SIZE; ret > 0; ret = read(fd, buff, BUFF_SIZE)){
		buff[ret] = '\0';
		line.append(buff);
	}
	if (ret == -1)
		throw ReadConfig::ReadConfigException();
	file = ReadConfig::splitline(line);
	return file;
}
