
#include "ant.h"
#include <iostream>

Ant::Ant(int x, int y, Direction dir) : PosX(x), PosY(y), Dir(dir) {}

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

void Ant::Step(Tape& tape) {
  const bool black = tape.IsBlack(PosX, PosY);
  tape.CambioColor(PosX, PosY);
  if(black) {
    TurnRight();
  } else { TurnLeft(); }
  Move();
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

std::ostream& operator<<(std::ostream& os, const Ant& ant) {
  os << "Ant(x=" << ant.GetX() << ", y=" << ant.GetY() << ", dir=" << ant.Orientation() << ")";
  return os;
}