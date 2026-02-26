
#include "simulator.h"
#include <iostream>

Simulator::Simulator(Tape tape, std::vector<std::unique_ptr<Ant>> ants) : tape_(std::move(tape)), ants_(std::move(ants)) {}

bool Simulator::Step() {
  for(const auto& a : ants_) {
    if(!tape_.Limits(a->GetX(), a->GetY())) {
      return false;
    }
  }
  
  for(auto& a :ants_) {
    a->Step(tape_);
    if(!tape_.Limits(a->GetX(), a->GetY())) {
      return false;
    }
  }

  ++steps_;
  return true;
}

char Simulator::CellChar(std::uint16_t color) const {
  if(color == 0) {
    return ' ';
  } else if (color == 1) {
    return 'X';
  } else if (color <= 9) {
    return static_cast<char>('0' + color);
  } else if (color <= 35) return static_cast<char>('A' + (color - 10));
  return '#';
}

void Simulator::Print(std::ostream& os) const {
  os << "Paso: " << steps_ << '\n';
  
  const int X = static_cast<int>(tape_.GetSizeX());
  const int Y = static_cast<int>(tape_.GetSizeY());

  std::vector<int> antIndex(static_cast<std::size_t>(X) * static_cast<std::size_t>(Y), -1);
  std::vector<int> antCount(static_cast<std::size_t>(X) * static_cast<std::size_t>(Y), 0);

  for(int i = 0; i < static_cast<int>(ants_.size()); ++i) {
    const int ax = ants_[i]->GetX();
    const int ay = ants_[i]->GetY();
    if(!tape_.Limits(ax, ay)) {
      continue;
    }
    const std::size_t index = static_cast<std::size_t>(ay) * static_cast<std::size_t>(X) + static_cast<std::size_t>(ax);
    if(antIndex[index] == -1) {
      antIndex[index] = i;
    }
    ++antCount[index];
  }

  for(int colY = 0; colY < Y; ++colY) {
    for(int filX = 0; filX < X; ++filX) {
      const std::size_t index = static_cast<std::size_t>(colY) * static_cast<std::size_t>(X) + static_cast<std::size_t>(filX);
      if(antIndex[index] != 1) {
        //Colision
        if(antCount[index] > 1) {
          os << '*';
        } else {
          const auto& a = ants_[antIndex[index]];
          if(!a->GetAntColor().empty()) {
            os << a->GetAntColor();
          }
          os << a->Orientation();
          if(!a->GetAntColor().empty()) {
            os << "\033[0m";
          }
        }
      } else {
        os << CellChar(tape_.GetColor(filX, colY));
      }
    }
    os << '\n';
  }
  os.flush();
}