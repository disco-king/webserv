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

void slashes(std::string &str)
{
	size_t shift, resize = 0;
	std::string::iterator inner, end = str.end();

	for(std::string::iterator it = str.begin();
			it + resize < end; ++it){
		shift = 0;
		while(*(it+shift) == '/' && it+shift < end)
			++shift;
		if(shift > 1){
			--shift;
			for(inner = it + shift + 1; inner < end; ++inner)
				*(inner - shift) = *inner;
			resize += shift;
		}
	}
	str.resize(str.size() - resize);
}

int main()
{
	// std::string str = "/////some / slashes / here ////// // //";
	std::string str = "// // //";

	// str = removeAdjacentSlashes(str);
	slashes(str);
	std::cout << "size: " << str.size() << '\n'
	<< str << '\n';
}