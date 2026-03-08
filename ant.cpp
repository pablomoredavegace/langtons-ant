
#include "ant.h"
#include <iostream>
#include <utility>

//Colores de fondo y reset
static constexpr const char* Reset = "\033[0m";
static constexpr const char* Rojo = "\033[41m";
static constexpr const char* Amarillo = "\033[43m";
static constexpr const char* Verde = "\033[42m";
static constexpr const char* Azul = "\033[44m";

Ant::Ant(int x, int y, Direction dir, std::string type, std::string antColor, int life) : PosX(x), PosY(y), Dir(dir), 
TypeAnt(std::move(type)), ColorAnt(std::move(antColor)), Life(life) {}


void Ant::TurnLeft() {
  switch (Dir) {
    case Direction::Up: 
      Dir = Direction::Left;
      break;

    case Direction::Down: 
      Dir = Direction::Right;
      break;

    case Direction::Left: 
      Dir = Direction::Down;
      break;

    case Direction::Right: 
      Dir = Direction::Up;
      break;
  }
}

void Ant::TurnRight() {
  switch (Dir) {
    case Direction::Up: 
      Dir = Direction::Right;
      break;

    case Direction::Down: 
      Dir = Direction::Left;
      break;

    case Direction::Left: 
      Dir = Direction::Up;
      break;

    case Direction::Right: 
      Dir = Direction::Down;
      break;
  }
}

void Ant::Move() {
  switch (Dir) {
    case Direction::Up: 
      --PosY;
      break;

    case Direction::Down: 
      ++PosY;
      break;

    case Direction::Left: 
      --PosX;
      break;

    case Direction::Right: 
      ++PosX;
      break;
  }
}

void Ant::MoveDiagonal() {
  switch (Dir) {
    case Direction::Up: 
      --PosY;
      --PosX;
      break;

    case Direction::Down: 
      ++PosY;
      ++PosX;
      break;

    case Direction::Left: 
      --PosX;
      ++PosY;
      break;

    case Direction::Right: 
      ++PosX;
      --PosY;
      break;
  }
}

std::uint16_t Ant::BaseStep(Tape& tape, std::string_view rule, bool diagonal) {

  tape.Normal(PosX, PosY, Dir);

  const std::uint16_t c = tape.GetColor(PosX, PosY);

  const std::uint16_t c2 = NextColor(tape, c);
  tape.CambioColor(PosX, PosY, c2);

  if(!rule.empty()) {
    const char turn = rule[static_cast<std::size_t>(c2) % rule.size()];
    if(turn == 'D' || turn == 'd') {
      TurnRight();
    }
    else TurnLeft();
  }

  if(diagonal) {
    MoveDiagonal();
  }
  else {
    Move();
  }

  tape.Normal(PosX, PosY, Dir);

  return c2;

}

HormigaHervibora::HormigaHervibora(std::string rule, int x, int y, Direction dir, std::string antColor, int life_initial)
  : Ant(x, y, dir, "H-" + rule, std::move(antColor), life_initial), Rule(std::move(rule)) {}

  void HormigaHervibora::Step(Tape& tape) {
    const std::uint16_t c2 = BaseStep(tape, Rule, false);
    MoreLife(static_cast<int>(c2));
  }

HormigaCarnivora::HormigaCarnivora(std::string rule, int voracidad, int x, int y, Direction dir, std::string antColor, int life_initial)
  : Ant(x, y, dir, "C-" + rule, std::move(antColor), life_initial), Rule(std::move(rule)), voracidad_(voracidad) {}

  void HormigaCarnivora::Step(Tape& tape) {
    (void)BaseStep(tape, Rule, true);
  }

// ----------------------Herviboras---------------------------------------------

AntDI::AntDI(int x, int y, Direction dir, std::string antColor, int life_initial)
  : HormigaHervibora("DI", x, y, dir, std::move(antColor), life_initial){}

void AntDI::Step(Tape& tape) {
  const std::uint16_t c2 = BaseStep(tape, "DI", false);
  MoreLife(static_cast<int>(c2));
}

AntDDII::AntDDII(int x, int y, Direction dir, std::string antColor, int life_initial)
  : HormigaHervibora("DDII", x, y, dir, std::move(antColor), life_initial) {}

void AntDDII::Step(Tape& tape) {
  const std::uint16_t c2 = BaseStep(tape, "DDII", false);
  MoreLife(static_cast<int>(c2));
}

AntIIDD::AntIIDD(int x, int y, Direction dir, std::string antColor, int life_initial)
  : HormigaHervibora("IIDD", x, y, dir, std::move(antColor), life_initial) {}

void AntIIDD::Step(Tape& tape) {
  const std::uint16_t c2 = BaseStep(tape, "IIDD", false);
  MoreLife(static_cast<int>(c2));
}

AntDIDI::AntDIDI(int x, int y, Direction dir, std::string antColor, int life_initial)
  : HormigaHervibora("DIDI", x, y, dir, std::move(antColor), life_initial) {}

void AntDIDI::Step(Tape& tape) {
  const std::uint16_t c2 = BaseStep(tape, "DIDI", false);
  MoreLife(static_cast<int>(c2));
}

Ant_IDID::Ant_IDID(int x, int y, Direction dir, std::string antColor, int life_initial)
  : HormigaHervibora("IDID", x, y, dir, std::move(antColor), life_initial) {}

void Ant_IDID::Step(Tape& tape) {
  const std::uint16_t c2 = BaseStep(tape, "IDID", false);
  MoreLife(static_cast<int>(c2));
}

Ant_IDID_45::Ant_IDID_45(int x, int y, Direction dir, std::string antColor, int life_initial)
  : HormigaHervibora("IDID", x, y, dir, std::move(antColor), life_initial) {}

