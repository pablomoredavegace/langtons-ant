/**
 * @file ant.h
 * @brief Declaración de la clase Ant con su comportamiento y orientación
 */

#ifndef ANT_H
#define ANT_H

#include <iostream>
#include <memory>

//debug
class Tape;

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
     * @param live Vida de la hormiga
     */
    Ant(int x, int y, Direction dir, std::string type, std::string antColor, int life);
    
    virtual ~Ant() = default;

    /**
     * @brief Ejecutar un paso de la hormiga sobre la cinta, método polimórfico
     * @param tape Cinta sobre la que se mueve la hormiga
     */
    virtual void Step(Tape& tape) = 0;

    /**
     * @brief Getters
     * @return Coordenada X actual / Coordenada Y actual / Dirección actual / Tipo de hormiga / Color / Vida
     */
    int GetX() const { return PosX; }
    int GetY() const { return PosY; }
    Direction GetDir() const { return Dir; }
    std::string_view GetType() const noexcept { return TypeAnt; }
    std::string_view GetAntColor() const noexcept { return ColorAnt; }
    int GetLife() const noexcept { return Life; }
    
    /**
     * @brief Indica si la hormiga está viva
     * @return true si está viva
     */
    bool Alive() const noexcept { return Life > 0; }

    /**
     * @brief Incremento o decremento de la vida de la hormiga
     */
    void LessLife() noexcept { --Life; }
    void MoreLife(int cantidad) noexcept { Life += cantidad; }


    /**
     * @brief Comprobar si una hormiga es carnívora
     */
    virtual bool Carnivora() const noexcept { return false; }

    /**
     * @brief Comprobar la voracidad de la hormiga
     * @return Valor de voracidad
     */
    virtual int Voracidad() const noexcept { return 0; }


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
     * @brief Rotación de 180 grados de la hormiga para TapeReflective
     */
    void RotateAnt();

    /**
     * @brief Movimiento hacia delante de la hormiga
     */
    void Move();
    void MoveDiagonal();

    /**
     * 
     */
    virtual std::uint16_t NextColor(const Tape& tape, std::uint16_t current) const noexcept;

    /**
     * 
     */
    std::uint16_t BaseStep(Tape& tape, std::string_view rule, bool diagonal);

    int PosX{0};
    int PosY{0};
    Direction Dir{Direction::Up};

  private:
    
  std::string TypeAnt;
  std::string ColorAnt;
  int Life{10};

};

/**
* @brief Representación textual de la hormiga
* @param os Flujo de salida
* @param ant Hormiga a mostrar
* @return Referencia al flujo de salida
*/
std::ostream& operator<<(std::ostream& os, const Ant& ant);


class HormigaHervibora : public Ant {
  public:
    HormigaHervibora(std::string rule, int x, int y, Direction dir, std::string antColor, int life_initial);
    void Step(Tape& tape) override;

  private:
    std::string Rule;
};

class AntDI final : public HormigaHervibora {
  public:
    AntDI(int x, int y, Direction dir, std::string antColor, int life_initial = 10);
    void Step(Tape& tape) override;
};

class AntDDII final : public HormigaHervibora {
  public:
    AntDDII(int x, int y, Direction dir, std::string antColor, int life_initial = 10);
    void Step(Tape& tape) override;
};

class AntIIDD final : public HormigaHervibora {
  public:
    AntIIDD(int x, int y, Direction dir, std::string antColor, int life_initial = 10);
    void Step(Tape& tape) override;
};

class AntDIDI final : public HormigaHervibora {
  public:
    AntDIDI(int x, int y, Direction dir, std::string antColor, int life_initial = 10);
    void Step(Tape& tape) override;
};

class Ant_IDID final : public HormigaHervibora {
  public:
    Ant_IDID(int x, int y, Direction dir, std::string antColor, int life_initial = 10);
    void Step(Tape& tape) override;
};

class Ant_IDID_45 final : public HormigaHervibora {
  public:
    Ant_IDID_45(int x, int y, Direction dir, std::string antColor, int life_initial = 10);
    void Step(Tape& tape) override;
};


class HormigaCarnivora : public Ant {
  public:
    HormigaCarnivora(std::string rule, int voracidad, int x, int y, Direction dir, std::string antColor, int life_initial);
    void Step(Tape& tape) override;

    bool Carnivora() const noexcept override { return true; }
    int Voracidad() const noexcept override { return voracidad_; }

  private:
    std::string Rule;
    int voracidad_{50};
};

class CarnDI final : public HormigaCarnivora {
  public:
    CarnDI(int x, int y, Direction dir, std::string antColor, int voracidad = 50, int life_initial = 10);
    void Step(Tape& tape) override;
};

class CarnDDII final : public HormigaCarnivora {
  public:
    CarnDDII(int x, int y, Direction dir, std::string antColor, int voracidad = 50, int life_initial = 10);
    void Step(Tape& tape) override;
};

class CarnIIDD final : public HormigaCarnivora {
  public:
    CarnIIDD(int x, int y, Direction dir, std::string antColor, int voracidad = 50, int life_initial = 10);
    void Step(Tape& tape) override;
};

class CarnDIDI final : public HormigaCarnivora {
  public:
    CarnDIDI(int x, int y, Direction dir, std::string antColor, int voracidad = 50, int life_initial = 10);
    void Step(Tape& tape) override;
};

class Carn_IDID final : public HormigaCarnivora {
  public:
    Carn_IDID(int x, int y, Direction dir, std::string antColor, int voracidad = 50, int life_initial = 10);
    void Step(Tape& tape) override;
};

std::unique_ptr<Ant> CrearHormiga(const std::string& type, int x, int y, Direction dir);

#endif