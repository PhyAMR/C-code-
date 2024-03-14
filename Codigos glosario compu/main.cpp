#include <thread>
#include <iostream> // Contiene std::ranges::equal_ranges
#include <cppexten/print>
#include <vector>
#include <algorithm>

namespace stdx = std::experimental;

struct Enemy {
  char type;
  int level;
};

auto main() -> int 
{   
  auto v = std::vector<Enemy>{{'f',7},{'b',3},{'d',5},{'c',1},{'d',2},{'z',4},
                              {'d',4}};

  std::ranges::sort(v, /*std::ranges::less*/{}, &Enemy::type);

  

  auto [first, last] = std::ranges::equal_range(v, 'd', /*std::ranges::less*/{}, &Enemy::type);

  auto chunk = std::ranges::subrange{first, last};

  std::ranges::sort(chunk, {}, &Enemy::level);

  for (Enemy const& c : chunk){
    stdx::println("({}, {})", c.type, c.level);
  }

}

