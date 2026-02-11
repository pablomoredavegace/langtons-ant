/**
 * @file tape.h
 * @brief Declaración de la clase Tape para representar la cinta bidimensional
 */

#ifndef TAPE_H
#define TAPE_H

#include <vector>
#include <iostream>


/**
 * @class Tape
 * @brief Representa la cinta bidimensional finita de celdas binarias (blanco/negro)
 * 
 * Tape, representa la cinta bidimensional de celdas sobre la que se mueve la hormiga. 
 * Además, se encarga de la visualización por pantalla
 */


class Tape {
  public:

    /**
     * @brief Construcción de una cinta de tamaño fijo sizeX * sizeY
     * @param sizeX Número de columnas
     * @param sizeY Número de filas
     */
    Tape(std::size_t sizeX, std::size_t sizeY);
    
    /**
     * @brief Comprobar si la celda (x,y) está dentro de la cinta
     * @param x Coordenada X
     * @param y Coordenada Y
     * @return true si está en el rango de la cinta
     */
    bool Limits(int x, int y) const;

    /**
     * @brief Comprobar si la celda (x,y) está en negro
     * @param x Coordenada X
     * @param y Coordenada Y
     * @return true si está en negro
     */
    bool IsBlack(int x, int y) const;


    /**
     * @brief Inicializar la celda (x,y) a negro o blanco
     * @param x Coordenada X
     * @param y Coordenada Y 
     * @param IsBlack true para coordenada en negro, false en blanco
     */
    void SetBlack(int x, int y, bool IsBlack);

    /**
     * @brief Cambiar la celda (x,y) de color
     * @param x Coordenada X
     * @param y Coordenada Y 
     */
    void CambioColor(int x, int y);

    /**
     * @brief Getters
     * @return Número de filas / columnas
     */
    std::size_t GetSizeX() const { return sizeX; };
    std::size_t GetSizeY() const { return sizeY; };

    /**
     * @brief Vector de coordenadas de las celdas negras
     * @return Vector de pares (x,y) con celdas negras
     */
    std::vector<std::pair<int,int>> BlackCells() const;

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

    std::size_t sizeX;
    std::size_t sizeY;
    
    std::vector<unsigned char> cells;
};

#endif