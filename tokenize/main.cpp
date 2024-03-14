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

auto main() -> int
{
    //lectura del texto en memoria convertido a minúscula.
    auto txiki = [](unsigned char c) {return std::tolower(c); };
    auto txt = mp::fileread("../../ulysses_joyce_4300-0.txt")
            | std::views::transform(txiki)
            | stdx::ranges::to<std::string>();

    //vector de string que contiene todas las palabras del texto.
    auto tokens = boost::tokenizer{txt} | stdx::ranges::to<std::vector<std::string>>();

    //ordenamos el vector lexicograficamente para juntar palbras idénticas.
    std::ranges::sort(tokens); 
    //poblamos el mapa empleando la vista "chunk_by".
    auto same_token = [](std::string t1, std::string t2){return t1==t2;};

    //mapa que asocia a cada frecuencia de ocurriencia el listado de palabras que se repite dichas veces.
/* Creating a map that associates to each frequency of occurrence the list of words that is repeated
that many times. */
    auto frec = std::map <int, std::vector<std::string>, std::ranges::greater>{};

    for (auto token_grp : tokens | stdx::views::chunk_by(same_token)) {
        auto v = std::ranges::distance(token_grp); //número de tokens en el bloque.
        auto token = *token_grp.begin(); //copia del primer representante del bloque.
        frec[v].push_back(token);
    };

    for (auto [v, words] : frec | std::views::take(5)){
        stdx::print("Frecuencia {}: ", v);
        for (std::string w : words) {
            stdx::print("{}, ", w);
        }
        stdx::println("\n________");
    };
    // Creamos un vector de frecuencia: Cada frecuencia se va a repetir 
    // tantas veces como indique la longitud del vector de tokens asociado
    // a dicho número

     auto freqs = std::vector<double>{};
     for (auto [v, words] : frec){
        for (auto idx = 0uz; idx < words.size(); ++idx) {
            freqs.push_back(v); 
        }
     };
    // Las posiciones de ranking asociadas a las frecuencias anteriores
    //[1, 2, 3, ..., freqs.size()]
    auto ranks = std::vector<double>{};
    for (auto idx = 0uz; idx < freqs.size(); ++idx) {
            ranks.push_back(idx+1); 
        }

    // Ajuste de minimos cuadrados ln(freqs) = ln(a)+bln(ranks)
    namespace bstat = boost::math::statistics ;
    auto ln = [](double d) -> double {return std::log10(d); };

    auto [y_intercept, b, r_sqr] = bstat::simple_ordinary_least_squares_with_R_squared(
        ranks | std::views::transform(ln) | stdx::ranges::to<std::vector<double>>(),
        freqs | std::views::transform(ln) | stdx::ranges::to<std::vector<double>>()
    );

    // Imprimimos los resultados en la terminal
    stdx::println("y_intercept = {:.2f}, b = {:.2f}, R2 = {:.2f}", y_intercept, b, r_sqr);

    // Representación "frecuencia vs posición del ranking" en escala log10-log10
    auto a = std::exp(y_intercept); // a= e^(log10(a))
    mp::gcf() ->  quiet_mode(mp::on); // Obtiene la la ventana que contiene la gráfica y espera hasta terminar para representar la recta de ajuste
    auto x = mp::logspace(0, 5);
    auto y = mp::transform(x, [&](double valor){return a*std::pow(valor, b); }); // [&] permite a la lambada usar variables externas
    mp::loglog(x, y);
    mp::hold(mp::on);
    mp::loglog(ranks, freqs, "o");

    mp::xlabel("Ranking positión");
    mp::ylabel("Frequency");
    mp::title("Zipf's law");
    mp::show();

}