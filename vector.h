#pragma once
#include <memory>

template<int n>
struct vector {
	float components[n];

	vector(std::initializer_list<float> components) {
		auto cell = components.begin();
		for (int j = 0; j < n; j++, ++cell) {
			this->components[j] = *cell;
		}
	}

	vector() {
		memset(components, 0, sizeof(float) * n);
	}

	vector(const vector<n>& other) {
		memcpy(components, other.components, sizeof(float) * n);
	}

	vector<n> operator*(float factor) {
		vector<n> v;
		for (int i = 0; i < n; i++) {
			v.components[i] = components[i] * factor;
		}
		return v;
	}
};

//m rows, n columns
template<int m, int n>
struct matrix {
	float components[m][n];

	matrix(std::initializer_list<std::initializer_list<float>> components) {
		auto row = components.begin();
		for (int i = 0; i < m; i++, ++row) {
			auto cell = row->begin();
			for (int j = 0; j < n; j++, ++cell) {
				this->components[i][j] = *cell;
			}
		}
	}

	matrix() {
		memset(components, 0, sizeof(float) * m * n);
	}

	matrix(const matrix<m, n>& other) {
		memcpy(components, other.components, sizeof(float) * m * n);
	}

	vector<m> mul(vector<n> x) {
		vector<m> v;
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				v.components[i] += x.components[j] * components[i][j];
			}
		}
		return v;
	}

	template<int p>
	matrix<m, p> mul(matrix<n, p> x) {
		matrix<m, p> v;
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < p; j++) {
				for (int k = 0; k < n; k++) {
					v.components[i][j] += x.components[k][j] * components[i][k];
				}
			}
		}
		return v;
	}

	vector<m> operator()(vector<n> x) {
		return this->mul(x);
	}

	template<int p>
	matrix<m, p> operator()(matrix<n, p> x) {
		return this->mul(x);
	}
};

//m rows, n columns
template<int m, int n>
struct augmented_matrix {
	//solve ax = b for x given a and b
	matrix<m, n> a;
	vector<m> b;

	augmented_matrix(matrix<m, n> a, vector<m> b) :
		a(a), b(b) {

	}

	void swap_rows(int x, int y) {
		for (int i = 0; i < n; i++) {
			std::swap(a.components[x][i], a.components[y][i]);
		}
		std::swap(b.components[x], b.components[y]);
	}

	void divide_row(int row, float factor) {
		for (int i = 0; i < n; i++) {
			a.components[row][i] /= factor;
		}
		b.components[row] *= factor;
	}

	void combine_rows(int dest, int src, double factor) {
		for (int i = 0; i < n; i++) {
			a.components[dest][i] += a.components[src][i] * factor;
			if (abs(a.components[dest][i]) < 1e-6) {
				a.components[dest][i] = 0;
			}
		}
		b.components[dest] += b.components[src] * factor;
		if (abs(b.components[dest]) < 1e-6) {
			b.components[dest] = 0;
		}
	}

	void move_empty_rows_bottom() {
		int bottom = m-1;
		for (int row = 0; row < bottom; row++) {
			bool row_empty = true;
			for (int c = 0; c < n; c++) {
				if (a.components[row][c] != 0) {
					row_empty = false;
					break;
				}
			}
			if (row_empty) {
				swap_rows(row, bottom);
				row--;
				bottom--;
			}
		}
	}

	void rref()
	{
		move_empty_rows_bottom();
		int lead = 0;

		for (int row = 0; row < m; ++row)
		{
			if (lead >= n)
				return;
			int i = row;
			while (a.components[i][lead] == 0)
			{
				++i;
				if (i >= m)
				{
					i = row;
					++lead;
					if (lead >= n)
						return;
				}
			}
			swap_rows(i, row);
			divide_row(row, a.components[row][lead]);
			for (i = 0; i < m; ++i)
			{
				if (i != row) {
					combine_rows(i, row, -a.components[i][lead]);
				}
			}
		}
	}
};

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
	for (int j = 0; j < 6; j++) {
		for (int i = 0; i < n; i++) {
			std::cout << v.a.components[j][i] << "\t";
		}
		std::cout << "|\t" << v.b.components[j] << std::endl;
	}
	std::cout << std::endl;
}