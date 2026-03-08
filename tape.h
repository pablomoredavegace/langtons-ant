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
#include "ant.h"
#include "sliding_vector.h"


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
    explicit Tape(std::uint16_t n_colors) : n_colors_(n_colors) {}
    virtual ~Tape();



    virtual std::vector<std::tuple<int, int, std::uint16_t>> NoWhiteCells() const = 0;

    /**
     * @brief Cambiar la celda (x,y) de color
     * @param x Coordenada X
     * @param y Coordenada Y 
     * @param color Color
     */
    virtual void CambioColor(int x, int y, std::uint16_t color) = 0;

    /**
     * @brief Getters
     * @return Número de filas / columnas / colores / color
     */
    virtual std::size_t GetSizeX() const { return sizeX; };
    virtual std::size_t GetSizeY() const { return sizeY; };
    std::uint16_t GetNumColors() const noexcept { return n_colors; }
    virtual std::uint16_t GetColor(int x, int y) const = 0;


    virtual std::uint16_t CicloColor(std::uint16_t c) const noexcept {
      if(n_colors == 0) return 0;
      return static_cast<std::uint16_t>((c + 1) % n_colors);
    }

    /**
     * @brief Rangos visibles
     */
    virtual int MinX() const noexcept = 0;
    virtual int MaxX() const noexcept = 0;
    virtual int MinY() const noexcept = 0;
    virtual int MaxY() const noexcept = 0;


    /**
     * @brief Condicion de frontera
     */
    virtual void Normal(int& x, int& y, Direction& dir) = 0;

    /**
     * @brief Representación textual de la cinta
     * @param os Flujo de salida
     * @param tape Cinta a mostrar
     * @return Referencia al flujo de salida
     */
    friend std::ostream& operator<<(std::ostream& os, const Tape& tape);

  protected:
    std::uint16_t n_colors_;

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

/**
 * @brief Implementación base para cintas fijas
 */
class TapeFixed : public Tape {
 public:
  TapeFixed(std::size_t sizeX, std::size_t sizeY, std::uint16_t n_colors);

  std::size_t GetSizeX() const noexcept override { return sx_; }
  std::size_t GetSizeY() const noexcept override { return sy_; }

  int MinX() const noexcept override { return 0; }
  int MaxX() const noexcept override { return static_cast<int>(sx_) - 1; }
  int MinY() const noexcept override { return 0; }
  int MaxY() const noexcept override { return static_cast<int>(sy_) - 1; }

  std::uint16_t GetColor(int x, int y) const override;
  void CambioColor(int x, int y, std::uint16_t c) override;

  std::vector<std::tuple<int,int,std::uint16_t>> NoWhiteCells() const override;

 protected:
  bool Limits(int x, int y) const noexcept;
  std::size_t Index(int x, int y) const noexcept;

  std::size_t sx_{0};
  std::size_t sy_{0};
  std::vector<std::uint16_t> cells_;
};

/**
 * @brief Cinta periódica
 */
class TapePeriodic final : public TapeFixed {
 public:
  TapePeriodic(std::size_t sx, std::size_t sy, std::uint16_t n_colors)
      : TapeFixed(sx, sy, n_colors) {}

  void Normal(int& x, int& y, Direction& dir) override;
};

/**
 * @brief Cinta reflectiva
 */
class TapeReflective final : public TapeFixed {
 public:
  TapeReflective(std::size_t sx, std::size_t sy, std::uint16_t n_colors)
      : TapeFixed(sx, sy, n_colors) {}

  void Normal(int& x, int& y, Direction& dir) override;
};

/**
 * @brief Cinta deslizante
*/
class TapeSliding final : public Tape {
 public:
  TapeSliding(std::size_t sx, std::size_t sy, std::uint16_t n_colors);

  std::size_t GetSizeX() const noexcept override;
  std::size_t GetSizeY() const noexcept override;

  int MinX() const noexcept override { return x_min_; }
  int MaxX() const noexcept override { return x_max_; }
  int MinY() const noexcept override { return y_min_; }
  int MaxY() const noexcept override { return y_max_; }

  std::uint16_t GetColor(int x, int y) const override;
  void CambioColor(int x, int y, std::uint16_t c) override;

  void Normal(int& x, int& y, Direction& dir) override;

  std::vector<std::tuple<int,int,std::uint16_t>> NoWhiteCells() const override;

 private:
  void PermX(int x);
  void PermY(int y);

  int x_min_{0}, x_max_{-1};
  int y_min_{0}, y_max_{-1};

  SlidingVector< SlidingVector<std::uint16_t> > grid_;
};

#endif