#include <iostream>
#include <cstring>
#include <cassert>

//------------------------------------------------------------------------------
// Task #4
//------------------------------------------------------------------------------

template <typename TypeA, typename TypeB>
class Quadruple {
public:
  Quadruple(TypeA a, TypeA b, TypeB c, TypeB d): m_pairA {a, b}, m_pairB {c, d} { }

  const auto& firstTwo() {
    return m_pairA;
  }

  const auto& lastTwo() {
    return m_pairB;
  }

private:

  std::pair<TypeA, TypeA> m_pairA;
  std::pair<TypeB, TypeB> m_pairB;

};

void task4_Test() {

  Quadruple<int, char> quadruple(int(10), int(20), char('a'), char('b'));
  const auto& firstTwo = quadruple.firstTwo();
  const auto& lastTwo  = quadruple.lastTwo();

  assert(firstTwo.first  == int(10));
  assert(firstTwo.second == int(20));
  assert(lastTwo.first   == char('a'));
  assert(lastTwo.second  == char('b'));

  std::cout << "Task 4 succeeded" << std::endl;
}

//------------------------------------------------------------------------------
// Task #5
//------------------------------------------------------------------------------

#define NOTHING T{}

class Tuple {
public:

  ~Tuple() {
    for(size_t i = 0; i < m_used; ++i) {
      if(m_values[i] == nullptr) {
        break;
      }

      delete static_cast<char*>(m_values[i]);
    }
  }

  template <class A, class ...T>
  Tuple(A a, T... value) {
    initialize(a, value...);
  }

  template <class A>
  void initialize(A a) {
    assert(m_used < std::extent<decltype(m_values)>::value);
    m_values[m_used] = new char[sizeof(A)];
    memcpy(m_values[m_used++], &a, sizeof(A));

    for(size_t i = m_used; i < std::extent<decltype(m_values)>::value; ++i) {
      m_values[i] = nullptr;
    }
  }

  template <class A, class ...T>
  void initialize(A a, T... value) {
    assert(m_used < std::extent<decltype(m_values)>::value);
    m_values[m_used] = new char[sizeof(A)];
    memcpy(m_values[m_used++], &a, sizeof(A));

    initialize(value...);
  }

  template <class T>
  T tuple_1() {
    if(m_used <= 0)
      return NOTHING;
    return *static_cast<T*>(m_values[0]);
  }

  template <class T>
  T tuple_2() {
    if(m_used <= 1)
      return NOTHING;
    return *static_cast<T*>(m_values[1]);
  }

  template <class T>
  T tuple_3() {
    if(m_used <= 2)
      return NOTHING;
    return *static_cast<T*>(m_values[2]);
  }

  template <class T>
  T tuple_4() {
    if(m_used <= 3)
      return NOTHING;
    return *static_cast<T*>(m_values[3]);
  }

private:
  void*  m_values[4];
  size_t m_used = 0;
};

void task5_Test() {
  Tuple tuple(int(10), char('a'), float(13.3f));
  assert(tuple.tuple_1<int>() == 10);
  assert(tuple.tuple_2<char>() == 'a');
  assert(tuple.tuple_3<float>() == 13.3f);
  assert(tuple.tuple_4<char>() == char(0));

  std::cout << "Task 5 succeeded" << std::endl;
}

//------------------------------------------------------------------------------
// Task #6
//------------------------------------------------------------------------------


// NOTE(nerance): Приблизний алгоритм наступний:
// В лексикографічно порядку сгенерувати перестановку
// Використовуючи трійкову систему обчислення, послідовно сортуючи знаки
// Згенероване сортування - це пріоритетність, котра вказує на
// Номер знаку, котрий використовується для розрахунку у наступній ітерації.
// Сам знак, як вказано раніше - визначається послідовністю зміною в трійковій
// системі обчислення.