void Ant_IDID_45::Step(Tape& tape) {
  const std::uint16_t c2 =  BaseStep(tape, "IDID", true);
  MoreLife(static_cast<int>(c2));
}

//----------------------------Carnivoras--------------------------------------
CarnDI::CarnDI(int x, int y, Direction dir, int voracidad, std::string antColor, int life_initial)
  : HormigaCarnivora("DI", voracidad, x, y, dir, std::move(antColor), life_initial) {}

void CarnDI::Step(Tape& tape) {
  (void)BaseStep(tape, "DI", true);
}

CarnDDII::CarnDDII(int x, int y, Direction dir, int voracidad, std::string antColor, int life_initial)
  : HormigaCarnivora("DDII", voracidad, x, y, dir, std::move(antColor), life_initial) {}

void CarnDDII::Step(Tape& tape) {
  (void)BaseStep(tape, "DDII", true);
}

CarnIIDD::CarnIIDD(int x, int y, Direction dir, int voracidad, std::string antColor, int life_initial)
  : HormigaCarnivora("IIDD", voracidad, x, y, dir, std::move(antColor), life_initial) {}

void CarnIIDD::Step(Tape& tape) {
  (void)BaseStep(tape, "IIDD", true);
}

CarnDIDI::CarnDIDI(int x, int y, Direction dir, int voracidad, std::string antColor, int life_initial)
  : HormigaCarnivora("DIDI", voracidad, x, y, dir, std::move(antColor), life_initial) {}

void CarnDIDI::Step(Tape& tape) {
  (void)BaseStep(tape, "DIDI", true);
}

Carn_IDID::Carn_IDID(int x, int y, Direction dir, int voracidad, std::string antColor, int life_initial)
  : HormigaCarnivora("IDID", voracidad, x, y, dir, std::move(antColor), life_initial) {}

void Carn_IDID::Step(Tape& tape) {
  (void)BaseStep(tape, "IDID", true);
}


char Ant::Orientation() const {
  switch (Dir) {
    case Direction::Up: 
      return '^';

    case Direction::Down: 
      return 'v';

    case Direction::Left: 
      return '<';

    case Direction::Right: 
      return '>';
    
    default: 
      return '?';
  }
}

static bool Inicial(const std::string& s, const char* prefijo) {
  return s.rfind(prefijo, 0) == 0;
}

static bool ParseCarnivoras(const std::string& type, int& voracidad, std::string& rule) {
  if (type.empty() || type[0] != 'C') return false;

  std::size_t i = 1;
  int v = 0;
  bool has_digits = false;

  while (i < type.size() && std::isdigit(static_cast<unsigned char>(type[i]))) {
    has_digits = true;
    v = v * 10 + (type[i] - '0');
    ++i;
  }

  if (i < type.size() && type[i] == '-') {
    rule = type.substr(i + 1);
    voracidad = has_digits ? v : 50;
    if (voracidad < 0) voracidad = 0;
    if (voracidad > 100) voracidad = 100;
    return !rule.empty();
  }

  return false;
}

std::unique_ptr<Ant> CrearHormiga(const std::string& type, int x, int y, Direction dir) {

  if (type == "DI")   return std::make_unique<AntDI>(x, y, dir);
  if (type == "DDII") return std::make_unique<AntDDII>(x, y, dir);
  if (type == "IIDD") return std::make_unique<AntIIDD>(x, y, dir);
  if (type == "DIDI") return std::make_unique<AntDIDI>(x, y, dir);

  if (Inicial(type, "H-")) {
    const std::string rule = type.substr(2);
    if (rule == "DI")   return std::make_unique<AntDI>(x, y, dir);
    if (rule == "DDII") return std::make_unique<AntDDII>(x, y, dir);
    if (rule == "IIDD") return std::make_unique<AntIIDD>(x, y, dir);
    if (rule == "DIDI") return std::make_unique<AntDIDI>(x, y, dir);

    return nullptr;
  }

  if (Inicial(type, "C-")) {
    const std::string rule = type.substr(2);
    if (rule == "DI")   return std::make_unique<CarnDI>(x, y, dir, 50);
    if (rule == "DDII") return std::make_unique<CarnDDII>(x, y, dir, 50);
    if (rule == "IIDD") return std::make_unique<CarnIIDD>(x, y, dir, 50);
    if (rule == "DIDI") return std::make_unique<CarnDIDI>(x, y, dir, 50);
    return nullptr;
  }

  int v = 50;
  std::string rule;
  if (ParseCarnivoras(type, v, rule)) {
    if (rule == "DI")   return std::make_unique<CarnDI>(x, y, dir, v);
    if (rule == "DDII") return std::make_unique<CarnDDII>(x, y, dir, v);
    if (rule == "IIDD") return std::make_unique<CarnIIDD>(x, y, dir, v);
    if (rule == "DIDI") return std::make_unique<CarnDIDI>(x, y, dir, v);
    return nullptr;
  }

  return nullptr;
}

std::uint16_t Ant::NextColor(const Tape& tape, std::uint16_t current) const noexcept {
  const std::uint16_t n = tape.GetNumColors();
  if(n == 0) return 0;
  return static_cast<std::uint16_t>((current + 1) % n);
}

std::ostream& operator<<(std::ostream& os, const Ant& ant) {
  if(!ant.GetAntColor().empty()) os << ant.GetAntColor();
  os << ant.GetType() << " (" << ant.GetX() << "," << ant.GetY() << ") " << ant.Orientation();
  if(!ant.GetAntColor().empty()) os << "\033[0m";
  return os;
}