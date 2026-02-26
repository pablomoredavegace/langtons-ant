
#include "tape.h"
#include <iostream>
#include <stdexcept>


Tape::Tape(size_t sizeX, size_t sizeY, std::uint16_t n_Colors) : sizeX(sizeX), sizeY(sizeY), n_colors(n_Colors), cells(sizeX * sizeY, 0) {}

bool Tape::Limits(int x, int y) const {
  return x >= 0 && y >= 0 && static_cast<std::size_t>(y) < sizeY && static_cast<std::size_t>(x) < sizeX;
}

std::size_t Tape::PositionIndex(int x, int y) const {
  return static_cast<std::size_t>(y) * sizeX + static_cast<std::size_t>(x);
}


void Tape::CambioColor(int x, int y, std::uint16_t color) {
  if(!Limits(x, y)) {
    throw std::out_of_range("La celda está fuera de rango");
  }
  if(n_colors == 0) {
    return;
  }
  cells[PositionIndex(x, y)] = static_cast<std::uint16_t>(color % n_colors);
}

std::uint16_t Tape::GetColor(int x, int y) const {
  if(!Limits(x, y)) {
    throw std::out_of_range("La celda está fuera de rango");
  }
  return cells[PositionIndex(x, y)];
}

std::vector<std::tuple<int, int, std::uint16_t>> Tape::NoWhiteCells() const {
  std::vector<std::tuple<int, int, std::uint16_t>> out;
  out.reserve(256);
  for(std::size_t y = 0; y < sizeY; ++y) {
    for(std::size_t x = 0; x < sizeX; ++x){
      const std::uint16_t c = cells [y * sizeX + x];
      if(c != 0) {
        out.emplace_back(static_cast<int>(x), static_cast<int>(y), c);
      }
    }
  }
  return out;
}

std::uint16_t Tape::CicloColor(std::uint16_t c) const noexcept {
  if(n_colors == 0) return 0;
  return static_cast<std::uint16_t>((c+1) % n_colors);
}

std::ostream& operator<<(std::ostream& os, const Tape& tape) {
  for(std::size_t y = 0; y < tape.sizeY; y++) {
    for(std::size_t x = 0; x < tape.sizeX; x++) {
      os << (tape.cells[y * tape.sizeX + x] ? 'X' : ' ');
    }
    os << '\n'; 
  }
  return os;
}