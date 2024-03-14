#include <algorithm> // Contiene std::sort y los algoritmos de ordenación 
#include <iostream> // Contiene el objeto global std::cin
#include <fstream> // Flujo a fichero
#include <string>
#include <vector> // Contenedor lineal
#include <cppexten/generator>
#include <cppexten/print>
#include <cppexten/ranges> // Contiene vistas com stdx::views::chunk_by para averiguar todos los elementos identicos y agruparlos

#include <nlohmann/json.hpp> // Para comprobar que la estructura del Json es correcta

namespace stdx = std::experimental; // Para no ponerlo constantemente

struct Unit  {
       std::string species,
                        type;
       std::array< double, 2 > location;         
  };

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Unit, species, type, location) // Lee el archivo Json y lo almacena en un tipo Unit

   auto from_json_lines(std::ifstream& ifs) -> stdx::generator<Unit>
{
   auto ln = std::string{}; // String auxilira inicialmente vacio para almacenar cada linea del fichero Jsonl
   while (std::getline(ifs, ln))
   {
      co_yield nlohmann::json::parse(ln).get<Unit>(); // Comprueba que el archivo no está corrompido y guarda lo definido en Unit
   }
}

auto obtain_units() -> std::vector<Unit>
{
   auto ifs = std::ifstream{"../../starcraft_map.jsonl", std::ios::binary}; // Para que funcione en cualquier sistema operativo | "../" Indica que esta 2 carpetas por encima de debug y lee el archivo
   if (!ifs) {
      throw std::ios::failure("Uneable to open the file"); // Retorna la función "if" un valor diferente de 0 ya que daría error
   }
   return from_json_lines(ifs) | stdx::ranges::to<std::vector>();
}

auto main() -> int {
   // auto t = Target{.name = "Sarah Connor", .achieved = false};  Tipo de dato del Jsonl
   // stdx::println("Name = {} | Achieved = {}",t.name, t.achieved); 
   auto units = obtain_units();

    // ifs.close();  al usar una clase ifstream el fichero se cierra automaticamente al final de la función main pero deberia cerrarse antes, al crear la función obtain_targets se cierra cada vez que termian esa función
   
   //Lo previo a este punto lo usaremos regularmente (hacer hotkey)
   
   stdx::println("Número de objetivos: {}", units.size());

   std::ranges::sort(units, std::less{} , &Unit::species);

   auto same_species = [](Unit& t1, Unit& t2) -> bool {return t1.species == t2.species; };
  

   for (auto unit_grp : units | stdx::views::chunk_by(same_species)){ 
      auto grp_species = std::ranges::begin(unit_grp) -> species; // Apunta a la primera casilla de cada subgrupo 
       std::ranges::sort(unit_grp, std::less{} , &Unit::type);
         auto same_type = [](Unit& t1, Unit& t2) -> bool {return t1.type == t2.type; 
         };
      for (auto species_grp : unit_grp | stdx::views::chunk_by(same_type)){
         auto grp_type = std::ranges::begin(species_grp) -> type;
            stdx::println("Especie: {} Tropa: {} --> {}", grp_species, grp_type, std::ranges::distance(grp_type));
         }

         

         

      }
   
}
   
   
