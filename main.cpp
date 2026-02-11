
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include "simulator.h"

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

int TranslateDiretion(Direction d) {
  switch (d) {
    case Direction::Left: 
      return 0;
    case Direction::Right:
      return 1;
    case Direction::Up: 
      return 2;
    case Direction::Down:
      return ;
    
    default: 
      return 2;
  }
}


bool SaveState(const std::string filename, const Simulator sim) {
  std::ofstream out(filename);
  if(!out) {
    return false;
  }

  const Tape& tape = sim.GetTape();
  const Ant& ant = sim.GetAnt();

  out << tape.GetSizeX() << " " << tape.GetSizeY() << "\n";
  out << ant.GetX() << " " << ant.GetY() << " " << TranslateDiretion(ant.GetDir()) << "\n";
  for(const auto [x, y] : tape.BlackCells()) {
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

  if(args.empty()) {
    std::cout << "Uso: \n"
    << " " << argv[0] << " input.txt [maxSteps] [delayEnMs]\n"
    << " " << argv[0] << " -p input.txt\n";
  return 1;
  }

  int argumentos = 0;

  if(args[0] == "-p") {
    porPasos = true;
    argumentos = 1;
  }

  if(argumentos >= static_cast<int>(args.size())) {
    std::cout << "Falta el fichero de entrada \n";
    return 1;
  }

  ArchivoInput = args[argumentos++];
  if(!porPasos) {
    if(argumentos < static_cast<int>(args.size())) {
      maxSteps = static_cast<std::size_t>(std::stoull(args[argumentos++]));
    }
    if(argumentos < static_cast<int>(args.size())) {
      delayEnMs = std::stoi(args[argumentos++]);
    }
  }

  std::ifstream in(ArchivoInput);
  if(!in) {
    std::cerr << "No se pudo abrir el fichero: " << ArchivoInput << "\n";
    return 1;
  }

  std::size_t sizeX, sizeY;
  in >> sizeX >> sizeY;

  int antX, antY, antDir;
  in >> antX >> antY >> antDir;

  Tape tape(sizeX, sizeY);
  Ant ant(antX, antY, ParseDirection(antDir));

  int x, y;
  while(in >> x >> y) {
    tape.SetBlack(x, y, true);
  }

  Simulator sim(tape, ant);

  bool UserFin = false;
  bool BoundsFin = false;

  if(porPasos) {
    while(true) {
      sim.Print(std::cout);
      std::cout << "Pasos: " << sim.GetSteps() << "\n";
      std::cout << "Enter: paso, q: salir ";
      
      std::string inputPaso;
      std::getline(std::cin, inputPaso);
      
      if(inputPaso == "q" || inputPaso == "Q") {
        UserFin = true;
        break;
      }

      if(!sim.Step()) {
        BoundsFin = true;
        std::cout << "\n La hormiga salio de los limites\n";
        break;
      }
    }
  } else {
    for(std::size_t i = 0; i < maxSteps; i++) {
      sim.Print(std::cout);
      std::cout << "Pasos: " << sim.GetSteps() << " de " << maxSteps << "\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(delayEnMs));

      if(!sim.Step()) {
        BoundsFin = true;
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