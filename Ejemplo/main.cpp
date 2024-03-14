#include <algorithm> //Funciones de ordenación, bsqueade. etc
#include <cppexten/print>  
#include <iostream> //Contiene el objeto global std::cin (flujo estandar deentrada)
#include <string>
#include <array> // Array estatico
#include <cppexten/ranges>  

auto main() -> int
{
   namespace stdx = std::experimental;
   auto num = int{25};
   auto grd = double{10.0};
   stdx::println("num={} | grd={} ", num, grd);

   // Solicitamos el nombre al usuario

   auto name = std::string{}; // String inicialmente vacio
   stdx::print("What's your name?");
   std::getline(std::cin, name); //leemos el strin desde el terminal
   stdx::println("Hello, {}!", name);

auto nums = std::array<int, 7>{7, -3, 0, 4, 11, -5, 2};
//std::ranges::sort(nums, std::greater{});
for(int n : nums | std::views::reverse) { //Impresión de array en orden invertido
stdx::println("{}", n);
}

for(int n : nums) { //Impresión de array en orden original
stdx::println("{}", n);
}
}