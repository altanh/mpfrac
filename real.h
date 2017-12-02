#ifndef REAL_H
#define REAL_H

#include <iostream>
#include <string>
#include <mpfr.h>

class Real {
    mpfr_t _val;

public:
    static const Real ZERO;
    static const Real ONE;
    static const Real TWO;

    Real();
    Real(long double d);
    Real(unsigned int ui);
    Real(std::string str, int base = 10);
    Real(const Real& other);

    Real &operator=(const Real &other);

    ~Real();

    mpfr_prec_t getPrecision() const;
    void setPrecision(mpfr_prec_t prec);

    Real add(const Real &other) const;
    Real operator+(const Real &other) const;

    Real sub(const Real &other) const;
    Real operator-(const Real &other) const;

    Real mul(const Real &other) const;
    Real operator*(const Real &other) const;

    Real div(const Real &other) const;
    Real operator/(const Real &other) const;

    Real neg() const;

    Real sqrt() const;
    Real root(unsigned long int k) const;

    Real pow(const Real &other) const;
    Real pow(long int n) const;

    bool operator<(const Real &rhs) const;
    bool operator>(const Real &rhs) const;
    bool operator<=(const Real &rhs) const;
    bool operator>=(const Real &rhs) const;

    bool operator==(const Real &rhs) const;
    bool operator!=(const Real &rhs) const;

    double toDouble() const;

    void print(mpfr_prec_t prec) const;
};

#endif