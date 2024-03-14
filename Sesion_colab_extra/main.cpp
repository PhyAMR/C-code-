// ISO C++20:
#include <algorithm> // incluye algoritmos de ordenación, partición, etc
#include <fstream>   // flujos a ficheros
#include <iostream>  // incluye el objeto global std::cin (flujo de entrada estándar)
#include <map>       // contenedor asociativo de clave única ordenada (parejas clave-valor)
#include <string>
#include <vector>    // contenedor secuencial de elementos contiguos en memoria

// ISO C++23:
#include <cppexten/print>
#include <cppexten/ranges>

// bibliotecas de tercero:
#include <nlohmann/json.hpp> // serialización de objetos JSON

struct Game {
    std::string title;   // nombre deljuego
    std::string developer;  // desarrollador
    int year;         //año
    bool won;        // ¿ganó?
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Game, title, developer, year, won)

namespace stdx = std::experimental;

auto main() -> int
{
    // deserializamos el fichero JSON Lines y almacenamos la información en el mapa:
    auto ifs = std::ifstream{"../../game_awards.jsonl", std::ios::binary};
    if (!ifs) {
        stdx::println("unable to open JSON Lines file");
        return -1;
    }

    auto games = std::vector<Game>{};

    auto ln = std::string{}; // string auxiliar para almacenar las líneas JSON
    while (std::getline(ifs, ln)) {
        auto j = nlohmann::json::parse(ln).get<Game>();
        games.push_back(j);
    }

    //agrupar a todos los juegos de un mismo año.
    auto same_year = [](Game const& g1, Game const& g2){ return g1.year == g2.year; };
    
    for (auto year_grp : games | stdx::views::chunk_by(same_year)) {
        stdx::println("{}__________________", std::ranges::begin(year_grp)->year);

        // ordenamos los juegos del año por orden alfabético: 
        std::ranges::sort(year_grp, {}, &Game::title);

        // imprimimos los viejo y desarrolladores:
        for (Game const& c : year_grp) {
            stdx::print("{} - {}", c.title, c.developer);

            // imprimimos si ganó:
            if (c.won) {
                stdx::println("   [WON]");
            }
            
            else {
                stdx::println(" ");
            }
        }
    stdx::println(" ");
    }

    //creamos el mapa para contener los datos, el vector de desarrolladores por nominaciones, y los agrupamos por mismo developer.
    auto nominaciones = std::map<int, std::vector<std::string>, std::ranges::greater>{};
    auto same_developer = [](Game const& d1, Game const& d2){ return d1.developer == d2.developer; };
    auto desarrolladores = std::vector<std::string>{};
    std::ranges::sort(games, {}, &Game::developer);

    //para cada developer mostramos su nombre y calculamos cuantas veces sale, y lo metemos al vector.
    for (auto developer_grp : games | stdx::views::chunk_by(same_developer)) {
        auto develope = std::ranges::begin(developer_grp)->developer;
        auto times = std::ranges::distance(developer_grp);
        nominaciones[times].push_back(develope);
    }

    for (auto [nomis, companys] : nominaciones | std::views::take(3)) {
        stdx::print("{} nominaciones ---> ", nomis);
        for (auto c : companys) {
            stdx::print("{}, ", c);
        }
    stdx::println(" ");
    }
}