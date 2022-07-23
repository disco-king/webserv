#include <string>
#include <iostream>
#include <vector>
#include <fcntl.h>
#include <unistd.h>

#define BUFF_SIZE 1024

class ReadConfig{
	public:
		ReadConfig();
		ReadConfig(ReadConfig const &ref);
		~ReadConfig();

		ReadConfig &operator=(ReadConfig const &);

		std::vector<std::string> filereader(const char *fname);
		std::vector<std::string> splitline(std::string);
		
		class ReadConfigException: public std::exception{
			virtual const char *what() const throw();
		};
};