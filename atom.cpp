#include "atom.h"
#include "examples.h"
#include <map>

atom_data atoms[56] = {
	hydrogen,
	helium,
	lithium,
	beryllium,
	boron,
	carbon,
	nitrogen,
	oxygen,
	fluorine,
	neon,
	sodium,
	magnesium,
	aluminium,
	silicon,
	phosphorus,
	sulfur,
	chlorine,
	argon,
	potassium,
	calcium,
	scandium,
	titanium,
	vanadium,
	chromium,
	manganese,
	iron,
	cobalt,
	nickel,
	copper,
	zinc,
	gallium,
	germanium,
	arsenic,
	selenium,
	bromine,
	krypton,
	rubidium,
	strontium,
	yttrium,
	zirconium,
	niobium,
	molybdenum,
	technetium,
	ruthenium,
	rhodium,
	palladium,
	silver,
	cadmium,
	indium,
	tin,
	antimony,
	tellurium,
	iodine,
	xenon,
	caesium,
	barium
};

std::map < std::string, atom_data> atoms_by_symbol;

void init() {
	for (int i = 0; i < sizeof(atoms) / sizeof(atom_data); i++) {
		atoms_by_symbol.emplace(atoms[i].symbol, atoms[i]);
	}
}

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
