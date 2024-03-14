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

auto data_a = std::array{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

auto const num_rows_a = 3;
auto const num_columns_a = 4;
/*                 Creamos la matriz a                */
auto matrix_a = stdx::mdspan{&data_a[0], num_rows_a, num_columns_a};
/*Matlab no interpreta las matrices por filas o columnas como si hace mdspan, es decir, mdspan ordena automaticamente por columnas
o filas según lo indiquemos, Matlab no lo hace*/
stdx::println("Matriz A");
stdx::println("Rows: {}", matrix_a.extent(0) /*num_rows*/);
stdx::println("Columns: {}", matrix_a.extent(1) /*num_columns*/);
stdx::println("Rank: {}", matrix_a.rank());

/*           Visualizar matriz a    */
for (auto i = 0u; i < matrix_a.extent(0); ++i){
   for (auto j = 0u; j < matrix_a.extent(1); ++j){
      stdx::print(" {:>2} ", matrix_a[i, j]);
   }
   stdx::println("");
}
stdx::println("");
/*             Creamos la matriz b (traspuesta de a)*/

auto data_b = std::array{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
auto matrix_b = stdx::mdspan{&data_b[0], num_columns_a, num_rows_a};


/*for (auto i = 0u; i < matrix_b.extent(0); ++i){
   for (auto j = 0u; j < matrix_b.extent(1); ++j){
       matrix_b[i, j] = matrix_a[j, i];
   }
}*/
//Cambiamos el bucle anterior por este nuevo
for (auto [i, j] : stdx::views::cartesian_product(std::views::iota(0u, matrix_b.extent(0)), 
                                                  std::views::iota(0u, matrix_b.extent(1)))){
       matrix_b[i, j] = matrix_a[j, i];
}

/*           Visualizar matriz b    */
stdx::println("");
stdx::println("Matriz B");
stdx::println("Rows: {}", matrix_b.extent(0) /*num_rows*/);
stdx::println("Columns: {}", matrix_b.extent(1) /*num_columns*/);
stdx::println("Rank: {}", matrix_b.rank());

for (auto i = 0u; i < matrix_b.extent(0); ++i){
   for (auto j = 0u; j < matrix_b.extent(1); ++j){
       stdx::print(" {:>2} ", matrix_b[i, j]);
   }
   stdx::println("");
}

/* Paralelizamos la operación de hacer la traspuesta de la matriz A*/

auto indices_b = stdx::views::cartesian_product(std::views::iota(0u, matrix_b.extent(0)), 
                                                std::views::iota(0u, matrix_b.extent(1)));

std::for_each(std::execution::par, std::begin(indices_b), std::end(indices_b),
   [&](auto idx){
   auto [r, c] = idx;
   matrix_b[r, c] = matrix_a[c, r];
   }
   );

}