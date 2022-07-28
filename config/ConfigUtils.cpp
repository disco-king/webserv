#include "ConfigUtils.hpp"

bool onlyDigits(std::string const &str){
	return str.find_first_not_of("0123456789") == std::string::npos;
}

unsigned int strtoip(std::string str){
	size_t sep = 0;
	std::string tmp;
	unsigned int	ip_int;
	unsigned char	ip_char[4];

	size_t start = 0;
	if (str == "localhost")
		str = "127.0.0.1";
	for (unsigned int i = 3; i != std::numeric_limits<uint32_t>::max(); i--){
		sep = str.find_first_of('.', sep);
		tmp = str.substr(start, sep);
		ip_int = atoi(tmp.c_str());
		ip_char[i] = static_cast<unsigned char>(ip_int);
		sep++;
		start = sep;
	}
	return *(reinterpret_cast<unsigned int *>(ip_char));
}