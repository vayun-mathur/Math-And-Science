#include "atom.h"
#include "examples.h"
#include <map>

std::map < std::string, atom_data> atoms_by_symbol = {
	{"H", hydrogen},
	{"Li", lithium},
	{"C", carbon},
	{"N", nitrogen},
	{"O", oxygen},
	{"Na", sodium},
	{"Al", aluminium},
	{"Si", silicon},
	{"S", sulfur},
	{"Cl", chlorine},
	{"K", potassium},
	{"Ti", titanium},
	{"Fe", iron},
	{"Cu", copper},
	{"Br", bromine},
	{"Ag", siver},
	{"Ba", barium},
};

atom_data get_atom_by_symbol(std::string symbol)
{
	return atoms_by_symbol.find(symbol)->second;
}

atom::atom(const atom_data& type, int _charge)
	: type(type), _charge(_charge)
{
}

int atom::charge()
{
	return _charge;
}

d_number atom::molar_mass()
{
	return type.molar_mass;
}

std::wstring atom::to_string()
{
	return std::wstring(type.symbol.begin(), type.symbol.end());
}
