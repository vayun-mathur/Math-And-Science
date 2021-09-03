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

	void scale_row(int row, double factor) {
		for (int i = 0; i < n; i++) {
			a.components[row][i] *= factor;
		}
		b.components[row] *= factor;
	}

	void combine_rows(int dest, int src, double factor) {
		for (int i = 0; i < n; i++) {
			a.components[dest][i] += a.components[src][i] * factor;
		}
		b.components[dest] += b.components[src] * factor;
	}

	void ref() {
		int column = 0;
		int row = 0;
		while (column < n && row < m) {
			if (a.components[row][column] == 0) {
				for (int r = row + 1; r < m; r++) {
					if (a.components[r][column] != 0) {
						swap_rows(row, r);
						break;
					}
				}
			}
			if (a.components[row][column] != 0) {
				scale_row(row, 1.0 / a.components[row][column]);
				for (int r = row + 1; r < m; r++) {
					combine_rows(r, row, -a.components[r][column]);
				}
				row++;
			}
			column++;
		}
	}

	void rref() {
		ref();
		for (int r = 1; r < m; r++) {
			int column = 0;
			while (a.components[r][column] == 0) column++;
			for (int r2 = 0; r2 < r; r2++) {
				combine_rows(r2, r, -a.components[r][column]);
			}
		}
	}
};