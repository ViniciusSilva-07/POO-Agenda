cmake_minimum_required(VERSION 3.10)
project(Agenda)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Diretório com os headers
include_directories(design)

# Apenas headers por enquanto (não há .cpp)
add_executable(agenda main.cpp)

# main.cpp será vazio por agora ou apenas com int main() {}