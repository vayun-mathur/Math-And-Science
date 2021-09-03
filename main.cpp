#include <iostream>
#include <io.h>
#include <fcntl.h>

#include "vector.h"
#include "equation.h"

int main() {
	/*
	matrix<3, 4> m = {
		{1, 2, 1, 1},
		{1, 2, 2, -1},
		{2, 4, 0, 6},
	};
	vector<3> v = {
		7, 12, 4
	};
	augmented_matrix<3, 4> a(m, v);
	print(a);

	a.combine_rows(1, 0, -1.0);
	a.combine_rows(2, 0, -2.0);
	a.combine_rows(2, 1, 2.0);
	a.combine_rows(0, 1, -1.0);

	print(a);

	augmented_matrix<3, 4> b(m, v);
	b.rref();
	print(b);
	*/
	init();
	equation e = equation("PCl_5 + H_2O --> H_3PO_4 + HCl");
	e.balance();
	_setmode(_fileno(stdout), _O_U16TEXT);
	std::wcout << equation_string(e) << std::endl;
}