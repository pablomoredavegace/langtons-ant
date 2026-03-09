#include "tape.h"
#include <algorithm>

TapeSliding::TapeSliding(std::size_t sx, std::size_t sy, std::uint16_t n_colors)
    : Tape(n_colors) {
  x_min_ = 0;
  y_min_ = 0;
  x_max_ = static_cast<int>(sx) - 1;
  y_max_ = static_cast<int>(sy) - 1;

  grid_ = SlidingVector< SlidingVector<std::uint16_t> >(y_min_, y_max_);
  for (int y = y_min_; y <= y_max_; ++y) {
    grid_[y] = SlidingVector<std::uint16_t>(x_min_, x_max_, 0);
  }
}

std::size_t TapeSliding::GetSizeX() const noexcept { return static_cast<std::size_t>(x_max_ - x_min_ + 1); }
std::size_t TapeSliding::GetSizeY() const noexcept { return static_cast<std::size_t>(y_max_ - y_min_ + 1); }

void TapeSliding::PermX(int x) {
  while (x < x_min_) {
    for (int yy = y_min_; yy <= y_max_; ++yy) grid_[yy].PushFront(0);
    --x_min_;
  }
  while (x > x_max_) {
    for (int yy = y_min_; yy <= y_max_; ++yy) grid_[yy].PushBack(0);
    ++x_max_;
  }
}

void TapeSliding::PermY(int y) {
  while (y < y_min_) {
    grid_.PushFront(SlidingVector<std::uint16_t>(x_min_, x_max_, 0));
    --y_min_;
  }
  while (y > y_max_) {
    grid_.PushBack(SlidingVector<std::uint16_t>(x_min_, x_max_, 0));
    ++y_max_;
  }
}

void TapeSliding::Normal(int& x, int& y, Direction&) {
  PermX(x);
  PermY(y);
}

std::uint16_t TapeSliding::GetColor(int x, int y) const {
  if (x < x_min_ || x > x_max_ || y < y_min_ || y > y_max_) return 0;
  return grid_[y][x];
}

void TapeSliding::CambioColor(int x, int y, std::uint16_t c) {
  Direction d = Direction::Up;
  Normal(x, y, d);
  if (GetNumColors() == 0) return;

  grid_[y][x] = static_cast<std::uint16_t>(c % GetNumColors());
}

std::vector<std::tuple<int,int,std::uint16_t>> TapeSliding::NoWhiteCells() const {
  std::vector<std::tuple<int,int,std::uint16_t>> out;
  for (int y = y_min_; y <= y_max_; ++y) {
    for (int x = x_min_; x <= x_max_; ++x) {
      const auto c = GetColor(x, y);
      if (c != 0) out.emplace_back(x, y, c);
    }
  }
  return out;
}