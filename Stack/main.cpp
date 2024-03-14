#include <algorithm> // Contiene std::sort y los algoritmos de ordenación 
#include <iostream> // Contiene el objeto global std::cin permite al usuario introducir datos
#include <fstream> // Flujo a fichero
#include <string>
#include <vector> // Contenedor lineal
#include <cppexten/generator>
#include <cppexten/print>
#include <cppexten/ranges> // Contiene vistas com stdx::views::chunk_by para averiguar todos los elementos identicos y agruparlos

namespace stdx = std::experimental;

struct item{
   std::string name;
   double price;

   ~item(){ /* Destructor de la clase item */
      stdx::println("Destroying element {} for {}$", name, price);
   }
};

struct Student{
   std::string name;
   double grade_1;
   double grade_2;
   double grade_3;
   double grade_4;

   auto avarage() -> double{ 
      return (grade_1+grade_2+grade_3+grade_4)/4.0;
      }
};

auto main() -> int 
{   
auto students =std::vector<Student>{};
students.push_back({"Phillip Josefino", 5, 7.98, 9, 3});
students.push_back({"Richard Josefino", 3, 2.5, 1.79, 3});
students.push_back({"Roseline Josefino", 8.5, 9.98, 8, 9.99});
students.push_back({"Muñaño Josefino", 8.5, 9.98, 8, 9.99});

for (Student& s : students){
   if (s.grade_2>=8.0){
      ++s.grade_2;
   }
   stdx::println("Name: {} | Grades: {:.2f}  {:.2f}  {:.2f}  {:.2f}",s.name, s.grade_1, s.grade_2, s.grade_3, s.grade_4);
}

stdx::println("Size of one student: {} bytes", sizeof(Student));
stdx::println("Size of all students: {} bytes", sizeof(students));
stdx::println("Number of students: {} students", students.size());

try { //Busca excepciones que fallan en el programa

auto p = students.begin(); //Inicia el vector students por uno
stdx::println("Name: {} | Avarage: {:.2f}",p->name, p->avarage()); //Voy a la primera casilla del vetor
p->grade_2 = 8.5; //Cambio la nota
stdx::println("Name: {} | Modified avarage: {:.2f}",p->name, p->avarage());
++p; //Me muevo por el vector (importante no salirse del vector)
stdx::println("Name: {} | Avarage: {:.2f}",p->name, p->avarage());
stdx::println("Name: {} | Avarage: {:.2f}",students[2].name, students[2].avarage()); //Voy a la segunda casilla del vector
stdx::println("Name: {} | Avarage: {:.2f}",students.at(3).name, students.at(3).avarage()); //Voy a la tercera cassila del vector
stdx::println("Name: {} | Avarage: {:.2f}",students.at(4).name, students.at(4).avarage()); //La función "at" detecta que me he salido del vector
//Student* p = &s; 
//stdx::println("Name: {} | Avarage: {:.2f}",s.name, s.avarage());
//stdx::println("Name: {} | Avarage: {:.2f}",p->name,(*p).avarge());
}
catch (std::exception& e){ //Imprime las excepciones
   stdx::println("The error is: {}", e.what());
}

}

//auto main() -> int 
//{   
//auto i = int{1};
//int* p = &i; /* Averiguamos la dirección en memoria */
//++(*p);
//stdx::println("int = {}", *p);

//std::cout << p; 
//}

//auto main() -> int 
//{   
   //auto ifs = std::ifstream{"../../military_camp.jsonl"};
   
   //auto Item_1 = item {"El conde de Montecristo", 9.95}; /*Esto va al user stack*/
   //stdx::println("Element: {} for {} ", Item_1.name, Item_1.price);
   //Item_1.price = 15.85 ; /*Cambiamos el precio*/
   //stdx::println("Rising price: {} for {} ", Item_1.name, Item_1.price); 
   //stdx::println("Size of element {} = {} bytes", Item_1.name, sizeof(Item_1));
   
   //auto Item_2 = new item {"Guerra y paz", 12.99}; /*Esto va al free store, que tiene más memoria y no se elimina al terimnar su ejecución*/
   //stdx::println("Element: {} for {} ", Item_2->name, Item_2->price);
   //stdx::println("Size of element {} = {} bytes", Item_2->name, sizeof(Item_2));
   //delete Item_2; /*Elimina el archivo de la free store*/
//}