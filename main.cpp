
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

static std::string Corte(const std::string& s) {
  std::size_t b = 0;
  while (b < s.size() && std::isspace(static_cast<unsigned char>(s[b]))) ++b;
  std::size_t e = s.size();
  while (e > b && std::isspace(static_cast<unsigned char>(s[e - 1]))) --e;
  return s.substr(b, e - b);
}

static std::vector<std::string> SepararHormigas(const std::string& s, char delim) {
  std::vector<std::string> out;
  std::string part;
  std::istringstream iss(s);
  while (std::getline(iss, part, delim)) out.push_back(Corte(part));
  return out;
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
  out << tape.GetSizeX() << " " << tape.GetSizeY() << " " << tape.GetNumColors() << "\n";

  const auto& ants = sim.GetAnts();
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
  std::string tape_modo = "periodic";
  std::size_t maxSteps = 2000;
  int delayEnMs= 50;

  std::vector<std::string> args(argv + 1, argv + argc);

  if(args.empty()) {
    std::cout << "Uso:\n"
    << " " << argv[0] << " input.txt [pasosMax] [delayEnMs]\n"
    << " " << argv[0] << " -p"
    << " " << argv[0] << " -t periodic|reflective|sliding";
    return 1;
  }

int i = 0;
  while (i < (int)args.size()) {
    if (args[i] == "-p") { porPasos = true; ++i; continue; }
    if (args[i] == "-t" && i + 1 < (int)args.size()) { tape_modo = args[i+1]; i += 2; continue; }
    break;
  }

  if (i >= (int)args.size()) {
    std::cerr << "Falta input.txt\n";
    return 1;
  }
  const std::string input = args[i++];

  if (!porPasos) {
    if (i < (int)args.size()) maxSteps = (std::size_t)std::stoull(args[i++]);
    if (i < (int)args.size()) delayEnMs = std::stoi(args[i++]);
  }

  // ---- Leer fichero ----
  std::ifstream in(input);
  if (!in) {
    std::cerr << "No se pudo abrir " << input << "\n";
    return 1;
  }

  std::size_t sx, sy;
  int nColors;
  in >> sx >> sy >> nColors;


  in.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::string line2;
  std::getline(in, line2);


  std::unique_ptr<Tape> tape;
  if (tape_modo == "periodic") {
    tape = std::make_unique<TapePeriodic>(sx, sy, (std::uint16_t)nColors);
  } else if (tape_modo == "reflective") {
    tape = std::make_unique<TapeReflective>(sx, sy, (std::uint16_t)nColors);
  } else if (tape_modo == "sliding") {
    tape = std::make_unique<TapeSliding>(sx, sy, (std::uint16_t)nColors);
  } else {
    std::cerr << "Modo de cinta inválido: " << tape_modo << "\n";
    return 1;
  }

  std::vector<std::unique_ptr<Ant>> ants;
  for (const auto& chunk : SepararHormigas(line2, ';')) {
    if (chunk.empty()) continue;
    std::istringstream iss(chunk);

    std::string type;
    int x, y;
    char dirc;
    iss >> type >> x >> y >> dirc;

    auto a = CrearHormiga(type, x, y, ParseDirection(dirc));
    if (!a) {
      std::cerr << "Tipo de hormiga inválido/no soportado: " << type << "\n";
      return 1;
    }
    ants.push_back(std::move(a));
  }

  if (ants.empty()) {
    std::cerr << "No se cargó ninguna hormiga (línea 2 vacía o mal formada)\n";
    return 1;
  }

  int x, y, c;
  while (in >> x >> y >> c) {
    tape->CambioColor(x, y, (std::uint16_t)c);
  }

  Simulator sim(std::move(tape), std::move(ants));

  if (porPasos) {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (true) {
      BorrarOutput();
      sim.Print(std::cout);

      std::cout << "Enter: paso, q: salir ";
      std::string cmd;
      std::getline(std::cin, cmd);

      if (cmd == "q" || cmd == "Q") break;
      if (!sim.Step()) break;
    }
  } else {
    for (std::size_t s = 0; s < maxSteps; ++s) {
      BorrarOutput();
      sim.Print(std::cout);

      std::this_thread::sleep_for(std::chrono::milliseconds(delayEnMs));
      if (!sim.Step()) break;
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