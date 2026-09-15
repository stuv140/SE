#pragma once

#include "matrix.h"
#include <memory>

template<class T, T N>
class Matrix;

/// @brief Объявление шаблона
/// @brief class T — обычный параметр шаблона: тип.
/// @brief T N — non-type параметр шаблона (значение), причём его тип — это T, то есть тип, переданный первым аргументом.

template<class T, T N>
/**
 * @file Proxy.h
 * @author Удовченко Егор (Udovchenko.egor@yandex.ru)
 * @brief Файл содержит реализацию патерна Proxy
 * @version 0.1.0
 * @date 2026-05-12
 *
 * @copyright Удовченко Егор 2026
 *
 */
class Proxy {
public:
    /// @brief коструктор класса обертки Proxy 
    /// @brief Matrix<T, N> — матрица
    /// @brief T — тип элементов
    /// @brief long — размерность (например, N×N).
    Proxy(Matrix<T, N>* matrixPtr_, long x_) :
        matrixPtr(matrixPtr_), element(0), x(x_)
    {
    };
    /// @brief Деструктор
    ~Proxy() {
    };
    /// @brief перегрузка оператора[] и обращение к элементам массива вида matrix[x][y]
    Proxy& operator[](int y) {
        coord = std::make_pair(x, y);
        element = matrixPtr->findByCoord(coord);
        return *this;
    };
    /// @brief operator= обеспечивает "ленивое" управление памятью или содержимым матрицы: элементы хранятся только, если они отличаются от дефолтного значения, а при присваивании к дефолтному элементу он удаляется
    /// @brief проверка на равенство со стандартным элементом
    /// @brief если новый элемент не дефолтный, то устанавливаем значение элемента.
    /// @brief если новый элемент дефолтный, то удаляем старый элемент или ничего не делаем.
    void operator=(T newValue) {
        if (newValue != matrixPtr->getDefaultElement()) {
            if (element != matrixPtr->getDefaultElement()) {
                element = newValue;
            }
            else {
                matrixPtr->insertElement(coord, newValue);
            }
        }
        else {
            if (element != matrixPtr->getDefaultElement()) {
                matrixPtr->removeElement(coord);
                return;
            }
            else return;
        }
    };
    /// @brief Оператор позволяет объекту вести себя как значение типа T
    operator T() { // для assert(a == -1);
        return element;
    }

private:
    long x;
    /// @brief Строка и столбец в матрице
    std::pair<long, long> coord;
    /// @brief Значение элемента
    T element;
    /// @brief класс Matrix<T, N> управляет коллекцией элементов, и этот указатель обеспечивает связь между элементом и его матрицей.
    Matrix<T, N>* matrixPtr;
};
