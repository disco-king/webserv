#include "../string_utils.hpp"
#include <iostream>

int main()
{
	std::string haystack = "header is Transfer-Encoding: Chunked";
	std::string needle1 = "Transfer-Encoding: Chunked";
	std::string needle2 = "transfer-encoding: chunked";
	std::string needle3 = "TRANSFER-ENCODING: CHUNKED";
	std::string needle4 = "Tronsfer-Encoding: Chunked";
	std::string needle5 = "transfer-encoding : chunked";
	std::string needle6 = "TRANSFERMENCODING: CHUNKED";

	std::cout << "identical: " << find_string(haystack, needle1) << '\n';
	std::cout << "lowecase: " << find_string(haystack, needle2) << '\n';
	std::cout << "uppercase: " << find_string(haystack, needle3) << '\n';
	std::cout << "identical: " << find_string(haystack, needle4) << '\n';
	std::cout << "lowecase: " << find_string(haystack, needle5) << '\n';
	std::cout << "uppercase: " << find_string(haystack, needle6) << '\n';
}