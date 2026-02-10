#ifndef TAPE_H
#define TAPE_H

/*   Tape, representa la cinta bidimensional de celdas con valor binario, color blanca/negra,
sobre la que se mueve la hormiga. Aunque teóricamente se trata de una cinta de tamaño
infinito, en la implementación se establece un tamaño fijo para cada dimensión, sizeX y
sizeY. De esta manera, cada celda se identifica mediante un par de coordenadas en el
rango: 0..sizeX-1, 0..sizeY-1.
La clase Tape oculta los detalles de su implementación, y provee las operaciones que
permiten acceder a una celda para consultar o modificar su color.

Se contemplan dos formas de inicializar la cinta:
○ Por defecto, todas las celdas son blancas (valor binario 0).
○ También se puede inicializar leyendo desde un fichero de texto las coordenadas de
las celdas negras (valor binario 1).

La visualización por pantalla también es responsabilidad de la cinta. Para ello sobrecarga
el operador de inserción en flujo, que muestra por pantalla la visualización en modo texto.
Las celdas blancas se muestran con el carácter ‘ ‘, y las celdas negras se muestran con
el carácter ‘X’. Opcionalmente se puede utilizar las secuencias de escape ANSI [4] para
mostrar caracteres a color en modo texto.*/


#include <vector>
#include <iostream>

class Tape {
  public:
  Tape(std::size_t sizeX, std::size_t sizeY);
  
  bool Limits(int x, int y) const;
  bool IsBlack(int x, int y) const;

  void SetBlack(int x, int y, bool IsBlack);
  void CambioColor(int x, int y);

  std::size_t GetSizeX() const;
  std::size_t GetSizeY() const;

  std::vector<std::pair<int,int>> BlackCells() const;

  friend std::ostream operator<<(std::ostream& os, const Tape& tape);

  private:
  std::size_t Index(int x, int y) const;

  std::size_t sizeX;
  std::size_t sizeY;
  
  std::vector<unsigned char> cells;
};

#endif