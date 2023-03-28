#include <iostream>
#include <cmath>

#define ASSERT(X)                                                           \
  if(!(X)) {                                                                \
    std::cout << __FILE__ << ", " << __LINE__ << " Failed: " << #X << "\n"; \
    *(int*)0 = 1;                                                           \
  }                                                                         \
  else {                                                                    \
    std::cout << "Succeded: " << #X << "\n";                                \
  }                                                                         \

#define BUILD(FUNC) \
  std::cout << "--------------------------------\n";        \
  std::cout << "Operation: " << #FUNC << "\n";              \
  std::cout << "--------------------------------\n\n";      \
  std::cout << " - | -1 |  0 |  1\n";                       \
  std::cout << "-----------------\n";                       \
  for(int i = -1; i < 2; ++i) {                             \
    printf("%+1d | %+1d | %+1d | %+1d\n",                   \
           i,                                               \
           int(FUNC(i, -1)),                                \
           int(FUNC(i, 0)),                                 \
           int(FUNC(i, 1)));                                \
  }                                                         \


//------------------------------------------------------------------------------
// Task 1
//------------------------------------------------------------------------------

using Real = float;

class Complex {
public:

  Complex(Real realPart, Real imaginaryPart) {
    m_real = realPart;
    m_imag = imaginaryPart;
  }

  Real getReal() const {
    return m_real;
  }

  Real getImaginary() const {
    return m_imag;
  }

  Complex operator+(const Complex& rop) const {
    return Complex(m_real + rop.m_real,
                   m_imag + rop.m_imag);
  }

  Complex operator-() const {
    return Complex(-m_real, -m_imag);
  }

  Complex operator-(const Complex& rop) const {
    return *this + (-rop);
  }

  Complex operator*(const Complex& rop) const {
    return Complex(rop.m_real * m_real - rop.m_imag * m_imag,
                   m_real * rop.m_imag + m_imag * rop.m_real);
  }

  Complex conjugate() const {
    return Complex(m_real, -m_imag);
  }

  Real magnitude() const {
    return sqrt(m_real * m_real + m_imag * m_imag);
  }

private:

  Real m_real;
  Real m_imag;

};


//------------------------------------------------------------------------------
// Task 2
//------------------------------------------------------------------------------


// NOTE(nerance): Щоб досягнути достичь "справжньої" ФП, можна використовувати
// CRTP, замість віртуальних методів
class Logic {
public:

  virtual int8_t AND(int8_t lop, int8_t rop) const = 0;
  virtual int8_t OR(int8_t lop, int8_t rop) const = 0;
  virtual int8_t XOR(int8_t lop, int8_t rop) const = 0;
  virtual int8_t NOT(int8_t lop) const = 0;
  virtual int8_t IMPLY(int8_t lop, int8_t rop) const = 0;
  virtual int8_t IS(int8_t lop, int8_t rop) const = 0;

  virtual void buildTable() {}

};


// NOTE(nerance): Дозволені лише 0 - False и 1 - True
class BinaryLogic: public Logic {
public:

  virtual int8_t AND(int8_t lop, int8_t rop) const {
    return lop && rop;
  }

  virtual int8_t OR(int8_t lop, int8_t rop) const {
    return lop || rop;
  }

  virtual int8_t XOR(int8_t lop, int8_t rop) const {
    return lop ^ rop;
  }

  virtual int8_t NOT(int8_t lop) const {
    return !lop;
  }

  virtual int8_t IMPLY(int8_t lop, int8_t rop) const {
    return OR(NOT(lop), rop);
  }
  virtual int8_t IS(int8_t lop, int8_t rop) const {
    return NOT(XOR(lop, rop));
  }

};

//------------------------------------------------------------------------------
// Task 3
//------------------------------------------------------------------------------


// NOTE(nerance): Дозволені тільки -1 - False, 0 - Undefined/Maybe , 1 - True
class TernaryLogic: public Logic {
public:
  virtual int8_t AND(int8_t lop, int8_t rop) const {
    return std::min<int8_t>(lop, rop);
  }

