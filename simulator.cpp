
#include "simulator.h"
#include <iostream>
#include <algorithm>
#include <unordered_map>
#include <map>

static constexpr const char* Reset = "\033[0m";

struct GroupAnts {
  std::size_t operator()(const std::pair<int,int>& p) const noexcept {
    return (static_cast<std::size_t>(static_cast<std::uint32_t>(p.first)) << 32) ^
      static_cast<std::size_t>(static_cast<std::uint32_t>(p.second));
  }
};

static void PrintCell(std::ostream& os, std::uint16_t c) {
  if (c == 0) { os << "  "; return; }
  const int bg = 41 + (static_cast<int>(c - 1) % 7);
  os << "\033[" << bg << "m" << "  " << Reset;
}

Simulator::Simulator(std::unique_ptr<Tape> tape, std::vector<std::unique_ptr<Ant>> ants) 
  : tape_(std::move(tape)), ants_(std::move(ants)) {}


void Simulator::EliminarHormiga() {
  ants_.erase(std::remove_if(ants_.begin(), ants_.end(), [](const std::unique_ptr<Ant>& a) {
    return !a || !a->Alive();
  }), ants_.end());
}

void Simulator::Colisiones() {

  std::unordered_map<std::pair<int,int>, std::vector<int>, GroupAnts> bucket;
  bucket.reserve(ants_.size() * 2);

  for (int i = 0; i < static_cast<int>(ants_.size()); ++i) {
    if (!ants_[i] || !ants_[i]->Alive()) continue;
    bucket[{ants_[i]->GetX(), ants_[i]->GetY()}].push_back(i);
  }

  for (auto& [pos, idxs] : bucket) {
    if (idxs.size() < 2) continue;

    for (int ci : idxs) {
      Ant* carn = ants_[ci].get();
      if (!carn || !carn->Alive() || !carn->Carnivora()) continue;

      const int v = carn->Voracidad();
      if (v <= 0) continue;

      for (int vi : idxs) {
        if (vi == ci) continue;
        Ant* victim = ants_[vi].get();
        if (!victim || !victim->Alive()) continue;

        int steal = (victim->GetLife() * v) / 100;
        if (steal <= 0) continue;

        victim->MoreLife(-steal);
        carn->MoreLife(steal);
      }
    }
  }
}


bool Simulator::Step() {

  for (auto& a : ants_) {
    if (a && a->Alive()) a->LessLife();
  }

  for (auto& a : ants_) {
    if (!a || !a->Alive()) continue;
    a->Step(*tape_);
  }

  Colisiones();
  EliminarHormiga();

  ++steps_;
  return !ants_.empty();
}

void Simulator::Print(std::ostream& os) const {
  os << "Paso: " << steps_ << "\n";

  const int minx = tape_->MinX();
  const int maxx = tape_->MaxX();
  const int miny = tape_->MinY();
  const int maxy = tape_->MaxY();

  std::map<std::pair<int,int>, std::vector<int>> occ;
  for (int i = 0; i < static_cast<int>(ants_.size()); ++i) {
    if (!ants_[i] || !ants_[i]->Alive()) continue;
    occ[{ants_[i]->GetX(), ants_[i]->GetY()}].push_back(i);
  }

  for (int y = miny; y <= maxy; ++y) {
    for (int x = minx; x <= maxx; ++x) {
      auto it = occ.find({x, y});
      if (it == occ.end()) {
        PrintCell(os, tape_->GetColor(x, y));
        continue;
      }

      const auto& idxs = it->second;
      if (idxs.size() > 1) {
        os << "**";
        continue;
      }

      const Ant* a = ants_[idxs[0]].get();
      os << a->GetAntColor() << a->Orientation() << " " << Reset;
    }
    os << "\n";
  }

  for (const auto& a : ants_) {
    os << a->GetType() << " (" << a->GetX() << "," << a->GetY()
       << ") life=" << a->GetLife() << "\n";
  }
  os.flush();
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


/*
static void PrintCellColor(std::ostream& os, std::uint16_t color) {
  if(color == 0) {
    os << " ";
    return;
  }

  const int CellColor = 41 + (static_cast<int>(color - 1) % 7);
  os << "\033[" << CellColor << "m" << " " << "\033[0m";
}
*/