void swap(int *a, int i, int j) {
  int s = a[i];
  a[i] = a[j];
  a[j] = s;
}
bool hasPermutation(int *a, int n) {
  int j = n - 2;
  while (j != -1 && a[j] >= a[j + 1]) j--;
  if (j == -1)
    return false;
  int k = n - 1;
  while (a[j] >= a[k]) k--;
  swap(a, j, k);
  int l = j + 1, r = n - 1;
  while (l<r)
    swap(a, l++, r--);
  return true;
}

bool hasOperations(int* operations, int size) {
  int carry = 1;
  for(int i = size - 1; i >= 0; i--) {
    if(carry == 0) {
      return true;
    }

    int value = operations[i];
    operations[i] = (value + carry) % 3;
    carry = std::max((value + carry) / 3, 0);
  }

  return carry <= 0;
}

std::string permutation(const std::string& numbers) {
  int* priorities = new int[numbers.size() - 1];
  for(size_t i = 0; i < numbers.size() - 1; ++i) {
    priorities[i] = i;
  }

  int* values = new int[numbers.size()];
  int* cpyvalues = new int[numbers.size()];
  int* indecies = new int[numbers.size()];

  for(size_t i = 0; i < numbers.size(); ++i) {
    values[i] = numbers[i] - '0';
  }

  int* operations = new int[numbers.size() - 1];

  while(hasPermutation(priorities, numbers.size() - 1)) {

    memset(operations, 0, sizeof(int) * (numbers.size() - 1));
    operations[numbers.size() - 2] = -1;

    while(hasOperations(operations, numbers.size() - 1)) {
      memcpy(cpyvalues, values, sizeof(int) * (numbers.size()));
      for(size_t i = 0; i < numbers.size(); ++i) {
        indecies[i] = -1;
      }

      for(int i = 0; i < numbers.size() - 1; ++i) {

        int left  = cpyvalues[priorities[i]];
        int right = cpyvalues[priorities[i] + 1];

        if(operations[i] == 0) {
          cpyvalues[priorities[i]] = left + right;
        }
        else if(operations[i] == 1) {
          cpyvalues[priorities[i]] = left * right;
        }
        else if(operations[i] == 2) {
          cpyvalues[priorities[i]] = left - right;
        }

        cpyvalues[priorities[i] + 1] = cpyvalues[priorities[i]];
        indecies[priorities[i] + 1] = priorities[i];

        int idx = priorities[i] + 1;
        while(indecies[idx] != -1) {
          cpyvalues[idx] = cpyvalues[priorities[i]];
          idx = indecies[idx];
        }

        cpyvalues[idx] = cpyvalues[priorities[i]];

        for(int i = 0; i < numbers.size(); ++i) {
          int idx = i;
          while(indecies[idx] != -1) {
            idx = indecies[idx];
          }

          cpyvalues[i] = cpyvalues[idx];
        }

      }

      for(int i = 0; i < numbers.size(); ++i) {
        int idx = i;
        while(indecies[idx] != -1) {
          idx = indecies[idx];
        }

        cpyvalues[i] = cpyvalues[idx];
      }

      if(cpyvalues[0] == 100) {
        for(size_t i = 0; i < numbers.size(); ++i) {
          std::cout << cpyvalues[i] << " ";
        }
        std::cout << std::endl;

        for(size_t i = 0; i < numbers.size() - 1; ++i) {
          std::cout << indecies[i] << " ";
        }
        std::cout << std::endl;

        for(size_t i = 0; i < numbers.size() - 1; ++i) {
          std::cout << operations[i] << " ";
        }
        std::cout << std::endl;

        for(size_t i = 0; i < numbers.size() - 1; ++i) {
          std::cout << priorities[i] << " ";
        }
        std::cout << std::endl << std::endl;
      }
    }
  }
  return "";
}

void task6() {
  permutation("123456");
}

int main(int argc, char** argv) {
  task4_Test();
  task5_Test();
  task6();

}