  virtual int8_t OR(int8_t lop, int8_t rop) const {
    return std::max(lop, rop);
  }

  int8_t NAND(int8_t lop, int8_t rop) const {
    return -AND(lop, rop);
  }

  int8_t NOR(int8_t lop, int8_t rop) const {
    return -OR(lop, rop);
  }

  virtual int8_t XOR(int8_t lop, int8_t rop) const {
    return OR(AND(lop, NOT(rop)), AND(rop, NOT(lop)));
  }

  virtual int8_t NOT(int8_t lop) const {
    return -lop;
  }

  virtual int8_t SUM(int8_t lop, int8_t rop) const {
    return OR(OR(AND(IS(lop, -1), rop - 1), AND(IS(lop, 0), rop)), AND(IS(lop, 1), rop + 1));
  }

  virtual int8_t IMPLY(int8_t lop, int8_t rop) const {
    return OR(OR(AND(AND(IS(lop, -1), IS(rop, 0)), 0),
                 AND(AND(IS(lop, -1), IS(rop, 1)), 0)),
              OR(AND(AND(IS(lop,  0), IS(rop,-1)), 0),
                 AND(AND(IS(lop,  0), IS(rop, 0)), 0)));
  }
  virtual int8_t IS(int8_t lop, int8_t rop) const {
    return lop == rop;
  }

  virtual void buildTable() {

    //------------------------------------------------------------------------------
    // Task 4
    //------------------------------------------------------------------------------

    BUILD(AND);
    BUILD(NAND);
    BUILD(OR);
    BUILD(NOR);
    BUILD(XOR);
    BUILD(SUM);
    BUILD(IS);

  }

};

int main(int argc, char** argv) {

  BinaryLogic logic;

  std::cout << "Binary logic tests\n\n";

  ASSERT((logic.AND(1, 1)) == 1);
  ASSERT(logic.AND(0, 1)   == 0);
  ASSERT(logic.OR(0, 0)    == 0);
  ASSERT(logic.XOR(0, 1)   == 1);
  ASSERT(logic.XOR(0, 0)   == 0);
  ASSERT(logic.NOT(0)      == 1);
  ASSERT(logic.NOT(1)      == 0);
  ASSERT(logic.IMPLY(1, 0) == 0);
  ASSERT(logic.IMPLY(1, 1) == 1);
  ASSERT(logic.IMPLY(0, 1) == 1);
  ASSERT(logic.IMPLY(0, 0) == 1);
  ASSERT(logic.IS(0, 0)    == 1);
  ASSERT(logic.IS(1, 0)    == 0);
  ASSERT(logic.IS(1, 1)    == 1);

  std::cout << "\n\nTernary logic tests\n\n";

  TernaryLogic tlogic;
  ASSERT(tlogic.AND(1, 1)  ==  1);
  ASSERT(tlogic.AND(0, 1)  ==  0);
  ASSERT(tlogic.AND(-1, 0) == -1);
  ASSERT(tlogic.AND(-1, 1) == -1);
  ASSERT(tlogic.OR(1, 1)   ==  1);
  ASSERT(tlogic.OR(1, 0)   ==  1);
  ASSERT(tlogic.OR(-1, 1)  ==  1);
  ASSERT(tlogic.OR(-1, -1) == -1);
  ASSERT(tlogic.OR(-1, 0)  ==  0);
  ASSERT(tlogic.SUM(-1, 0) ==  0);
  ASSERT(tlogic.SUM(0, 1)  ==  1);
  ASSERT(tlogic.SUM(1, 0)  ==  1);
  ASSERT(tlogic.SUM(1, 1)  ==  1);
  ASSERT(tlogic.IS(1, 0)   ==  0);
  ASSERT(tlogic.IS(1, -1)  ==  0);
  ASSERT(tlogic.IS(-1, -1) == -1);
  ASSERT(tlogic.IS(1, 1)   ==  1);
  ASSERT(tlogic.IS(0, 0)   ==  1);

  tlogic.buildTable();

}