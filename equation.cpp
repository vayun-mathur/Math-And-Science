#include "equation.h"
#include <iostream>
#include <regex>

std::vector<std::string> split(std::string s, std::string del = " ")
{
	std::vector<std::string> vec;
	std::regex rgx(del);
	std::sregex_token_iterator iter(s.begin(),
		s.end(),
		rgx,
		-1);
	std::sregex_token_iterator end;
	for (; iter != end; ++iter)
		vec.push_back(*iter);
	return vec;
}

void trim(std::string& str) {
	int start = str.find_first_not_of(' ');
	int end = str.find_last_not_of(' ');
	str = str.substr(start, end - start + 1);
}

equation::equation(std::string formula)
{
	auto idx = formula.find("-->");
	std::string left = formula.substr(0, idx);
	std::string right = formula.substr(idx + 3);
	for (std::string s : split(left, R"(([^\^]\+))")) {
		trim(s);
		int idx = 0;
		while (s[idx] >= '0' && s[idx] <= '9') idx++;
		this->left.push_back({ molecule(s.substr(idx)), idx == 0 ? 1 : stoi(s.substr(0, idx)) });
	}
	for (std::string s : split(right, R"(([^\^]\+))")) {
		trim(s);
		int idx = 0;
		while (s[idx] >= '0' && s[idx] <= '9') idx++;
		this->right.push_back({ molecule(s.substr(idx)), idx == 0 ? 1 : stoi(s.substr(0, idx)) });
	}
}

std::wstring equation_string(equation& m)
{
	std::wstring s;
	for (int i = 0; i < m.left.size(); i++) {
		if (i != 0) {
			s += L" + ";
		}
		auto& term = m.left[i];
		if (term.count != 1)
			s += std::to_wstring(term.count);
		s += term.molecule.to_string();
	}

	s += L" → ";

	for (int i = 0; i < m.right.size(); i++) {
		if (i != 0) {
			s += L" + ";
		}
		auto& term = m.right[i];
		if (term.count != 1)
			s += std::to_wstring(term.count);
		s += term.molecule.to_string();
	}
	return s;
}
