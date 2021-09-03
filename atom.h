#pragma once
#include "dimensional.h"
#include <string>

inline units grams_per_mole = units{ 0.001, dimensions{0, 0, -1, 1 } };

struct atom_data
{
	std::string name;
	std::string symbol;
	int atomic_number;
	d_number molar_mass;

	atom_data(std::string name, std::string symbol, int atomic_number, d_number molar_mass)
		: name(name), symbol(symbol), atomic_number(atomic_number), molar_mass(molar_mass) {

	}

};

struct atom {
	atom_data type;
	int _charge;

	atom(const atom_data& type, int _charge);

	int charge();

	d_number molar_mass();

	std::wstring to_string();
};

atom_data get_atom_by_symbol(std::string symbol);