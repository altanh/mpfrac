#include "complex.h"

Complex::Complex() {

}

Complex::Complex(Real r, Real i) : re(r), im(i) {
}

Complex::Complex(const Complex &other) : re(other.re), im(other.im) {
    
}

Complex &Complex::operator=(const Complex &other) {
    re = other.re;
    im = other.im;

    return *this;
}

Complex Complex::add(const Complex &rhs) const {
    return Complex(this->re + rhs.re, this->im + rhs.im);
}

Complex Complex::operator+(const Complex &rhs) const {
    return this->add(rhs);
}

Complex Complex::sub(const Complex &rhs) const {
    return Complex(this->re - rhs.re, this->im - rhs.im);
}

Complex Complex::operator-(const Complex &rhs) const {
    return this->sub(rhs);
}

Complex Complex::mul(const Complex &rhs) const {
    return Complex((this->re * rhs.re) - (rhs.im * this->im),
            (this->im * rhs.re) + (this->re * rhs.im));
}

Complex Complex::mul(const Real &rhs) const {
    return Complex(this->re * rhs, this->im * rhs);
}

Complex Complex::operator*(const Complex &rhs) const {
    return this->mul(rhs);
}

Complex Complex::operator*(const Real &rhs) const {
    return this->mul(rhs);
}

Complex Complex::div(const Complex &rhs) const {
    Real magSquared = rhs.re.pow(2) + rhs.im.pow(2);

    return Complex(((this->re * rhs.re) + (this->im * rhs.im)) / magSquared,
            ((this->im * rhs.re) - (this->re * rhs.im)) / magSquared);
}

Complex Complex::div(const Real &rhs) const {
    return Complex(this->re / rhs, this->im / rhs);
}

Complex Complex::operator/(const Complex &rhs) const {
    return this->div(rhs);
}

Complex Complex::operator/(const Real &rhs) const {
    return this->div(rhs);
}

Complex Complex::conjugate() const {
    return Complex(this->re, this->im.neg());
}

Complex Complex::reciprocal() const {
    return this->conjugate() / (this->re.pow(2) + this->im.pow(2));
}

Complex Complex::pow(int p) const {
    if(p == 0)
        return Complex(Real("1"), Real("0"));
    else if(p < 0) {
        Complex result = this->reciprocal();

        for(int i = 1; i < -p; ++i)
            result = result / *this;

        return result;
    } else {
        Complex result = *this;

        for(int i = 1; i < p; ++i) {
            result = result * *this;
        }

        return result;
    }
}

Real Complex::mag() const {
    return re.pow(2).add(im.pow(2)).sqrt();
}
