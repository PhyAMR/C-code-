
                                    /*Examen parcial Álvaro Méndez Rodriguez de tembleque*/
#include <array>
#include <cmath>    // contiene std::sqrt, std::pow, etc
#include <fstream>
#include <map>
#include <random>
#include <string>
#include <vector>

#include <cppexten/print>
#include <cppexten/generator>
#include <cppexten/ranges>

#include <nlohmann/json.hpp>

namespace stdx = std::experimental;

struct Point {
    double x, y; 
};

struct Character {
    std::string name, // nombre del personaje
                town; // población en la que reside
    Point location;   // coordenadas en el mapa 2-dimensional
    bool leader;      // ¿es líder de sus conciudadanos?
};

// función de deserialización auxiliar que sustituye, en este caso, el uso de
// la macro NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE:

void from_json(nlohmann::json const& j, Character& c) 
{
    j.at("name").get_to(c.name);
    j.at("town").get_to(c.town);
    // obtenemos el array de dos doubles de clave "location":
    auto const coordinates = j.at("location").get<std::array<double, 2>>();
    // cargamos los doubles en c.location:
    c.location.x = coordinates[0]; // primera entrada del array como abscisa
    c.location.y = coordinates[1]; // segunda entrada del array como ordenada
    j.at("leader").get_to(c.leader);
}

auto rng = std::mt19937{std::random_device{}()};	// [0, 2^32)
auto x_distr = std::uniform_real_distribution{100.0, 1400.0};
auto y_distr = std::uniform_real_distribution{150.0, 1100.0};


auto main() -> int
{
    auto ifs = std::ifstream{"../../walking_dead.jsonl", std::ios::binary}; // Para que funcione en cualquier sistema operativo | "../" Indica que esta 2 carpetas por encima de debug y lee el archivo
   if (!ifs) {
      throw std::ios::failure("Uneable to open the file"); // Retorna la función "if" un valor diferente de 0 ya que daría error
   }   
    auto ln = std::string{};

    /*auto walking_dead = std::map<std::string, std::vector<Character>>{};
    while (std::getline(ifs, ln)) {
    auto j = nlohmann::json::parse(ln);
    walking_dead[ j ["town"]].push_back(j.get<Character>());
    }*/

    auto x_rand = [&]() -> double { return x_distr(rng); };
    auto y_rand = [&]() -> double { return y_distr(rng); };

    auto zombie = std::vector<Point>{};
    for (auto i = int{0}; i<300; ++i){
        auto z_position = Point{.x = x_rand(), .y = y_rand()};
        zombie.push_back(z_position);
        stdx::println("{}", zombie.size() );
    }
    auto character = std::vector<Character>{};

    std::ranges::sort(character, {}, &Character::name); 

    auto same_town = [](Character& c1, Character& c2) -> bool { return c1.town == c2.town; };

    for (auto town_grp : character | stdx::views::chunk_by(same_town)) {
      // nombre del primer representante del subgrupo:
      stdx::println("_______{}_______", std::ranges::begin(town_grp) -> town);
      auto [part, last] = std::ranges::stable_partition(town_grp, &Character::leader);
      
      for (Character  const& c: std::ranges::subrange(town_grp.begin(), part)){
         stdx::println("{} [[Leader]]", c.name);
      }
     
      for (Character  const& c: std::ranges::subrange(part, last)){
         stdx::println("{} ", c.name);
      }
    }
    
}