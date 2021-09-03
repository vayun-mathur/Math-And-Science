#include <iostream>
#include <io.h>
#include <fcntl.h>

#include "vector.h"
#include "equation.h"

template<int n>
void print(vector<n>& v) {
	for (int i = 0; i < n; i++) {
		std::cout << v.components[i] << std::endl;
	}
	std::cout << std::endl;
}
template<int n, int m>
void print(matrix<n, m>& v) {
	for (int j = 0; j < m; j++) {
		for (int i = 0; i < n; i++) {
			std::cout << v.components[j][i] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}
template<int m, int n>
void print(augmented_matrix<m, n>& v) {
	for (int j = 0; j < m; j++) {
		for (int i = 0; i < n; i++) {
			std::cout << v.a.components[j][i] << "\t";
		}
		std::cout << "|\t" << v.b.components[j] << std::endl;
	}
	std::cout << std::endl;
}

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
	equation e = equation("C_6H_12O_6 + O_2 --> CO_2 + H_2O");
	e.balance();
	_setmode(_fileno(stdout), _O_U16TEXT);
	std::wcout << equation_string(e) << std::endl;
}