#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include "tape.h"
#include "ant.h"

class Simulator {
  public:
    Simulator(Tape tape, Ant ant);

    bool Step();

    void Print(std::ostream& os) const;

    const Tape& GetTape() const { return tape_; }
    const Ant& GetAnt() const { return ant_; }
    
    std::size_t GetSteps() const { return steps_; }

    private:
      Tape tape_;
      Ant ant_;
      std::size_t steps_{0};
};

#endif