#ifndef SLIDING_VECTOR_H
#define SLIDING_VECTOR_H

#include <stdexcept>
#include <vector>

/**
 * @brief Manejo de excepciones
 */
class SlidingException : public std::out_of_range {
  public:
    SlidingException(int index, int min, int max)
      : std::out_of_range("Indice del Sliding Vector fuera de rango"),
        index_(index), min_(min), max_(max) {}


    /**
     * @brief Getters
     */
    int GetIndex() const noexcept { return index_; }
    int GetMin() const noexcept { return min_; }
    int GetMax() const noexcept { return max_; }

  private:
    int index_;
    int min_;
    int max_;
};

/**
 * @brief Vector deslizante
 */

template <typename T>
class SlidingVector {
 public:
  SlidingVector() = default;

  SlidingVector(int index_min, int index_max, const T& value = T())
      : index_min_(index_min), index_max_(index_max),
        data_(static_cast<std::size_t>(index_max - index_min + 1), value) {}

  int GetIndexMin() const noexcept { return index_min_; }
  int GetIndexMax() const noexcept { return index_max_; }

  std::size_t GetSize() const noexcept {
    if (index_max_ < index_min_) return 0;
    return static_cast<std::size_t>(index_max_ - index_min_ + 1);
  }

  T& operator[](int i) {
    if (i < index_min_ || i > index_max_) throw SlidingException(i, index_min_, index_max_);
    return data_[static_cast<std::size_t>(i - index_min_)];
  }

  const T& operator[](int i) const {
    if (i < index_min_ || i > index_max_) throw SlidingException(i, index_min_, index_max_);
    return data_[static_cast<std::size_t>(i - index_min_)];
  }

  void PushFront(const T& value = T()) {
    data_.insert(data_.begin(), value);
    --index_min_;
  }

  void PushBack(const T& value = T()) {
    data_.push_back(value);
    ++index_max_;
  }

 private:
  int index_min_{0};
  int index_max_{-1};
  std::vector<T> data_;
};

#endif