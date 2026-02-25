/**
 * @file ant.h
 * @brief Declaración de la clase Ant con su comportamiento y orientación
 */

#ifndef ANT_H
#define ANT_H

#include <iostream>
#include "tape.h"


/**
 * @enum Dirección
 * @brief Orientación de la hormiga
 */
enum class Direction { Left = 0, Right = 1, Up = 2, Down = 3};

/**
 * @class Ant
 * @brief Clase abstracta de hormigas. Representa la hormiga de Langton, su posición y su orientación.
 * 
 */
class Ant {
  public:

    /**
     * @brief Constructor de hormiga con posición y orientación inicial, además de tipo y color
     * @param x Coordenada X inicial
     * @param y Coordenada Y inicial
     * @param dir Orientación inicial
     * @param type Tipo de hormiga (DDII, DI...)
     * @param antColor
     */
    Ant(int x, int y, Direction dir, std::string type, std::string antColor);
    
    virtual ~Ant() = default;

    /**
     * @brief Ejecutar un paso de la hormiga sobre la cinta, método polimórfico
     * @param tape Cinta sobre la que se mueve la hormiga
     */
    virtual void Step(Tape& tape) = 0;

    /**
     * @brief Getters
     * @return Coordenada X actual / Coordenada Y actual / Dirección actual / Tipo de hormiga / Color
     */
    int GetX() const { return PosX; }
    int GetY() const { return PosY; }
    Direction GetDir() const { return Dir; }
    std::string_view GetType() const noexcept { return TypeAnt; }
    std::string_view GetAntColor() const noexcept { return ColorAnt; }


    /**
     * @brief Símbolo de orientación en consola
     * @return '<', '>', '^', 'v'
     */
    char Orientation() const;

  protected:

    /**
     * @brief Giros de 90 grados a la izquierda / derecha
     */
    void TurnLeft();
    void TurnRight();

    /**
     * @brief Movimiento hacia delante de la hormiga
     */
    void Move();

    /**
     * 
     */
    virtual std::uint16_t NextColor(const Tape& tape, std::uint16_t current) const noexcept;

    /**
     * 
     */
    void BaseStep(Tape& tape, std::string_view rule);

    int PosX{0};
    int PosY{0};
    Direction Dir{Direction::Up};

  private:
    
  std::string TypeAnt;
  std::string ColorAnt;

};

/**
* @brief Representación textual de la hormiga
* @param os Flujo de salida
* @param ant Hormiga a mostrar
* @return Referencia al flujo de salida
*/
std::ostream& operator<<(std::ostream& os, const Ant& ant);



class AntDI final : public Ant {
  public:
    AntDI(int x, int y, Direction dir, std::string antColor = "\033[32m");
    void Step(Tape& tape) override;
};

class AntDDII final : public Ant {
  public:
    AntDDII(int x, int y, Direction dir, std::string antColor = "\033[34m");
    void Step(Tape& tape) override;
};

class AntIIDD final : public Ant {
  public:
    AntIIDD(int x, int y, Direction dir, std::string antColor = "\033[35m");
    void Step(Tape& tape) override;
};

class AntDIDI final : public Ant {
  public:
    AntDIDI(int x, int y, Direction dir, std::string antColor = "\033[36m");
    void Step(Tape& tape) override;
};

#endif