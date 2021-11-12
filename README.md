## This library provides function to get a random number or user's defined object.
#### Information
This library uses the `mt19937(32-bit)` as the core engine.
#### Important!
Before using any random functions, the random's seed must be set by using: 
```cpp
auto seed = std::random_device{}(); // to get a random number
RRND::Basic::set_seed(seed);
```
If the seed was not set, any random function would throw an exception.

#### Example
```cpp
#include "ratio_random.h"
#include <random> // for random_device
#include <iostream>

struct Person {
  char const* m_name;
};

int main() {
  // 1. Set random engine's seed
  RRND::Basic::set_seed(std::random_device{}());

  // 2. To observe the seed
  std::cout << std::hex << RRND::Basic::get_seed() << '\n';

  // 3. To get a random number in range of [0:100]
  std::cout << RRND::Basic::random(0U, 100U) << '\n';

  // 4. To get a random object with same ratio
  auto person_1 = RRND::Core<Person>::random({
    { "Jacky" },
    { "Micky" },
    { "Nicky" }
  });
  std::cout << person_1.m_name << '\n';

  // 5. To get a random object with particular ratio
  RRND::Core<Person> random_list;
  random_list.add({ "Jacky" }, 30); // 10/100
  random_list.add({ "Micky" }, 50); // 50/100
  random_list.add({ "Nicky" }, 20); // 20/100
  std::cout << random_list.random().m_name << '\n';

  std::cin.get();

  return 0;
}
```
