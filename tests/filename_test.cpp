#include "../file_utils.hpp"
#include <iostream>
#include <unistd.h>

int main()
{
	char *cwd = getcwd((char *)0, 0);

	std::string fname = "../config/test_dir/foobar.html";

	std::cout << "trying file " << fname
	<< " from " << cwd << '\n';

	std::cout << std::boolalpha << is_filename(fname) << '\n';

	free(cwd);
}