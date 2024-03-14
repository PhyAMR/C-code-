//C++20
#include <map> // Contiene el diccionario (contenedor asociativo de clave única de menor a mayor por defecto)
#include <iostream> // Contiene el objeto global std::cin
#include <string>
#include <vector> // Contenedor lineal
//C++23
#include <cppexten/generator> // Contiene un generador de números
#include <cppexten/print>
#include <cppexten/ranges> // Contiene vistas com stdx::views::chunk_by para averiguar todos los elementos identicos y agruparlos


namespace stdx = std::experimental;

auto main() -> int 
{   
   // Mapa inicialmente vacio. Claves: std::strings | Valor: int
   auto phone_book = std::map<std::string, int>{};

   // Poblamos el diccionario map
   phone_book["Carlos"] = 748390204 ;
   phone_book["Ana"] = 759390246 ;
   phone_book["Javi"] = 653027503 ;
   phone_book["Amelia García"] = 840047487 ;
   phone_book["Virginia Beiztegui"] = 754920475 ;

   for (auto [name, phone] : phone_book){
         stdx::println("{:>15}___________{}", name, phone);
   }

}