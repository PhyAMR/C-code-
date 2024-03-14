#include <algorithm> //Funciones de ordenación, bsqueade. etc
#include <execution>
#include <cppexten/print>  
#include <iostream> //Contiene el objeto global std::cin (flujo estandar deentrada)
#include <string>
#include <random>
#include <vector>
#include <array> // Array estatico

#include <cppexten/mdspan>
#include <cppexten/ranges>  

namespace stdx = std::experimental;

auto main() -> int
{
                                                /*Ejemplo producto cartesiano*/
//  auto nucleobases = std::array<char, 4>{'A', 'C', 'G', 'T'};
//for (auto&&  n1: nucleobases){
//   for (char const  n2: nucleobases){
//      for (auto&&  n3: nucleobases){
//         stdx::println("{} {} {}\n", n1, n2, n3);
//      }
//   }
// }
// for (auto [n1, n2, n3] : stdx::views::cartesian_product(nucleobases, nucleobases, nucleobases) /*nucleobases x nucleobases x nucleobases*/){
//   stdx::println("{} {} {}\n", n1, n2, n3);
// }

auto vec_x = std::vector<int>{};

for (auto i = 0; i<30; ++i){
   vec_x.push_back(i+1);
}

auto vec_y = std::vector<int>{};

for (auto i = 0; i<20; ++i){
   vec_y.push_back(0);
}

auto vec_m = std::vector<int>{};

for (auto i = 0; i<600; ++i){
   vec_m.push_back(i+1);
}


auto const num_rows_m = 20;
auto const num_columns_m = 30;
/*                 Creamos la matriz a                */
auto matrix_m = stdx::mdspan{&vec_m[0], num_rows_m, num_columns_m};

auto matrix_x= stdx::mdspan{&vec_x[0], num_columns_m};

auto matrix_y= stdx::mdspan{&vec_y[0], num_rows_m};

/*Matlab no interpreta las matrices por filas o columnas como si hace mdspan, es decir, mdspan ordena automaticamente por columnas
o filas según lo indiquemos, Matlab no lo hace*/
stdx::println("Matriz M");
stdx::println("Rows: {}", matrix_m.extent(0) /*num_rows*/);
stdx::println("Columns: {}", matrix_m.extent(1) /*num_columns*/);
stdx::println("Rank: {}", matrix_m.rank());
stdx::println("");
/*           Visualizar matriz M    */
for (auto i = 0u; i < matrix_m.extent(0); ++i){
   for (auto j = 0u; j < matrix_m.extent(1); ++j){
      stdx::print(" {:>2} ", matrix_m[i, j]);
   }
   stdx::println("");
}
stdx::println("");

stdx::println("Vector columna X");
stdx::println("Rows: {}", matrix_x.extent(0) /*num_rows*/);
stdx::println("Columns: {}", matrix_x.extent(1) /*num_columns*/);
stdx::println("Rank: {}", matrix_x.rank());
stdx::println("");
/*           Visualizar matriz M    */
for (auto i = 0u; i < matrix_x.extent(0); ++i){
   stdx::print(" {:>5} ", matrix_x[i]);
   stdx::println("");
}
stdx::println("");



/* Paralelizamos la operación de hacer la traspuesta de la matriz A*/

auto indices_mx = std::views::iota(0u, 20u);

std::for_each(
   std::execution::par_unseq,
   std::begin(indices_mx), std::end(indices_mx),
   [&](auto r){
      for (auto c = 0u; c < 30; ++c){
         matrix_y[r] += matrix_m[r, c]*matrix_x[c];
   }
   }
);

stdx::println("Matriz Y");
stdx::println("Rows: {}", matrix_y.extent(0) /*num_rows*/);
stdx::println("Columns: {}", matrix_y.extent(1) /*num_columns*/);
stdx::println("Rank: {}", matrix_y.rank());
stdx::println("");
/*           Visualizar matriz Y    */
for (auto i = 0u; i < matrix_y.extent(0); ++i){
   stdx::print(" {:>2} ", matrix_y[i]);
   stdx::println("");
}
stdx::println("");
auto suma = 0;
for (auto i = 0u; i < matrix_y.extent(0); ++i){
     suma +=matrix_y[i] ;
}
stdx::print(" {:>2} ", suma);
}