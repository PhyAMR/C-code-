#include <algorithm> // Contiene std::sort y los algoritmos de ordenación 
#include <iostream> // Contiene el objeto global std::cin
#include <fstream> // Flujo a fichero
#include <random>
#include <string>
#include <vector> // Contenedor lineal
#include <cppexten/generator>
#include <cppexten/print>
#include <cppexten/ranges> // Contiene vistas com stdx::views::chunk_by para averiguar todos los elementos identicos y agruparlos


namespace stdx = std::experimental; // Para no ponerlo constantemente

struct Point { double x, y, z; };

struct Sable_guard {
   Point location;
   bool alert, attack;
};


auto main() -> int 
{



auto rng = std::mt19937{std::random_device{}()};	// [0, 2^32)

auto xyz_distr = std::uniform_real_distribution{-15.0, 15.0};
	
auto xyz_rand = [&]() -> double { return xyz_distr(rng); };

auto distance = [&](Point const& p1){
   auto const& [x1, y1, z1] = p1;
   return std::sqrt(std::pow(x1, 2) + std::pow(y1, 2) + std::pow(z1, 2));
};



auto sable_guard = std::vector<Sable_guard>{};
for (auto i = int{0}; i<25;++i){
   auto p = Point{.x = xyz_rand(), .y = xyz_rand(), .z = xyz_rand()};
   auto d = distance(p);
   auto Sg = Sable_guard{
      .location = p, 
      .alert = d<10,
      .attack = d<5 ,
   };
   sable_guard.push_back(Sg);
}

auto [alert_iter, last] = std::ranges::partition(sable_guard, &Sable_guard::alert);

auto [attack_iter, alret_iter] = std::ranges::partition(std::ranges::subrange{sable_guard.begin(), alert_iter}, &Sable_guard::attack);

for(Sable_guard SG : std::ranges::subrange{sable_guard.begin(), attack_iter}){
   stdx::println("Attack");
   stdx::println("x = {}, y = {}, z = {}", SG.location.x, SG.location.y, SG.location.z);
   
}

for(Sable_guard SG : std::ranges::subrange{attack_iter, alert_iter}){
stdx::println("Alert");
   stdx::println("x = {}, y = {}, z = {}", SG.location.x, SG.location.y, SG.location.z);
}

}