#ifndef COMPLEX_H
#define COMPLEX_H

#include "real.h"

struct Complex {
    Real re;
    Real im;

    Complex();
    Complex(Real re, Real im);
    Complex(const Complex &other);
    Complex &operator=(const Complex &other);

    Complex add(const Complex &rhs) const;
    Complex operator+(const Complex &rhs) const;

    Complex sub(const Complex &rhs) const;
    Complex operator-(const Complex &rhs) const;

    Complex mul(const Complex &rhs) const;
    Complex mul(const Real &rhs) const;
    Complex operator*(const Complex &rhs) const;
    Complex operator*(const Real &rhs) const;

    Complex div(const Complex &rhs) const;
    Complex div(const Real &rhs) const;
    Complex operator/(const Complex &rhs) const;
    Complex operator/(const Real &rhs) const;

    Complex conjugate() const;

    Complex reciprocal() const;

    Complex pow(int p) const;

    Real mag() const;
};

#endif