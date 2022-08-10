#pragma once

#include <vector>
#include <string>
#include <iostream>

std::vector<std::string> split(std::string const &str, char del = ' ');

std::string &strip(std::string &str);

std::string &to_lower(std::string &str);

bool ends_with(std::string const &str, std::string ending);

size_t find_string(std::string const& haystack, std::string needle);

void removeExtraSlashes(std::string &str, size_t index = 0, size_t shift = 0);

std::string getFileType(std::string path);
