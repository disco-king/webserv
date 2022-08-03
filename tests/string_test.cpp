#include <iostream>

int main()
{
	std::string str;

	std::cout << "string: " << '[' << str << ']'
		<< " size: " << str.size()
		<< " length: " << str.length() << '\n';

	std::string add = "abc";
	for(int i = 0; i < 3; ++i)
		str.push_back(add[i]);

	std::cout << "string: " << '[' << str << ']'
		<< " size: " << str.size()
		<< " length: " << str.length() << '\n';

	for(int i = 0; i < 3; ++i)
		str.push_back(0);

	std::cout << "string: " << '[' << str << ']'
		<< " size: " << str.size()
		<< " length: " << str.length() << '\n';

	add = "foo";
	for(int i = 0; i < 3; ++i)
		str.push_back(add[i]);

	std::cout << "string: " << '[' << str << ']'
		<< " size: " << str.size()
		<< " length: " << str.length() << '\n';

	std::cout << "foo at: " << str.find("foo") << '\n';

	// doesn't work like this
	// str = "";
	// char buff[] = {'a', 'b', 'c', 0, 0, 0, 'f', 'o', 'o'};
	// str += buff;

	for(int i = 0; i < str.size(); ++i){
		std::cout << "str at index " << i
			<< " is " << '<' << int(str[i]) << '>' << '\n';
	}

	std::string sub_str = str.substr(2);

	std::cout << "substring: " << '[' << sub_str << ']'
		<< " size: " << sub_str.size()
		<< " length: " << sub_str.length() << '\n';

	for(int i = 0; i < sub_str.size(); ++i){
		std::cout << "str at index " << i
			<< " is " << '<' << int(sub_str[i]) << '>' << '\n';
	}
}