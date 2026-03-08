
#include "tape.h"
#include <iostream>
#include <stdexcept>

Tape::~Tape() = default;

TapeFixed::TapeFixed(size_t sizeX, size_t sizeY, std::uint16_t n_colors) 
  : Tape(n_colors), sx_(sizeX), sy_(sizeY), cells_(sizeX * sizeY, 0) {}

bool TapeFixed::Limits(int x, int y) const noexcept{
  return x >= 0 && y >= 0 && static_cast<std::size_t>(x) < sx_ && static_cast<std::size_t>(y) < sy_;
}

std::size_t TapeFixed::Index(int x, int y) const noexcept {
  return static_cast<std::size_t>(y) * sx_ + static_cast<std::size_t>(x);
}

std::uint16_t TapeFixed::GetColor(int x, int y) const {
  if (!Limits(x, y)) return 0;
  return cells_[Index(x, y)];
}

void TapeFixed::CambioColor(int x, int y, std::uint16_t c) {
  if (!Limits(x, y)) return;
  if (GetNumColors() == 0) return;
  cells_[Index(x, y)] = static_cast<std::uint16_t>(c % GetNumColors());
}

std::vector<std::tuple<int,int,std::uint16_t>> TapeFixed::NoWhiteCells() const {
  std::vector<std::tuple<int,int,std::uint16_t>> out;
  for (std::size_t y = 0; y < sy_; ++y) {
    for (std::size_t x = 0; x < sx_; ++x) {
      const auto c = cells_[y * sx_ + x];
      if (c != 0) out.emplace_back(static_cast<int>(x), static_cast<int>(y), c);
    }
  }
  return out;
}

//-----------------Periodic---------------------
static int ModWrap(int v, int m) {
  int r = v % m;
  if (r < 0) r += m;
  return r;
}

void TapePeriodic::Normal(int& x, int& y, Direction&) {
  const int W = static_cast<int>(sx_);
  const int H = static_cast<int>(sy_);
  x = ModWrap(x, W);
  y = ModWrap(y, H);
}


// ---------- Reflective ----------
void TapeReflective::Normal(int& x, int& y, Direction& dir) {
  bool bounce = false;

  const int maxX = static_cast<int>(sx_) - 1;
  const int maxY = static_cast<int>(sy_) - 1;

  if (x < 0)    { x = 0;    bounce = true; }
  if (x > maxX) { x = maxX; bounce = true; }
  if (y < 0)    { y = 0;    bounce = true; }
  if (y > maxY) { y = maxY; bounce = true; }

  if (bounce) {
    switch (dir) {
      case Direction::Left:  dir = Direction::Right; break;
      case Direction::Right: dir = Direction::Left;  break;
      case Direction::Up:    dir = Direction::Down;  break;
      case Direction::Down:  dir = Direction::Up;    break;
    }
  }
}

/*
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
*/