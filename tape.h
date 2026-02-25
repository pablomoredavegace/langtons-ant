/**
 * @file tape.h
 * @brief Declaración de la clase Tape para representar la cinta bidimensional
 */

#ifndef TAPE_H
#define TAPE_H

#include <tuple>
#include <vector>
#include <iostream>
#include <cstdint>


/**
 * @class Tape
 * @brief Representa la cinta bidimensional finita de celdas con n colores
 * 
 * Tape, representa la cinta bidimensional de celdas sobre la que se mueven las hormigas. 
 * Además, se encarga de la visualización por pantalla
 */


class Tape {
  public:

    /**
     * @brief Construcción de una cinta de tamaño fijo sizeX * sizeY con n colores
     * @param sizeX Número de columnas
     * @param sizeY Número de filas
     * @param colors Colores
     */
    Tape(std::size_t sizeX, std::size_t sizeY, std::uint16_t colors);
    
    /**
     * @brief Comprobar si la celda (x,y) está dentro de la cinta
     * @param x Coordenada X
     * @param y Coordenada Y
     * @return true si está en el rango de la cinta
     */
    bool Limits(int x, int y) const;

    /**
     * @brief Cambiar la celda (x,y) de color
     * @param x Coordenada X
     * @param y Coordenada Y 
     * @param color Color
     */
    void CambioColor(int x, int y, std::uint16_t color);

    /**
     * @brief Getters
     * @return Número de filas / columnas / colores / color
     */
    std::size_t GetSizeX() const { return sizeX; };
    std::size_t GetSizeY() const { return sizeY; };
    std::uint16_t GetNumColors() const noexcept { return n_colors; }
    std::uint16_t GetColor(int x, int y) const noexcept;

    /**
     * @brief Tupla de coordenadas de celdas no blancas
     * @return Vector de pares (x,y) con celdas negras
     */
    std::vector<std::tuple<int,int, std::uint16_t>> NoWhiteCells() const;

    std::uint16_t CicloColor(std::uint16_t c) const noexcept;

    /**
     * @brief Representación textual de la cinta
     * @param os Flujo de salida
     * @param tape Cinta a mostrar
     * @return Referencia al flujo de salida
     */
    friend std::ostream& operator<<(std::ostream& os, const Tape& tape);

  private:

    /**
     * @brief Convertir coordenadas a índice
     * @param x Coordenada X
     * @param y Coordenada Y 
     * @return Índice
     */
    std::size_t PositionIndex(int x, int y) const;

    std::size_t sizeX{0};
    std::size_t sizeY{0};
    std::uint16_t n_colors{2};
    
    std::vector<std::uint16_t> cells;
};

#endif