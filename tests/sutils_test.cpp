#include "../string_utils.hpp"
#include <iostream>

int main()
{
	std::string s = "foo bar foo bar foo bar";

	std::cout << std::boolalpha;

	std::cout << ends_with(s, "foo bar") << '\n';
	std::cout << ends_with(s, "ffoo bar") << '\n';
	std::cout << ends_with(s, "foo bad") << '\n';
	std::cout << ends_with(s, "foo\tbad") << '\n';
	std::cout << ends_with(s, "bar foo bar") << '\n';
}
