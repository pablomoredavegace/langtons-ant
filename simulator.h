
#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <iostream>
#include <vector>
#include <memory>
#include "tape.h"
#include "ant.h"

/**
 * @file simulator.h
 * @brief Declaración de la clase Simulator
 */


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
    Simulator(std::unique_ptr<Tape> tape, std::vector<std::unique_ptr<Ant>> ants);

    /**
     * @brief Ejecución de un paso de simulación
     * @return false si la hormiga está fuera del rango después del paso; true si está dentro
     */
    bool Step();

    /**
     * @brief Devuelve el color de una celda (traduce)
     */
    char CellChar(std::uint16_t color) const;

    /**
     * @brief Muestra el estado actual por pantalla
     * @param os Flujo de salida
     */
    void Print(std::ostream& os) const;


    /**
     * @brief Getters
     * @return Referencia constante a la cinta / hormigas
     */
    const Tape& GetTape() const noexcept { return *tape_; }
    const std::vector<std::unique_ptr<Ant>>& GetAnts() const { return ants_; }
    

    /**
     * @brief Devuelve el número de pasos ejecutados
     * @return Contador de pasos
     */
    std::size_t GetSteps() const { return steps_; }

    private:

      void Colisiones();
      void EliminarHormiga();

      std::unique_ptr<Tape> tape_;
      std::vector<std::unique_ptr<Ant>> ants_;
      std::size_t steps_{0};
};

#endif