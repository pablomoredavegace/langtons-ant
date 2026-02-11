
#include "simulator.h"
#include <iostream>

Simulator::Simulator(Tape tape, Ant ant) : tape_(std::move(tape)), ant_(std::move(ant)) {}

bool Simulator::Step() {
  if(!tape_.Limits(ant_.GetX(), ant_.GetY())) {
    return false;
  }
  
  ant_.Step(tape_);
  steps_++;

  if(!tape_.Limits(ant_.GetX(), ant_.GetY())) {
    return false;
  }
  return true;
}

void Simulator::Print(std::ostream& os) const {
  os << "Paso: " << steps_ << '\n';
  for(int y = 0; y < static_cast<int>(tape_.GetSizeY()); ++y) {
    os << "|";
    for(int x = 0; x < static_cast<int>(tape_.GetSizeX()); ++x) {
      if(x == ant_.GetX() && y == ant_.GetY()) {
        os << ant_.Orientation();
      } else {
        os << (tape_.IsBlack(x, y) ? 'X' : ' ');
      }
    }
  }
  os << '|\n';
}