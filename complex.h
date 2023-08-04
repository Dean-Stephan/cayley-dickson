/*
Copyright (c) 2022, Dean Stephan

All rights reserved.

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of {{ project }} nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// Provide templated class definition for complex numbers
// See https://en.wikipedia.org/wiki/Complex_number for mathematical explanation
#pragma once

#include <iostream>
#include <type_traits>

namespace Stephan {

template <typename T>
class complex {
private:
	T	real_part;
	T	imaginary_part;
	bool use_electric_syntax; // Output will use j instead of i

public:
	complex(T _real_part = 0, T _imaginary_part = 0, bool _use_electric_syntax = false)
		: real_part(_real_part)
		, imaginary_part(_imaginary_part)
		, use_electric_syntax(_use_electric_syntax)
	{}
	complex()
		: complex(0,0)
	{}

	// Provide real-part and imaginary-part routines
	T Re() { return real_part; }
	T Im() { return imaginary_part; }

	// Provide a way to set the output format to use j instead of i
	void UseJInOutput(bool flag = true) { this->use_electric_syntax = flag; }

	// Boolean relationships
	// Note that only equality is defined. There is no ordering, so the concept
	// of greater than and less than has no meaning.
	bool operator==(const complex<T>& rhs) {
		return (this->real_part == rhs.real_part) && (this->imaginary_part == rhs.imaginary_part);
	}

	// Conjugate operation.
	// For a complex number a+bi, the conjugate is a-bi
	complex<T> conjugate() { return complex(this->real_part, -(this->imaginary_part)); }

	// Addition + Subtraction
	complex<T> operator+(const complex<T>& rhs) {
		return complex(this->real_part + rhs.real_part, this->imaginary_part + rhs.imaginary_part);
	}
	complex<T> operator+(const T& value) {
		return complex(this->real_part + value, this->imaginary_part);
	}
	complex<T> operator-(const complex<T>& rhs) {
		return complex(this->real_part - rhs.real_part, this->imaginary_part - rhs.imaginary_part);
	}
	complex<T> operator-(const T& value) {
		return complex(this->real_part - value, this->imaginary_part);
	}

	// Multiplication
	complex<T> operator*(const complex<T>& rhs) {
		return complex((this->real_part * rhs.real_part) - (this->imaginary_part * rhs.imaginary_part),
			(this->real_part * rhs.imaginary_part) + (this->imaginary_part * rhs.real_part));
	}
	complex<T> operator*(const T& value) {
		return complex(this->real_part * value, this->imaginary_part * value);
	}

	// Division
	complex<T> operator/(const complex<T>& rhs) {
		static_assert(is_floating_point<T>::value);
		T divisor = (rhs.real_part * rhs.real_part) + (rhs.imaginary_part * rhs.imaginary_part);
		T real_numerator = (this->real_part * rhs.real_part) + (this->imaginary_part * rhs.imaginary_part);
		T imaginary_numerator = (this->imaginary_part * rhs.real_part) - (this->real_part * rhs.imaginary_part);
		return complex(T(real_numerator/divisor), T(imaginary_numerator/divisor));
	}
	complex<T> operator/(const T& value) {
		return complex(this->real_part / value, this->imaginary_part / value);
	}

	// This returns the principal square root. 
	complex<T> sqrt() {
		T common = std::sqrt(this->real_part*this->real_part + this->imaginary_part*this->imaginary_part);
		T gamma = std::sqrt((this->real_part + common) / 2);
		int sign_bit = std::signbit(this->imaginary_part) ? -1 : 1;
		T delta = sign_bit * std::sqrt((-this->real_part + common) / 2);
		return complex(T(gamma), T(delta));
	}
	T norm() {
		return std::sqrt((this->real_part * this->real_part) + (this->imaginary_part * this->imaginary_part));
	}

	friend std::ostream& operator<<(std::ostream& out, const complex<T> &rhs);
	friend complex<T> T::operator+(const complex<T>& value);
	friend complex<T> T::operator-(const complex<T>& value);
	friend complex<T> T::operator*(const complex<T>& value);
	friend complex<T> T::operator/(const complex<T>& value);
};

std::ostream& operator<<(std::ostream& out, const complex<T> &rhs) {
	const char complex_tag = (rhs.use_electric_syntax) ? 'j' : 'i';
	return out << rhs.Re() << (rhs->Im()>=0)?"+":"" << rhs->Im() << complex_tag;
}

template <typename T>
complex<T> T::operator+(const complex<T>& value) {
	return complex(value.Re() + *this, value.Im());
}
template <typename T>
complex<T> T::operator-(const complex<T>& value) {
	return complex(*this - value.Re(), -value.Im());
}
template <typename T>
complex<T> T::operator*(const complex<T>& value) {
	return complex(*this * value.Re(), *this * value.Im());
}
template <typename T>
complex<T> T::operator/(const complex<T>& value) {
	static_assert(is_floating_point<T>::value);
	return complex<T>(*this, 0)/value;
}

}