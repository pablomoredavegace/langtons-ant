
#include "ant.h"
#include <iostream>

Ant::Ant(int x, int y, Direction dir, std::string type, std::string antColor) : PosX(x), PosY(y), Dir(dir), 
TypeAnt(std::move(type)), ColorAnt(std::move(antColor)) {}


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

void Ant::BaseStep(Tape& tape, std::string_view rule) {
  if(!tape.Limits(PosX, PosY)) {
    return;
  }

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
   
  Move();

}

AntDI::AntDI(int x, int y, Direction dir, std::string ColorAnt) : Ant(x, y, dir, "DI", std::move(ColorAnt)) {}

void AntDI::Step(Tape& tape) {
  BaseStep(tape, "DI");
}

AntDDII::AntDDII(int x, int y, Direction dir, std::string ColorAnt) : Ant(x, y, dir, "DDII", std::move(ColorAnt)) {}

void AntDDII::Step(Tape& tape) {
  BaseStep(tape, "DDII");
}

AntIIDD::AntIIDD(int x, int y, Direction dir, std::string ColorAnt) : Ant(x, y, dir, "IIDD", std::move(ColorAnt)) {}

void AntIIDD::Step(Tape& tape) {
  BaseStep(tape, "IIDD");
}

AntDIDI::AntDIDI(int x, int y, Direction dir, std::string ColorAnt) : Ant(x, y, dir, "DIDI", std::move(ColorAnt)) {}

void AntDIDI::Step(Tape& tape) {
  BaseStep(tape, "DIDI");
}

/*
void Ant::Step(Tape& tape) {
  const bool black = tape.IsBlack(PosX, PosY);
  tape.CambioColor(PosX, PosY);
  if(black) {
    TurnRight();
  } else { TurnLeft(); }
  Move();
}
*/
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

std::ostream& operator<<(std::ostream& os, const Ant& ant) {
  if(!ant.GetAntColor().empty()) os << ant.GetAntColor();
  os << ant.GetType() << " (" << ant.GetX() << "," << ant.GetY() << ") " << ant.Orientation();
  if(!ant.GetAntColor().empty()) os << "\033[0m";
  return os;
}