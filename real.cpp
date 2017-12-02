#include "real.h"

const Real Real::ZERO = Real("0");
const Real Real::ONE = Real("1");
const Real Real::TWO = Real("2");

Real::Real() {
    mpfr_init(_val);
}

Real::Real(long double d) {
    mpfr_init_set_d(_val, d, MPFR_RNDN);
}

Real::Real(unsigned int ui) {
    mpfr_init_set_ui(_val, ui, MPFR_RNDN);
}

Real::Real(std::string str, int base) {
    mpfr_init_set_str(_val, str.c_str(), base, MPFR_RNDN);
}

Real::Real(const Real& other) {
    mpfr_init_set(_val, other._val, MPFR_RNDN);
}

Real &Real::operator=(const Real& other) {
    mpfr_clear(_val);
    mpfr_init_set(_val, other._val, MPFR_RNDN);

    return *this;
}

Real::~Real() {
    mpfr_clear(_val);
}

mpfr_prec_t Real::getPrecision() const {
    return mpfr_get_prec(_val);
}

void Real::setPrecision(mpfr_prec_t prec) {
    mpfr_set_prec(_val, prec);
}

Real Real::add(const Real &other) const {
    Real result;

    result.setPrecision(this->getPrecision() > other.getPrecision() ?
            this->getPrecision() : other.getPrecision());

    mpfr_add(result._val, this->_val, other._val, MPFR_RNDN);

    return result;
}

Real Real::operator+(const Real &other) const {
    return this->add(other);
}

Real Real::sub(const Real &other) const {
    Real result;

    result.setPrecision(this->getPrecision() > other.getPrecision() ?
            this->getPrecision() : other.getPrecision());

    mpfr_sub(result._val, this->_val, other._val, MPFR_RNDN);

    return result;
}

Real Real::operator-(const Real &other) const {
    return this->sub(other);
}

Real Real::mul(const Real &other) const {
    Real result;

    result.setPrecision(this->getPrecision() > other.getPrecision() ?
            this->getPrecision() : other.getPrecision());

    mpfr_mul(result._val, this->_val, other._val, MPFR_RNDN);

    return result;
}

Real Real::operator*(const Real &other) const {
    return this->mul(other);
}

Real Real::div(const Real &other) const {
    Real result;

    result.setPrecision(this->getPrecision() > other.getPrecision() ?
            this->getPrecision() : other.getPrecision());

    mpfr_div(result._val, this->_val, other._val, MPFR_RNDN);

    return result;
}

Real Real::operator/(const Real &other) const {
    return this->div(other);
}

Real Real::neg() const {
    Real result;
    result.setPrecision(getPrecision());

    mpfr_neg(result._val, this->_val, MPFR_RNDN);

    return result;
}

Real Real::sqrt() const {
    Real result;

    result.setPrecision(getPrecision());

    mpfr_sqrt(result._val, this->_val, MPFR_RNDN);

    return result;
}

Real Real::root(unsigned long int k) const {
    Real result;

    result.setPrecision(getPrecision());

    mpfr_root(result._val, this->_val, k, MPFR_RNDN);

    return result;
}

Real Real::pow(const Real &other) const {
    Real result;

    result.setPrecision(this->getPrecision() > other.getPrecision() ?
            this->getPrecision() : other.getPrecision());

    mpfr_pow(result._val, this->_val, other._val, MPFR_RNDN);

    return result;
}

Real Real::pow(long int n) const {
    Real result;

    result.setPrecision(getPrecision());

    mpfr_pow_si(result._val, this->_val, n, MPFR_RNDN);

    return result;
}

bool Real::operator<(const Real &rhs) const {
    return mpfr_less_p(this->_val, rhs._val) != 0;
}

bool Real::operator>(const Real &rhs) const {
    return mpfr_greater_p(this->_val, rhs._val) != 0;
}

bool Real::operator<=(const Real &rhs) const {
    return mpfr_lessequal_p(this->_val, rhs._val) != 0;
}

bool Real::operator>=(const Real &rhs) const {
    return mpfr_greaterequal_p(this->_val, rhs._val) != 0;
}

bool Real::operator==(const Real &rhs) const {
    return mpfr_equal_p(this->_val, rhs._val) != 0;
}

bool Real::operator!=(const Real &rhs) const {
    return !(*this == rhs);
}

double Real::toDouble() const {
    return mpfr_get_d(_val, MPFR_RNDN);
}

void Real::print(mpfr_prec_t prec) const {
    mpfr_printf(std::string("%." + std::to_string(prec) + "Rf").c_str(), _val);
}
