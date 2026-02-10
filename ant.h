#ifndef ANT_H
#define ANT_H

#include <iostream>
#include "tape.h"

enum class Direction { Left = 0, Right = 1, Up = 2, Down = 3};

class Ant {
  public:
    Ant(int x, int y, Direction dir);

    void Step(Tape& tape);

    int GetX() const { return PosX; }
    int GetY() const { return PosY; }
    Direction GetDir() const { return Dir; }

    char Orientation() const;

    private:
      void TurnLeft();
      void TurnRight();
      void Move();

      int PosX;
      int PosY;
      Direction Dir;
};

std::ostream& operator<<(std::ostream& os, const Ant& ant);

#endif