
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
  os << tape_;
  os << ant_ << '\n';
}