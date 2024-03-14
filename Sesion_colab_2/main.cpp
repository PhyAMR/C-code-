#include <algorithm> // Contiene std::sort y los algoritmos de ordenación 
#include <iostream> // Contiene el objeto global std::cin
#include <fstream> // Flujo a fichero
#include <string>
#include <vector> // Contenedor lineal
#include <cppexten/generator>
#include <cppexten/print>
#include <cppexten/ranges> // Contiene vistas com stdx::views::chunk_by para averiguar todos los elementos identicos y agruparlos
#include <cmath>
#include <random>

namespace stdx = std::experimental; // Para no ponerlo constantemente

struct Point { double x, y, z; };

struct Character {
    std::string name;
    Point location;
};

auto main() -> int {

   auto rng = std::mt19937{std::random_device{}()};	// [0, 2^32)

   auto xyz_distr = std::uniform_real_distribution{-25.0, 25.0};
	
   auto _xyz_rand = [&]() -> double { return xyz_distr(rng); };

   auto heroes = std::vector<std::string>{
            "Chewbacca",
            "Han Solo",
            "Leia Organa",
            "Luke Skywalker",
            "Obi-Wan Kenobi",
            "R2-D2",
            "Yoda" };

   auto villains = std::vector<std::string>{
            "Conde Dooku",
            "Darth Maul",
            "Darth Vader",
            "Jabba el Hutt",
            "Jango Fett",
            "Grand Moff Tarkin",
            "Palpatine" };

   std::ranges::shuffle(heroes, rng);         

   auto random_char = [&](std::string& name){return Character{name,  _xyz_rand(), _xyz_rand(), _xyz_rand()}; };
   auto aliado1 = random_char(heroes[0]);
   auto aliado2 = random_char(heroes[1]);
   auto aliado3 = random_char(heroes[2]);
   
   stdx::println("{:_^70}", "Heroes");
   stdx::println("{:>20}  Posición: x={:>5.1f}  y={:>5.1f}  Z={:>5.1f}", aliado1.name, aliado1.location.x, aliado1.location.y, aliado1.location.z );
   stdx::println("{:>20}  Posición: x={:>5.1f}  y={:>5.1f}  Z={:>5.1f}", aliado2.name, aliado2.location.x, aliado2.location.y, aliado2.location.z );
   stdx::println("{:>20}  Posición: x={:>5.1f}  y={:>5.1f}  Z={:>5.1f}", aliado3.name, aliado3.location.x, aliado3.location.y, aliado3.location.z );
   std::ranges::shuffle(villains, rng);
   auto villano1 = random_char(villains[0]);
   auto villano2 = random_char(villains[1]);
   auto villano3 = random_char(villains[2]);
   stdx::println("{:_^70}", "Villanos");
   stdx::println("{:>20}  Posición: x={:>5.1f}  y={:>5.1f}  Z={:>5.1f}", villano1.name, villano1.location.x, villano1.location.y, villano1.location.z );
   stdx::println("{:>20}  Posición: x={:>5.1f}  y={:>5.1f}  Z={:>5.1f}", villano2.name, villano2.location.x, villano2.location.y, villano2.location.z );
   stdx::println("{:>20}  Posición: x={:>5.1f}  y={:>5.1f}  Z={:>5.1f}", villano3.name, villano3.location.x, villano3.location.y, villano3.location.z );

}
   
   
