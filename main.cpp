
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include "simulator.h"

/**
 * @brief Limpiar la salida de consola
 */
void BorrarOutput() {
  std::cout << "\033[H\033[2J\033[3J";
  std::cout.flush();
}

/**
 * @brief Conversión de entero del archivo de entrada a dirección
 * @param d Valor 0,1,2,3
 * @return Dirección equivalente al entero
 */
Direction ParseDirection(int d) {
  switch (d) {
    case 0: 
      return Direction::Left;
    case 1:
      return Direction::Right;
    case 2: 
      return Direction::Up;
    case 3:
      return Direction::Down;
    
    default: 
      return Direction::Up;
  }
}

/**
 * @brief Conversión de dirección a entero para guardar en archivo de salida
 * @param d Dirección
 * @return Entero 0,1,2,3
 */
int TranslateDirection(Direction d) {
  switch (d) {
    case Direction::Left: 
      return 0;
    case Direction::Right:
      return 1;
    case Direction::Up: 
      return 2;
    case Direction::Down:
      return 3;
    
    default: 
      return 2;
  }
}


/**
 * @brief Guardar el estado actual de la simulación en un archivo
 * @param filename Archivo de salida
 * @param sim Simulador con el estado actual
 * @return true si se guardó, false en caso contrario
 */
bool SaveState(const std::string& filename, const Simulator& sim) {
  std::ofstream out(filename);
  if(!out) {
    return false;
  }

  const Tape& tape = sim.GetTape();
  const Ant& ant = sim.GetAnt();

  out << tape.GetSizeX() << " " << tape.GetSizeY() << "\n";
  out << ant.GetX() << " " << ant.GetY() << " " << TranslateDirection(ant.GetDir()) << "\n";
  for(const auto& [x, y] : tape.BlackCells()) {
    out << x << " " << y << "\n";
  }
  return true;
}

int main(int argc, char* argv[]) {

  bool porPasos = false;
  std::string ArchivoInput;
  std::size_t maxSteps = 10000;
  int delayEnMs= 50;

  std::vector<std::string> args(argv + 1, argv + argc);

  int argumentos = 0;

  if(!args.empty() && args[0] == "-p") {
    porPasos = true;
    argumentos = 1;
  }

  bool usoInput = false;
  if(argumentos < static_cast<int>(args.size())) {
    usoInput = true;
    ArchivoInput = args[argumentos++];
  }

  if(!porPasos) {
    if(argumentos < static_cast<int>(args.size())) {
      maxSteps = static_cast<std::size_t>(std::stoull(args[argumentos++]));
    }
    if(argumentos < static_cast<int>(args.size())) {
      delayEnMs = std::stoi(args[argumentos++]);
    }
  }

  Tape tape(60, 30); 
  Ant ant(30, 15, Direction::Up);

  if(usoInput) {
    std::ifstream in(ArchivoInput);
    if(!in) {
      std::cerr << "No se pudo abrir el fichero: " << ArchivoInput << "\n";
      return 1;
    }

    std::size_t sizeX, sizeY;
    in >> sizeX >> sizeY;

    int antX, antY, antDir;
    in >> antX >> antY >> antDir;

    tape = Tape(sizeX, sizeY);
    ant = Ant(antX, antY, ParseDirection(antDir));

    int x, y;
    while(in >> x >> y) {
      tape.SetBlack(x, y, true);
    }
  }

  Simulator sim(tape, ant);

  if(porPasos) {
    while(true) {
      BorrarOutput();
      sim.Print(std::cout);

      std::cout << "Enter: paso, q: salir ";
      std::string inputPaso;
      std::getline(std::cin, inputPaso);
      
      if(inputPaso == "q" || inputPaso == "Q") {
        break;
      }

      if(!sim.Step()) {
        BorrarOutput();
        sim.Print(std::cout);
        std::cout << "\n La hormiga salio de los limites\n";
        break;
      }
    }
  } else {
    for(std::size_t i = 0; i < maxSteps; i++) {
      BorrarOutput();
      sim.Print(std::cout);

      std::cout << "Pasos: " << sim.GetSteps() << " de " << maxSteps << "\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(delayEnMs));

      if(!sim.Step()) {
        std::cout << "\n La hormiga salio de los limites\n";
        break;
      }
    }
  }

  std::cout << "\n Estado final \n";
  sim.Print(std::cout);

  std::cout << "\n ¿Guardar el estado final? (s/n): ";
  char inputSave;
  std::cin >> inputSave;
  if(inputSave == 's' || inputSave == 'S') {
    std::cout << "Nombre del archivo de salida: ";
    std::string ArchivoOutput;
    std::cin >> ArchivoOutput;

    if(!SaveState(ArchivoOutput, sim)) {
      std::cout << "No se pudo guardar el archivo\n";
      return 1;
    }
  }

  return 0;
}