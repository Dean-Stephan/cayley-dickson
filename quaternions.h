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

// Provide templated class definition for quaternion numbers
// See https://en.wikipedia.org/wiki/Quaternion for mathematical explanation.
// This implementation works off the algebraic equations that are based on
// the original description by William Rowan Hamilton which represents a
// quaternion via the equation
//      a + bi + cj + dk
// where a, b, c, and d are real numbers and i, j, and k have
// the following relationship:
//      i*i = j*j = k*k = i*j*k = -1
#pragma once

#include "complex.h"

namespace Stephan {

template <typename T>
class quaternion {
private:
	T	real_part;
	T	i_part;
    T   j_part;
    T   k_part;

public:
	quaternion(T _real_part = 0, T i = 0, T j = 0, T k = 0)
		: real_part(_real_part)
		, i_part(i)
        , j_part(j)
        , k_part(k)
	{}
	quaternion()
		: quaternion(0,0,0,0)
	{}

	// Provide real-part and imaginary-part routines
	T Re() { return real_part; }
	T Im1() { return i_part; }
	T Im2() { return j_part; }
	T Im3() { return k_part; }

	// Boolean relationships
	// Note that only equality is defined. There is no ordering, so the concept
	// of greater than and less than has no meaning.
	bool operator==(const quaternion<T>& rhs) {
		return (this->real_part == rhs.real_part) && (this->i == rhs.i_part)
            && (this->j_part == rhs.j_part) && (this->k_part == rhs.k_part);
	}

	// Conjugate operation.
	// For a quaternion number a+bi, the conjugate is a-bi
	quaternion<T> conjugate() { return quaternion(this->real_part, -(this->i_part), -(this->j_part), -(this->k_part)); }

	// Addition + Subtraction
	quaternion<T> operator+(const quaternion<T>& rhs) {
		return quaternion(this->real_part + rhs.real_part, this->i_part + rhs.i_part,
            this->j_part + rhs.j_part, this->k_part + rhs.j_part);
	}
	quaternion<T> operator+(const T& value) {
		return quaternion(this->real_part + value, this->i_part, this->j_part, this->k_part);
	}
	quaternion<T> operator-(const quaternion<T>& rhs) {
		return quaternion(this->real_part - rhs.real_part, this->i_part - rhs.i_part,
            this->j_part - rhs.j_part, this->k_part - rhs.k_part);
	}
	quaternion<T> operator-(const T& value) {
		return quaternion(this->real_part - value, this->i_part, this->j_part, this->k_part);
	}

	// Multiplication
	quaternion<T> operator*(const quaternion<T>& rhs) {
		return quaternion(
            (this->real_part * rhs.real_part) - (this->i_part * rhs.i_part) - (this->j_part * rhs.j_part) - (this->k_part * rhs.k_part),
            (this->real_part * rhs.i_part) + (this->i_part * rhs.real_part) + (this->j_part * rhs.k_part) - (this->k_part * rhs.j_part),
            (this->real_part * rhs.j_part) + (this->j_part * rhs.real_part) + (this->k_part * rhs.i_part) - (this->i_part * rhs.k_part),
            (this->real_part * rhs.k_part) + (this->k_part * rhs.real_part) + (this->i_part * rhs.j_part) - (this->j_part * rhs.i_part));
	}
	quaternion<T> operator*(const T& value) {
		return quaternion(this->real_part * value, this->i_part * value,
            this->j_part * value, this->k_part * value);
	}

	// Division
    // NOTE: Given two quaternions p and q. The result of the division can lead to two possible
    // solutions (q^-1) * p or p * (q^-1).
    T norm() {
        return std::sqrt((this->real_part*this->real_part) + (this->i_part*this->i_part) + (this->j_part*this->j_part) + (this->k_part*this->k_part));
    }
    quaternion<T> reciprocal() {
        T base = this->norm();
        base *= base;
        return (*this) / base;
    }
	quaternion<T> operator/(const quaternion<T>& rhs) {
		return (*this) * rhs->reciprocal();
	}
	quaternion<T> operator/(const T& value) {
		return quaternion(this->real_part / value, this->i_part / value,
            this->j_part / value, this->k_part / value);
	}
};

}