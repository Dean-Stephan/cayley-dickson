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

// Provide templated class definition for octonion numbers
// See https://en.wikipedia.org/wiki/Octonion for mathematical explanation
#pragma once

namespace Stephan {

template <typename T>
class octonion {
private:
	T	real_part;
	T	imaginary_part;

public:
	octonion(T _real_part = 0, T _imaginary_part = 0, bool _use_electric_syntax = false)
		: real_part(_real_part)
		, imaginary_part(_imaginary_part)
		, use_electric_syntax(_use_electric_syntax)
	{}
	octonion()
		: octonion(0,0)
	{}

	// Provide real-part and imaginary-part routines
	T Re() { return real_part; }
	T Im() { return imaginary_part; }

	// Boolean relationships
	// Note that only equality is defined. There is no ordering, so the concept
	// of greater than and less than has no meaning.
	bool operator==(const octonion<T>& rhs) {
		return (this->real_part == rhs.real_part) && (this->imaginary_part == rhs.imaginary_part);
	}

	// Conjugate operation.
	// For a octonion number a+bi, the conjugate is a-bi
	octonion<T> conjugate() { return octonion(this->real_part, -(this->imaginary_part)); }

	// Addition + Subtraction
	octonion<T> operator+(const octonion<T>& rhs) {
		return octonion(this->real_part + rhs.real_part, this->imaginary_part + rhs.imaginary_part);
	}
	octonion<T> operator+(const T& value) {
		return octonion(this->real_part + value, this->imaginary_part);
	}
	octonion<T> operator-(const octonion<T>& rhs) {
		return octonion(this->real_part - rhs.real_part, this->imaginary_part - rhs.imaginary_part);
	}
	octonion<T> operator-(const T& value) {
		return octonion(this->real_part - value, this->imaginary_part);
	}

	// Multiplication
	octonion<T> operator*(const octonion<T>& rhs) {
		return octonion(this->real_part * rhs.real_part) - (this->imaginary_part * rhs.imaginary_part), (this->real_part * rhs.imaginary_part) + (this->imaginary_part * rhs.real_part));
	}
	octonion<T> operator*(const T& value) {
		return octonion(this->real_part * value, this->imaginary_part * value);
	}

	// Division
	octonion<T> operator/(const octonion<T>& rhs) {
		T divisor = (this->real_part * this->real_part) + (this->imaginary_part * this->imaginary_part);
		T real_numerator = (this->real_part * rhs.real_part) + (this->imaginary_part * rhs.imaginary_part);
		T imaginary_numerator = (rhs.imaginary_part * this->real_part) - (rhs.real_part * this->imaginary_part);
		return octonion(T(real_numerator/divisor), T(imaginary_numerator/divisor))
	}
	octonion<T> operator/(const T& value) {
		return octonion(this->real_part / value, this->imaginary_part / value);
	}

#ifdef 0
	// Because octonion multiplication involves combining both real and imaginary
	// parts, it appears that a remainder function will require performing division
	// and then multiplication followed by subtraction from the lhs.
	octonion<T> operator%(const octonion<T>& rhs) {
		T divisor = (this->real_part * this->real_part) + (this->imaginary_part * this->imaginary_part);
		T real_numerator = (this->real_part * rhs.real_part) + (this->imaginary_part * rhs.imaginary_part);
		T imaginary_numerator = (rhs.imaginary_part * this->real_part) - (rhs.real_part * this->imaginary_part);
		return octonion(T(real_numerator%divisor), T(imaginary_numerator%divisor))
	}
	octonion<T> operator%(const T& value) {
		return octonion(this->real_part % value, this->imaginary_part % value);
	}
#endif

	// This returns the principal square root. 
	octonion<T> sqrt() {
		T common = std::sqrt(this->real_part*this->real_part + this->imaginary_part*this->imaginary_part);
		T gamma = std::sqrt((this->real_part + common) / 2);
		int sign_bit = std::signbit(this->imaginary_part) ? -1 : 1;
		T delta = sign_bit * std::sqrt((-this->real_part + common) / 2);
		return octonion(T(gamma), T(delta));
}
};

}