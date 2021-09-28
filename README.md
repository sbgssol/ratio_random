# ratio_random
This repository is used to manage, maintain random core, random weight project

# Usage
```cpp
#include "ratio_random.h"
#include <string>

struct Person {
  std::string name;
  // ...
}

int main() {
  // 1. Set seed for the random engine
  RRND::Basic::set_seed(0x10101010);                  
  
  // 2. Get a random number in range of [0U, 100U]
  auto random_number = RRND::Basic::random(0U, 100U);
  
  // 3. Get a random object (string) with same ratio for each object
  auto hello_str = RRND::Core<std::string>::random({"Hello", "Yasou", "Zdravstvuyte", "Hola", "Bonjour"});
  
  // 4. Get a random object (user defined) with modified ratio
  RRND::Core<A> obj_rnd;
  obj_rnd.add(Person{"Peter"}, 10); // 10/total
  obj_rnd.add(Person{"Sam"},   50); // 50/total
  obj_rnd.add(Person{"Alex"},  30); // 30/total
  obj_rnd.add(Person{"Jonny"}, 20); // 20/total
  auto obj = obj.random();
  
  return 0;
}
```
