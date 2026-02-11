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
 * @brief Representa la hormiga de Langton, su posición y su orientación.
 * 
 * Si está sobre una celda blanca, cambia el color a negra, 
 *   gira noventa grados a la izquierda y avanza una celda.
 * Si está sobre una celda negra, cambia el color a blanca, 
 *   gira noventa grados a la derecha y avanza una celda
 * 
 */
class Ant {
  public:

    /**
     * @brief Constructor de hormiga con posición y orientación inicial
     * @param x Coordenada X inicial
     * @param y Coordenada Y inicial
     * @param dir Orientación inicial
     */
    Ant(int x, int y, Direction dir);

    /**
     * @brief Ejecutar un paso de la hormiga sobre la cinta
     * @param tape Cinta sobre la que se mueve la hormiga
     */
    void Step(Tape& tape);

    /**
     * @brief Getters
     * @return Coordenada X actual / Coordenada Y actual / Dirección actual
     */
    int GetX() const { return PosX; }
    int GetY() const { return PosY; }
    Direction GetDir() const { return Dir; }


    /**
     * @brief Símbolo de orientación en consola
     * @return '<', '>', '^', 'v'
     */
    char Orientation() const;

  private:

    /**
     * @brief Giros de 90 grados a la izquierda / derecha
     */
    void TurnLeft();
    void TurnRight();

    /**
     * @brief Movimiento hacia delante de la hormiga
     */
    void Move();

    int PosX;
    int PosY;
    Direction Dir;
};

/**
* @brief Representación textual de la hormiga
* @param os Flujo de salida
* @param ant Hormiga a mostrar
* @return Referencia al flujo de salida
*/
std::ostream& operator<<(std::ostream& os, const Ant& ant);

#endif