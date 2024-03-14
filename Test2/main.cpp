//C++20
#include <algorithm> // Contiene std::sort y los algoritmos de ordenación 
#include <iostream> // Contiene el objeto global std::cin
#include <string>
#include <vector> // Contenedor lineal
//C++23
#include <cppexten/generator> // Contiene un generador de números
#include <cppexten/print>
#include <cppexten/ranges> // Contiene vistas com stdx::views::chunk_by para averiguar todos los elementos identicos y agruparlos


namespace stdx = std::experimental;

struct S{
   char a;
   int i;
   char b;
   
};
auto main() -> int 
{   
stdx::println("Size of object S: {} bytes", sizeof(S));
stdx::println("Aligment of object S: {} bytes", alignof(S));

}