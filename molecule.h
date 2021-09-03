#pragma once
#include "atom.h"
#include <vector>
#include <map>

struct atom_term { atom atom; int count; };

struct molecule {

	std::vector<atom_term> atoms;

	molecule(std::string formula);

	d_number molar_mass();

	std::wstring to_string();

	std::map<int, int> get_atom_count();
};