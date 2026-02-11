/**
 * @file simulator.h
 * @brief Declaración de la clase Simulator
 */

#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include "tape.h"
#include "ant.h"


/**
 * @class Simulator
 * @brief Simula la hormiga sobre la cinta
 * 
 * Contiene los objetos Tape y Ant y gestiona todos los pasos
 * 
 */
class Simulator {
  public:

    /**
     * @brief Construcción del simulador de la hormiga sobre la cinta
     * @param tape Cinta inicial
     * @param ant Hormiga inicial
     */
    Simulator(Tape tape, Ant ant);

    /**
     * @brief Ejecución de un paso de simulación
     * @return false si la hormiga está fuera del rango después del paso; true si está dentro
     */
    bool Step();


    /**
     * @brief Muestra el estado actual por pantalla
     * @param os Flujo de salida
     */
    void Print(std::ostream& os) const;


    /**
     * @brief Getters
     * @return Referencia constante a la cinta / hormiga
     */
    const Tape& GetTape() const { return tape_; }
    const Ant& GetAnt() const { return ant_; }
    

    /**
     * @brief Devuelve el número de pasos ejecutados
     * @return Contador de pasos
     */
    std::size_t GetSteps() const { return steps_; }

    private:
      Tape tape_;
      Ant ant_;
      std::size_t steps_{0};
};

#endif