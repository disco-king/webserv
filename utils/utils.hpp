#pragma once

#include "string_utils.hpp"
#include "file_utils.hpp"

# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define RESET "\033[0m"

struct t_listen
{
	unsigned int host;
	short port;
};
