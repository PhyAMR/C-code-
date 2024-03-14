// ISO C++20:
#include <cmath>
#include <map>    //contenedor asociativo de clave única ordenada.
#include <cctype> //contiene la función "std::tolower".
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numbers> // Tiene constantes como pi o e

// Extensiones de ISO C++20/23:
#include <cppexten/generator>
#include <cppexten/print>
#include <cppexten/ranges> //contiene vistas como stdx::views::chunk_by.

#include <cppexten/format>

namespace stdx = std::experimental;

//Bibliotecas de terceros:

#include <boost/tokenizer.hpp>
#include <dlib/optimization.h> // Contiene el método L-M de regresión no-lineal
#include <units/format.h> // Nos permite realizar el tratamiento de unidades
#include <units/math.h> // Realiza la misma función que cmath pero con unidades
#include <units/generic/angle.h>
#include <units/generic/dimensionless.h>
#include <units/isq/si/length.h>
#include <units/isq/si/international/speed.h>
#include <units/isq/si/acceleration.h>
#include <matplot/matplot.h>

namespace un = units;
namespace usi = units::isq::si;

namespace mp = matplot;

using ang_t = un::angle<un::radian>;
using len_t = usi::length<usi::centimetre>;
using spd_t = usi::speed<usi::international::mile_per_hour>;
using acc_t = usi::acceleration<usi::metre_per_second_sq>;

struct Data {
    ang_t angle; // Rad
    spd_t speed; // Mi/h
    len_t range; // cm
};

auto get_data(std::string path) -> stdx::generator<Data> // Generador basado en corrutina
{
    auto ifs = std::ifstream(path, std::ios::binary);
    if (!ifs) throw std::ios::failure{"Unable to open the CSV file"};

    auto line = std::string{}; // Almacena cada línea del archivo CSV

    while (std::getline(ifs, line)){ // Guarda las lineas del CSV en el string line
        auto values = boost::tokenizer(line, boost::char_separator(",")) // Mientras lo almacena elimina las comas
            | std::views::transform([](std::string values_dubles ){return std::stod(values_dubles); }) //Cambia el texto a doubles 
            | stdx::ranges::to<std::vector<double>>();
        co_yield Data {
            .angle = ang_t{2*std::numbers::pi*values[0]/360.0}, //Convertimos a radianes, ya que la medida esta en grados pero la queremos en radianes
            .speed = spd_t{values[1]},
            .range = len_t{values[2]}
        };
    } 
}

auto theoretic_range(ang_t phi, spd_t v_exp, acc_t grav) -> len_t {
    auto const d = len_t{3.3} ;
    auto const h = len_t{29.8} ;

    auto const cos = un::cos(phi);
    auto const sin = un::sin(phi);
    auto const num1 = un::pow<2>(v_exp) +2*grav*d*sin;
    return num1*(sin+un::sqrt(un::pow<2>(sin) +2*grav*h/num1))*cos/grav;
}
auto main() -> int 
{
    auto const data = get_data("../../parabolic.csv")
                    | stdx::ranges::to<std::vector<Data>>();

    //Dlib_____________________________________________________UNSAFE____________________________________________________________________
    using parameter_t = dlib::matrix<double, 1, 1>; // Tipo de parametro a optimizar
    
    // Función de cálculo de residuos (diferencia entre alcance teórico y experimental)
    auto residual = [](Data const& dato, parameter_t const& param) -> double {
        auto const [ang, spd, r] = dato;
        return (theoretic_range(ang, spd, acc_t{param(0)})- r).number(); // Llamamos a .number por que dlib no trabaja con unidades
    };
    // Estimación inicial (aceleración de la gravedad en m/s^2) para facilitar

    auto grav_param_without_units = parameter_t{11.0};

    /*Ejecutamos el método de L-M con los datos experimentales, optimizando el valor de g. El resultado obtenido por 2 proporciona el 
    RSS (Suma de los Residuos al Cuadrado) medido en cm^2*/

    auto rss_grav_param_without_units = 2.0*dlib::solve_least_squares_lm(dlib::objective_delta_stop_strategy{1.0e-7} , //precisión
                                                                        residual ,
                                                                        dlib::derivative(residual) ,
                                                                        data, //Primeros argumentos de mando a la función 
                                                                        grav_param_without_units);
    //Dlib_____________________________________________________________________________________________________________________________
   
    auto const grav = acc_t{grav_param_without_units(0)};
    auto const rse = len_t{std::sqrt(rss_grav_param_without_units/(data.size()-1))};
                /*:%.2Q %q*/    /*:%.2Q %q*/    /*:%.2Q %q*/
    stdx::println("g = {:%.2Q %q} | RSE = {:%.2Q %q} | RSS = {} cm^2", grav, rse, rss_grav_param_without_units );

/*Gráfica tridimensional de una función

auto f = [](double x, double y){return std::sin(x)+std::cos(y);};
mp::fsurf(f);
mp::show();*/

/*Gráfica de puntos verticales en 3D

auto x = mp::linspace(-1.0*std::numbers::pi/2.0, std::numbers::pi/2.0, 40);
auto z = mp::transform(x, [](double v){return std::cos(v); });
mp::stem3(z);
mp::show();*/

/*Ignoramos todos los datos salvo los ángulos*/

auto tod = [](auto q)->double {return q.number();}; //Elimina las unidades de los valores por que matplot no trabaja con unidades

auto const p = data | std::views::transform(&Data::angle)
                    | std::views::transform(tod) 
                    | stdx::ranges::to<std::vector<double>>(); //Vector de ángulos

/*Vector de velocidades (sin unidades), esto es equivalente a ''p'' (para las velocidades) con la excepción de que no podemos 
declarar constante el vector por que si no no permitiría hacer push_back*/

auto v = std::vector<double>{};
for(Data const& dt : data){
    v.push_back(dt.speed.number());
}

/*Vector de rangos (sin unidades), esto es equivalente a ''p'' (para los rangos) con la excepción de que no podemos 
declarar constante el vector por que si no no permitiría hacer push_back*/

auto r = std::vector<double>{};
for(Data const& dt : data){
    r.push_back(dt.range.number());
}

/*Iteradores que apuntan a los valores máximos y minimos de los ángulos y las velocidades, más tarde accederemos con un
puntero haciendo uso del asterisco*/
auto [pmin, pmax] = std::ranges::minmax_element(p);

auto [vmin, vmax] = std::ranges::minmax_element(v);

mp::gcf()->quiet_mode(mp::on);
mp::xlabel(" rad");
mp::ylabel("Vexp mi/h");
mp::zlabel("r cm");

auto f = [&](double ang, double spd)-> double {
    return theoretic_range(ang_t{ang}, spd_t{spd}), grav).number();
};
/* Representamos la superficie con los valores máximos de ''p'' y ''v'' como límites de los ejes*/
mp::fsurf(f, {*pmin, *pmax}, {*vmin,*vmax})->face_alpha(0.7);
mp::hold(mp::on); //Esperamos hasta tener todo preprarado
mp::stem3(p, v, r, "filled") -> color("r").line_width(1.5).marker_face_color("w");
//* mp::show();

}