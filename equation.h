#pragma once
#include <vector>
#include <map>
#include <iostream>
#include "molecule.h"
#include "vector.h"

struct equation {
	struct term { molecule molecule; int count; };
	std::vector<term> left;
	std::vector<term> right;

	equation(std::string formula);

	void stoic(std::vector<d_number> left_amount, std::vector<d_number>& right_grams, std::vector<d_number>& right_moles, std::vector<d_number>& left_remaining_moles, std::vector<d_number>& left_remaining_grams, std::vector<d_number>& left_moles, std::vector<d_number>& left_grams, int& lr) {
		for (int i = 0; i < left.size(); i++) {
			if (left_amount[i].dim.kilograms == 1) {
				left_amount[i] = left_amount[i] / left[i].molecule.molar_mass();
			}
		}
		int x = stoic_limiting_reactant_moles(left_amount);
		lr = x;
		d_number m = left_amount[x] / d_number{ (double)left[x].count, {0, 0, 0, 0} };
		for (int i = 0; i < right.size(); i++) {
			right_moles[i] = (right[i].count * m);
			right_grams[i] = (right_moles[i] * right[i].molecule.molar_mass());
		}
		for (int i = 0; i < left.size(); i++) {
			left_remaining_moles[i] = (left_amount[i] - left[i].count * m);
			left_moles[i] = left[i].count * m;
		}
		for (int i = 0; i < left.size(); i++) {
			left_remaining_grams[i] = left_remaining_moles[i] * left[i].molecule.molar_mass();
			left_grams[i] = left_moles[i] * left[i].molecule.molar_mass();
		}
	}

	int stoic_limiting_reactant_moles(std::vector<d_number> left_moles) {
		double m = std::numeric_limits<double>::infinity();
		int x = 0;
		for (int i = 0; i < left.size(); i++) {
			if ((left_moles[i] / d_number{ (double)left[i].count, {0, 0, 0, 0} }).val < m) {
				m = (left_moles[i] / d_number{ (double)left[i].count, {0, 0, 0, 0} }).val;
				x = i;
			}
		}
		return x;
	}

	void balance() {
		//matrix m(20, left.size() + right.size());
		switch (left.size() + right.size()) {
		case 4:
			matrix<10, 4> m;
			for (int i = 0; i < left.size(); i++) {
				std::map<int, int> atom_count = left[i].molecule.get_atom_count();
				for (auto&& [atom_number, count] : atom_count) {
					m.components[atom_number][i] = count;
				}
			}
			for (int i = 0; i < right.size(); i++) {
				std::map<int, int> atom_count = right[i].molecule.get_atom_count();
				for (auto&& [atom_number, count] : atom_count) {
					m.components[atom_number][i + left.size()] = -count;
				}
			}
			vector<10> v;
			augmented_matrix<10, 4> a(m, v);
			a.rref();
			vector<10> v2;
			for (int i = 0; i < 10; i++) {
				v2.components[i] = a.a.components[i][left.size() + right.size() - 1] * -1;
			}
			for (int factor = 1; factor < 50; factor++) {
				vector<10> v3 = v2 * factor;
				bool correct = true;
				for (int i = 0; i < 10; i++) {
					if (abs(v3.components[i] - round(v3.components[i])) > 0.02) {
						correct = false;
					}
				}
				if (correct) {
					for (int i = 0; i < left.size(); i++) {
						left[i].count = round(v3.components[i]);
					}
					for (int i = 0; i < right.size()-1; i++) {
						right[i].count = round(v3.components[left.size() + i]);
					}
					right[right.size() - 1].count = factor;
					break;
				}
			}
		}

	}
};

std::wstring equation_string(equation& m);