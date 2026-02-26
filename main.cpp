
#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <tuple>
#include <vector>
#include <memory>
#include <sstream>

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
 * @param d Valor < > ^ v
 * @return Dirección equivalente
 */
Direction ParseDirection(char d) {
  switch (d) {
    case '<': 
      return Direction::Left;
    case '>':
      return Direction::Right;
    case '^': 
      return Direction::Up;
    case 'v':
      return Direction::Down;
    
    default: 
      return Direction::Up;
  }
}


static std::unique_ptr<Ant> TranslateAnt(const std::string& type, int x, int y, Direction dir) {
  if(type == "DI") return std::make_unique<AntDI>(x, y, dir);
  if(type == "DDII") return std::make_unique<AntDDII>(x, y, dir);
  if(type == "IIDD") return std::make_unique<AntIIDD>(x, y, dir);
  if(type == "DIDI") return std::make_unique<AntDIDI>(x, y, dir);
  return nullptr;
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
  const auto& ants = sim.GetAnts();

  out << tape.GetSizeX() << " " << tape.GetSizeY() << " " << tape.GetNumColors() << "\n";

  for(std::size_t i = 0; i < ants.size(); ++i) {
    const auto& a = ants[i];
    out << a->GetType() << " " << a->GetX() << " " << a->GetY() << " " << a->Orientation();
    if(i + 1 < ants.size()) out << " ; ";
  }
  out << "\n";

  for(const auto& [x, y, c] : tape.NoWhiteCells()) {
    out << x << " " << y << " " << c << "\n";
  }
  return true;
}

int main(int argc, char* argv[]) {

  bool porPasos = false;
  std::size_t maxSteps = 10000;
  int delayEnMs= 50;

  std::vector<std::string> args(argv + 1, argv + argc);

  if(args.empty()) {
    std::cout << "Uso:\n"
    << " " << argv[0] << " input.txt [pasosMax] [delayEnMs]\n"
    << " " << argv[0] << " -p input.txt\n";
    return 1;
  }

  int argumentos = 0;

  if(args[0] == "-p") {
    porPasos = true;
    argumentos = 1;
  }
  
  bool usoInput = false;
  if(argumentos >= static_cast<int>(args.size())) {
    std::cerr << "Falta el fichero de entrada\n";
    return 1;
  }

  const std::string ArchivoInput = args[argumentos++];

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
    int nColors;
    in >> sizeX >> sizeY >> nColors;

    int antX, antY, antDir;
    in >> antX >> antY >> antDir;

    Tape tape(sizeX, sizeY, static_cast<std::uint16_t>(nColors));

    in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string line2;
    std::getline(in, line2);
    std::vector<std::unique_ptr<Ant>> ants;
    
    std::stringstream ss(line2);
    std::string chunk;
    while(std::getline(ss, chunk, ';')) {
      std::istringstream item(chunk);
      std::string type;
      int x, y;
      char dir;

      item >> type >> x >> y >> dir;
      auto a = TranslateAnt(type, x, y, ParseDirection(dir));
      if(!a) {
        std::cerr << "Tipo no soportado: " << type << "\n";
        return 1;
      }
      ants.push_back(std::move(a));
    }  

    int x, y, c;
    while(in >> x >> y >> c) {
      tape.CambioColor(x, y, static_cast<std::uint16_t>(c));
    }

  Simulator sim(std::move(tape), std::move(ants));

  if(porPasos) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
        std::cout << "\n Una hormiga salio de los limites\n";
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
        BorrarOutput();
        sim.Print(std::cout);
        std::cout << "\n Una hormiga salio de los limites\n";
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