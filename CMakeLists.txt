cmake_minimum_required(VERSION 3.16)
project(Tarea5_Patrones_de_Diseno CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)

# Ejercicio 1: Singleton - Transaction Manager concurrente
add_executable(ej1_singleton ej1/main.cpp)
target_link_libraries(ej1_singleton PRIVATE Threads::Threads)

# Ejercicio 2: Abstract Factory - Motor multiplataforma (integrante 1)
# add_executable(ej2_abstract_factory ej2/main.cpp)

# Ejercicio 3: Composite - Expresiones matematicas (integrante 1)
# add_executable(ej3_composite ej3/main.cpp)

# Ejercicio 4: Iterator - Recorrido de grafo
add_executable(ej4_iterator ej4/main.cpp)
