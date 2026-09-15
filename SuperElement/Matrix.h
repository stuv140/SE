#pragma once
#include <map>
#include <algorithm>
#include <tuple>
#include <memory>
#include "proxy.h"

/**
 * @file Matrix.h
 * @author Удовченко Егор (Udovchenko.egor@yandex.ru)
 * @brief Файл содержит реализацию класса матрицы
 * @version 0.1.0
 * @date 2026-05-12
 *
 * @copyright Удовченко Егор 2026
 *
 */
template<class T, T N>
class Matrix {
    /// @brief  Это определение типа итератора для std::map, где:

  /// @brief  ключ — std::pair<long, long>(координаты элемента в матрице),
  /// @brief  значение — T(элемент матрицы).
    using IteratorType = typename std::map<std::pair<long, long>, T>::iterator;

public:
    /// @brief Конструктор устанавливает значение элемента по умолчанию
    Matrix() : defaultElement(N)
    {
    };

    ~Matrix() {
    }
    /// @brief Перегрузка оператора [] позволяет обращаться к элементам матрицы по индексу x
    Proxy<T, N> operator[](long x) {
        Proxy<T, N> proxy(this, x);
         return proxy;
    };
    /// @brief Выводит размет матрицы
    size_t size() {
        return matrix.size();
    }
    /// @brief Выводит элемент по умолчанию
    T getDefaultElement() {
        return defaultElement;
    };

    /// @brief поиск элемента по координатам: строки, столбец
    const T& findByCoord(std::pair<long, long> coordinates) const {
        auto it = matrix.find(coordinates);
        if (it == matrix.end()) {
            return defaultElement;
        }
        else {
            return it->second;
        }
    };

    /// @brief удаление элемента
    void removeElement(std::pair<long, long> coordinates) {
        matrix.erase(matrix.find(coordinates));
    };

    /// @brief вставка элемента
    void insertElement(std::pair<long, long> coordinates, T value) {
        matrix.emplace(coordinates, value);
    };

    /// @brief  пользовательский итератор для обхода контейнера
    class Iterator {
    public:
        /// @brief Конструктор инициализирует итератор
        Iterator(IteratorType  matrixIt_) :iterator(matrixIt_) {};

        IteratorType getInsideIterator() {
            return this->iterator;
        }
        /// @brief Продвижение итератора вперед
        void operator++() {
            iterator++;
        };
        /// @brief Возвращает кортеж(long, long, T) — координаты и значение элемента
        std::tuple<long, long, T> operator*() {
            return std::make_tuple(iterator->first.first, iterator->first.second, iterator->second);
        };
        /// @brief продвижение итератора назад
        void operator--() {
            iterator--;
        }
        /// @brief Сравнение итераторов по внутренним итераторам.
        bool operator !=(Iterator& other) {
            return iterator != other.getInsideIterator();
        }
        /// @brief Сравнение итераторов по внутренним итераторам.
        bool operator ==(Iterator& other) {
            return iterator == other.getInsideIterator();
        }

    private:
        IteratorType  iterator;
    };

    /// @brief Возвращает итератор для начала  контейнера.
    Iterator begin() {
        return Iterator(matrix.begin());
    };
    /// @brief Возвращает итератор для конца  контейнера.
    Iterator end() {
        return Iterator(matrix.end());
    };
private:
    /// @brief элемент по умолчанию
    const T defaultElement;
    /// @brief контейнер матрицы
    std::map<std::pair<long, long>, T > matrix;
};