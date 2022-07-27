#include <iostream>
#include <cstdlib>

enum couple{
	// negative = -1,
	first,
	second,
	third = 3000000000
};

int main()
{
	couple c = third;
	std::cout << "third size " << sizeof(couple) << " val " << c << '\n';
	// std::cout << "n " << negative << '\n';
	std::cout << "f " << first << '\n';
	std::cout << "s " << second << '\n';
}

	// std::cout << "couple size: " << sizeof(couple) << '\n';

	// f(0);
	// f(1);
	// f(15);
	// f(1000000000);


// void f(int val)
// {
// 	couple data = couple(val);
// 	if(data == first || data == second)
// 		std::cout << "got couple " << data << '\n';
// 	else
// 		std::cout << "not a couple, val " << data << '\n';
// }