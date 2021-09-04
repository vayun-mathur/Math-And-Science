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
		const int max_unique_elements = 118;
		const int max_unique_molecules = 10;
		const int max_coefficient = 100;
		vector<max_unique_elements> v2;
		matrix<max_unique_elements, max_unique_molecules> m;
		for (int i = 0; i < left.size(); i++) {
			m.components[0][i] = -left[i].molecule.charge;
			for (auto&& [atom_number, count] : left[i].molecule.get_atom_count()) {
				m.components[atom_number][i] = count;
			}
		}
		for (int i = 0; i < right.size(); i++) {
			m.components[0][i + left.size()] = right[i].molecule.charge;
			for (auto&& [atom_number, count] : right[i].molecule.get_atom_count()) {
				m.components[atom_number][i + left.size()] = -count;
			}
		}
		augmented_matrix<max_unique_elements, max_unique_molecules> a(m, vector<max_unique_elements>());
		a.rref();
		for (int i = 0; i < max_unique_elements; i++) {
			v2.components[i] = -a.a.components[i][left.size() + right.size() - 1];
		}

		for (int factor = 1; factor <= max_coefficient; factor++) {
			vector<max_unique_elements> v3 = v2 * factor;
			bool correct = true;
			for (int i = 0; i < max_unique_elements; i++) {
				if (abs(v3.components[i] - round(v3.components[i])) > 0.02) {
					correct = false;
				}
			}
			if (correct) {
				for (int i = 0; i < left.size(); i++) {
					left[i].count = round(v3.components[i]);
				}
				for (int i = 0; i < right.size() - 1; i++) {
					right[i].count = round(v3.components[left.size() + i]);
				}
				right[right.size() - 1].count = factor;
				break;
			}
		}
	}
};

std::wstring equation_string(equation& m);