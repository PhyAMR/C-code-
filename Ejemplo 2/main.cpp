#include <algoritm>
#include <iostream> // Contiene el objeto global std::cin
#include <string>
#include <vector>
#include <cppexten/print>

auto incrementar(int& m, double& n) -> void  //Con el "&" hacemos uso de cualquier entero con la que usemos la función incrementar
{
   ++m; // Toma un entero y le suma uno
   ++n; // Toma un double y le suma uno
}

auto main () -> int
{
   auto i = int{7};
   auto d = double{8.7};
   incrementar(i, d);
   auto& j = i; // j es un nombre alternativo para usar la variable original (i)
   ++j; // j suma una unidad su valor (j += 1 | j = j + 1)
   std::experimental::println("Enteros i = {} y j = {} | Decimal d = {}", i, j, d); 

   auto names = std::vector<std::string>{}; // Es un vector homogeneo, es decir, todos los vectores son del mismo tipo
   std::experimental::print("Insert name: "); // Pedimos al usuario que introduzca el nombre
   
   auto name = std::string{}; // Crea un string inicialemnte vacio
   std::getline(std::cin, name);
   while (std::getline(std::cin, name))
   {
     names.push_back(name);
     std::experimental::print("Insert name: ");
   }

   std::ranges::sort(names);

   for (auto nm : names) {
      std::experimental::print("Name: {}", nm);
   }
}
