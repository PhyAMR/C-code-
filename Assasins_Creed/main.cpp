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

     struct Character {                 
         std::string type;    // tipo de personaje ("pure titan", "abnormal titan", "nine titan" o "human")
         std::string name;  // nombre del personaje
         double x, y;  // localización (x, y); x e y vienen expresados en pies             
     };

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Character, name, type, x, y) // Lee el archivo Json y lo almacena en un tipo Target

   auto from_json_lines(std::ifstream& ifs) -> stdx::generator<Character>
{
   auto ln = std::string{}; // String auxilira inicialmente vacio para almacenar cada linea del fichero Jsonl
   while (std::getline(ifs, ln))
   {
      co_yield nlohmann::json::parse(ln).get<Character>(); // Comprueba que el archivo no está corrompido y guarda lo definido en Target
   }
}

auto obtain_targets() -> std::vector<Character>
{
   auto ifs = std::ifstream{"../../attack_titan.jsonl", std::ios::binary}; // Para que funcione en cualquier sistema operativo | "../" Indica que esta 2 carpetas por encima de debug y lee el archivo
   if (!ifs) {
      throw std::ios::failure("Uneable to open the file"); // Retorna la función "if" un valor diferente de 0 ya que daría error
   }
   return from_json_lines(ifs) | stdx::ranges::to<std::vector>();
}

auto main() -> int 
   try {
   
   auto characters = obtain_targets();

   auto titans = std::vector<std::string>{};

   auto humans = std::vector<std::string>{};


   for (auto c = 0; c<characters.size(); c++){
      if(characters[c] = "human"){
         characters.push_back(humans);
      }
      else {
         characters.push_back(titans);
      }
   }

   auto same_titan = [](Character& t1, Character& t2) -> bool { return t1.type == t2.type;};

   for (auto target_grp : titans | stdx::views::chunk_by(same_titan)) {
      // nombre del primer representante del subgrupo:
      auto grp_name = std::ranges::begin(target_grp)->type; 
      stdx::println("{}", grp_name);
      for(auto c=0; c<titans.size(); c++){
         stdx::println("{}->x: {}, y: {}", titans.name, titans.x, titans.y);
      }
      }

   auto human_distance = std::map<std::string, double , double>{};

   std::experimental::print("Introduce un entero: ");
   auto num = int{0};
   (std::cin >> num).ignore();

   auto time =[](Character& c1){return c1.y/25;};
}
catch(std::exception& e){
   stdx::println("{}", e.what());
}

