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
	bool slashes = false;
	int count = -1;
	for(; index < str.size(); ++index){
		if(str[index] == '/'){
			slashes = true;
			++count;
		}
		else if(slashes)
			break;
	}
	shift += count;
	--index;
	slashes = false;
	while(index < str.size()){
		if(str[index] == '/'){
			if(slashes)
				return(removeSlashes(str, index, shift));
			slashes = true;
		}
		str[index - shift] = str[index];
		index++;
	}
	str.resize(str.size() - shift);
}

int main()
{
	// std::string str = "/////some / slashes / here ////// // //";
	std::string str = "some / slashes / here ";
	// std::string str = "// // //";

	// str = removeAdjacentSlashes(str);
	removeSlashes(str);
	std::cout << "size: " << str.size() << '\n'
	<< '[' << str << ']' << '\n';
}