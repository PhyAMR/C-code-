// ISO C++20:
#include <cmath> //funciones matemáticas.
#include <map> //contenedor asociativo de clave única ordenada.
#include <cctype> //contiene la función "std::tolower".
#include <string>
#include <vector>
#include <algorithm>

//Extensiones de ISO C++20/23:
#include <cppexten/print> 
#include <cppexten/ranges> //contiene vistas como stdx::views::chunk_by.

//Bibliotecas de terceros:
#include <boost/math/statistics/linear_regression.hpp>
#include <boost/tokenizer.hpp>
#include <matplot/matplot.h>

namespace mp = matplot;
namespace stdx = std::experimental;

auto uarm_displacement(double a, double t) -> double {
    return a*t*t/2;
}

auto main() -> int
{
    auto a =  0.5;
    auto t = 12.0;
    auto d = uarm_displacement(a, t);
    stdx::println("Displacement = {} m", d);

}