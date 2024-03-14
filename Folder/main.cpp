// ISO C++20:
#include <atomic> // Si no usaramos esto podría ocurrir un data race
#include <chrono> // Realiza medidas temporales
#include <cstdlib>
#include <filesystem> // Sistema de de ficheros de C++
#include <future> // Incluye objetos futuros std::async
#include <map>
#include <mutex>
#include <thread>
#include <string>
#include <vector>


//Extensiones de ISO C++20/23:
#include <cppexten/print> 
#include <cppexten/ranges> //contiene vistas como stdx::views::chunk_by.

namespace stdx = std::experimental;
namespace rn = std::ranges;
namespace fs = std::filesystem; 

struct Extension_info { // Información final a cerca de una extensión determinada (.exe, .txt, etc)
    std::uintmax_t num_files, // Número total de archivos con una extensión determinada
                    total_size; // Tamaño total de bytes acumulado por dicha extensión
};

auto main() -> int
{
    auto const root = fs::path{"C:/Users/alvar/OneDrive/Escritorio/Europea/Tercero/Computacion_avanzada/Maps"};
    if (!fs::is_directory(root)) {
        stdx::println("You must indicate an actual directory/folder", );
        return -1;
    }

    using clock = std::chrono::steady_clock();
    auto const start = clock::now();

    auto dir_iter = fs::recursive_directory_iterator{root};
    auto const paths = rn::subrange{rn::begin(dir_iter), rn::end(dir_iter)}
                     | stdx::ranges::to<std::vector<fs::path>>();

    using map_type = std::map<std::string, Extension_info>; // Definimos el mapa
    auto processed_data = map_type{}; // Inicializamos el mapa
    auto directory_counter = std::atomic<std::uintmax_t>{0}; // Contador que nos mostrará cuantas subcarpetas hay

    auto generate_map = [&](auto chunk) -> map_type{
        auto res = map_type{}; // Genera un mapa para cada ruta
        for (fs::path const& pth : chunk){
            if (fs::is_regular_file(pth)){
                auto const ext = pth.extension().string();
                ++res[ext].num_files;
                res[ext].total_size += fs::file_size(pth);
            }
            else if (fs::is_directory(pth)){
                ++directory_counter;
            }
        }
        return res;
    };

    // Número de hilos que puede usar la maquina
    auto const  hardw_threads = std::thread::hardware_concurrency();
    // Usamos todos los hilos menos 1, que lo hara el main del programa
    auto const num_futures = hardw_threads - 1;
    auto const max_chunk_sz = paths.size()/ hardw_threads;

    auto futures = std::vector<std::future<map_type>>{};

    auto chunk_iter = paths.begin();
    for (auto i = 0ull; i<num_futures; ++i){
        futures.push_back(std::async(std::launch::async, generate_map, 
                                                                        rn::subrange(chunk_iter, chunk_iter + max_chunk_sz)));
        chunk_iter += max_chunk_sz; 
    }

    auto map_main = generate_map(rn::subrange{chunk_iter, paths.end()});

    auto process_map =[&](map_type const& map){
        for (auto const& [ext, info] : map){
            processed_data[ext].num_files += info.num_files;
            processed_data[ext].total_size += info.total_size;
        }
    };

    process_map(map_main);

    for (auto& ftr : futures){
        process_map(ftr.get());
    }

    auto root_file = std::uintmax_t{0};
    auto root_size = std::uintmax_t{0};

    for (auto const& [ext, info] : processed_data) {
        auto const& [num, sz] = info;
        stdx::println("{:>15}: {:>8} files {:>16} bytes", ext, num, sz );
        root_file += num; 
        root_size += sz;
    }

    auto const stop = clock::now();
    auto const duration = duration_cast<std::chrono::milliseconds>(stop - start);
    stdx::println("\n{:>15}: {:>8} files {:>16} bytes | {} folders [{} ms]",
                    "Total" , root_file, root_size, directory_counter, duration.count());
}