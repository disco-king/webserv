#include <string>
#include <iostream>

std::string	removeAdjacentSlashes(const std::string &str) {
	std::string	ret;
	bool		lastIsSlash = false;

	for (std::string::size_type i = 0; i < str.length(); i++) {
		if (str[i] == '/') {
			if (!lastIsSlash)
				ret.push_back(str[i]);
			lastIsSlash = true;
		}
		else {
			lastIsSlash = false;
			ret.push_back(str[i]);
		}
	}
	return ret;
}

void removeSlashes(std::string &str, size_t index = 0, size_t shift = 0)
{
	int count = -1;
	for(; index < str.size(); ++index){
		if(str[index] == '/')
			++count;
		else if(count >= 0)
			break;
	}
	shift += count;
	count = --index;
	for(; index < str.size(); ++index){
		if(str[index] == '/' && index != count)
			return(removeSlashes(str, index, shift));
		str[index - shift] = str[index];
	}
	str.resize(str.size() - shift);
}

int main()
{
	// std::string str = "/////some / slashes / here ////// // //";
	// std::string str = "some // slashes // here ";
	//std::string str = "// // //";
	std::string str = "some / slashes / here ";

	// str = removeAdjacentSlashes(str);
	removeSlashes(str);
	std::cout << "size: " << str.size() << '\n'
	<< '[' << str << ']' << '\n';
}
