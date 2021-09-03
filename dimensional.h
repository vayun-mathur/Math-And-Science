#pragma once
#include <exception>

struct dimensions {
	int meters, seconds, moles, kilograms;

	dimensions operator*(const dimensions& other) const {
		return { meters + other.meters, seconds + other.seconds, moles + other.moles, kilograms + other.kilograms };
	}

	dimensions operator/(const dimensions& other) const {
		return { meters - other.meters, seconds - other.seconds, moles - other.moles, kilograms - other.kilograms };
	}

	bool operator!=(const dimensions& other) const {
		return meters != other.meters ||
			seconds != other.seconds ||
			moles != other.moles ||
			kilograms != other.kilograms;
	}
};

struct d_number
{
	double val;
	dimensions dim;

	d_number operator+(const d_number& other) const {
		if (dim != other.dim) throw std::exception("dimensions do not match for addition");
		return { val + other.val, dim };
	}

	d_number operator-(const d_number& other) const {
		if (dim != other.dim) throw std::exception("dimensions do not match for subtraction");
		return { val - other.val, dim };
	}

	d_number operator*(double other) const {
		return { val * other, dim };
	}

	friend d_number operator*(double other, const d_number& t) {
		return { t.val * other, t.dim };
	}

	d_number operator*(const d_number& other) const {
		return { val * other.val, dim * other.dim };
	}

	d_number operator/(const d_number& other) const {
		return { val / other.val, dim / other.dim };
	}
};

typedef d_number units;