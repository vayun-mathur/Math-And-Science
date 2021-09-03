#include "molecule.h"
#include <regex>
#include <iostream>
#include <Windows.h>
#include <numeric>
#include <algorithm>

std::wstring subscript(int n) {
	if (n == 1) return std::wstring();
	std::wstring s = std::to_wstring(n);
	for (int i = 0; i < s.size(); i++) {
		s[i] = s[i] - L'0' + L'₀';
	}
	return s;
}

std::wstring superscript(int n) {
	static const wchar_t* super = L"⁰¹²³⁴⁵⁶⁷⁸⁹";
	if (n == 1) return std::wstring();
	std::wstring s = std::to_wstring(n);
	for (int i = 0; i < s.size(); i++) {
		s[i] = super[s[i] - L'0'];
	}
	return s;
}

molecule::molecule(std::string formula)
{
	int atoms_cnt = 0;
	for (char c : formula) {
		if (c >= 'A' && c <= 'Z') atoms_cnt++;
	}
	int idx = 0;
	for (int i = 0; i < atoms_cnt; i++) {
		int nextIdx = idx + 1;
		while (nextIdx < formula.size() && !(formula[nextIdx] >= 'A' && formula[nextIdx] <= 'Z')) nextIdx++;
		std::string atom_string = formula.substr(idx, nextIdx - idx);
		idx = nextIdx;
		auto index = atom_string.find_first_of('_');
		if (index == std::string::npos) {
			atoms.push_back(atom_term{ atom(get_atom_by_symbol(atom_string), 0), 1 });
		}
		else {
			atoms.push_back(atom_term{ atom(get_atom_by_symbol(atom_string.substr(0, index)), 0),
				std::stoi(atom_string.substr(index + 1)) });
		}
	}
}

d_number molecule::molar_mass()
{
	d_number mm = 0 * grams_per_mole;
	for (atom_term term : atoms) {
		mm = mm + term.atom.molar_mass() * term.count;
	}
	return mm;
}

std::wstring molecule::to_string()
{
	std::wstring s;
	for (atom_term term : atoms) {
		s += term.atom.to_string();
		s += subscript(term.count);
	}
	return s;
}

std::map<int, int> molecule::get_atom_count()
{
	std::map<int, int> map;
	for (atom_term term : atoms) {
		map[term.atom.type.atomic_number] += term.count;
	}
	return map;
}
