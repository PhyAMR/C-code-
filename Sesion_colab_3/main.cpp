#include <algorithm> // Contiene std::sort y los algoritmos de ordenación 
#include <iostream> // Contiene el objeto global std::cin
#include <fstream> // Flujo a fichero
#include <map>
#include <string>
#include <vector> // Contenedor lineal
#include <cppexten/generator>
#include <cppexten/print>
#include <cppexten/ranges> // Contiene vistas com stdx::views::chunk_by para averiguar todos los elementos identicos y agruparlos

#include <nlohmann/json.hpp> // Para comprobar que la estructura del Json es correcta

namespace stdx = std::experimental; // Para no ponerlo constantemente

struct Character  {
       std::string name, 
                   realm; 
        bool bearer;       
  };
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Character, name, realm, bearer)
//   auto from_json_lines(std::ifstream& ifs) -> stdx::generator<Character>
//{
//   auto ln = std::string{}; // String auxilira inicialmente vacio para almacenar cada linea del fichero Jsonl
//   while (std::getline(ifs, ln))
//   {
//      co_yield nlohmann::json::parse(ln).get<Character>(); // Comprueba que el archivo no está corrompido y guarda lo definido en Unit
//   }
//}


auto main() -> int 
{
auto ifs = std::ifstream{"../../lord_of_the_rings.jsonl", std::ios::binary}; // Para que funcione en cualquier sistema operativo | "../" Indica que esta 2 carpetas por encima de debug y lee el archivo
   if (!ifs) {
      throw std::ios::failure("Uneable to open the file"); // Retorna la función "if" un valor diferente de 0 ya que daría error
   }   
auto ln = std::string{}; // String donde se guarda todo




/* Crear un mapa con la clave "carrera" y el valor "Personaje" */
auto middle_earth = std::map<std::string, std::vector<Character>>{}; // Mapa donde se guarda la estructura de
 // Lee el archivo Json y lo almacena en un tipo Character
/* Analizando el archivo json y empujándolo en el mapa. */

while (std::getline(ifs, ln)) {
   auto j = nlohmann::json::parse(ln);
   middle_earth[ j ["race"]].push_back(j.get<Character>());
}

/* Iterando sobre el mapa e imprimiendo la raza y los personajes de esa raza. */
/*for (auto const& [race, characters] : middle_earth) {
   stdx::println("{}____________________", race);
   for (Character const& c : characters ){
      stdx::println("{} {} {}",c.realm, c.name, c.bearer );
   }
}*/

auto race_to_search = std::string{};
std::getline(std::cin, race_to_search);
std::cout << "Type a race (men, hobbits, elves, maiar, unknown, dwarves): " ;
std::cin >> race_to_search ;

auto iter = middle_earth.find(race_to_search);


if (iter != middle_earth.end()) {
    auto& [_, characters] = *iter ;
    std::ranges::sort(characters, /*std::ranges::less*/{}, &Character::realm);
   auto same_realm = [](Character& c1, Character& c2) -> bool {return c1.realm == c2.realm; }; // Comprueba si dos characters son de idéntico reino
   for (auto realm_group : characters | stdx::views::chunk_by(same_realm)){
      stdx::println("_______{}_______", std::ranges::begin(realm_group) -> realm);
      std::ranges::sort(realm_group, {}, &Character::name);
      /* Usando el algoritmo stable_partition para dividir el rango de caracteres en dos rangos: uno
      con los portadores del anillo y otro con el resto de los personajes. */
      auto [part, last] = std::ranges::stable_partition(realm_group, &Character::bearer);
      /* Impresión de los personajes que son portadores del anillo. */
      for (Character  const& c: std::ranges::subrange(realm_group.begin(), part)){
         stdx::println("{} [[ring-bearer]]", c.name);
      }
      /* Impresión de los personajes que no son portadores de anillos. */
      for (Character  const& c: std::ranges::subrange(part, last)){
         stdx::println("{} ", c.name);
      }
   }
}
   else {

      stdx::println("Sorry,{} doesn't appear, read the book", race_to_search);

   }

}