//C++20
#include <algorithm> // Contiene std::sort y los algoritmos de ordenación 
#include <iostream> // Contiene el objeto global std::cin
#include <fstream>
#include <string>
#include <vector> // Contenedor lineal
//C++23
#include <cppexten/generator> // Contiene un generador de números
#include <cppexten/print>
#include <cppexten/ranges> // Contiene vistas com stdx::views::chunk_by para averiguar todos los elementos identicos y agruparlos


namespace stdx = std::experimental;

auto countfrom(int start) -> stdx::generator<int>  // Esto forma parte de una corutina que es una subrutina que no finaliza cada vez que se la llama
{
   auto n = start;
   while (true){
     ++n;  
     co_yield n; // Comando de una corutina
   }
  
}

auto is_even(int i) -> bool 
{
   return i%2 == 0;
}

std::ifstream my_input_file;

auto main() -> int {
std::string Path;
    std::cout<<"Insert the path of the CSV file (): "; std::cin>>Path;
    my_input_file.open(Path, std::ios::in);
    while( my_input_file.fail() )
{
    my_input_file.clear();
    std::cout<<"Incorrect file path, Re-Enter ";
    std::cin>>Path;
    my_input_file.open( Path, std::ios::in ) ;
}
   //auto is_even = [] (int i) -> bool { return i%2 == 0; }; //Es una función que solo vamos a usar 1 vez llamada lambda
   //for (int n : countfrom(7) | std::views::take(50) /*Considera los primeros n sin contar el 0*/ | std::views::filter(is_even)/*Considera solo los pares*/) {
   //   stdx::println("{}", n);
   //}
}