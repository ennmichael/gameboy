#ifndef _MATRICE_H__
#define _MATRICE_H__

#include "Sdl-Plus-Plus/drawing.h"
#include <cstddef>
#include <array>
#include <functional>
#include <utility>

/*
 * General utilities for dealing with matrices
 */

namespace Util {
template <class T, std::size_t W, std::size_t H>
using Matrice = std::array<std::array<T, W>, H>;

/*
 * Value_type:
 * quality of life alias. Saves me from writing
 * a lot of stuff in some declarations.
 */
template <class T>
using Value_type = typename T::value_type;

/*
 * Pres_value_type:
 * extracts the value type from the given class,
 * except if the given class is const,
 * the value type will be const too
 * (i.e., the constness is preserved).
 */
template <class, bool is_const>
struct Pres_value_type;

template <class From>
struct Pres_value_type<From, true> {
    using Type = const Value_type<From>;
};

template <class From>
struct Pres_value_type<From, false> {
    using Type = Value_type<From>;
};

/*
 * Pres_value_type_t:
 * a nice interface to the
 * Pres_value_type class.
 */
template <class From>
using Pres_value_type_t =
    typename Pres_value_type<From, std::is_const<From>::value>::Type;

/*
 * row:
 * accesses the nth row of the matrice.
 */
template <class M>
auto& row(M& matrice, std::size_t n) {
    return matrice.at(n);
}

/*
 * at:
 * accesses the matrice element at the position pos.
 */
template <class M>
auto& at(M& matrice, Sdl::Point pos) {
    return matrice.at(pos.y).at(pos.x);
}

/*
 * for_each_row:
 * apply f to each row.
 * The std::size_t parameter of f is the row number.
 */
template <class M>
void for_each_row(M& matrice,
                  std::function<void(std::size_t, Pres_value_type_t<M>&)> f) {
    for (std::size_t y = 0; y < matrice.size(); ++y) {
        f(y, row(matrice, y));
    }
}

/*
 * for_each:
 * apply f to each individual matrice element.
 * The Sdl::Point parameter of f is the element position.
 */
template <class M>
void for_each(M& matrice,
              std::function<void(Sdl::Point,
                                 Pres_value_type_t<Pres_value_type_t<M>>&)> f) {
    for_each_row(matrice, [f](auto y, auto& row) {
        for (std::size_t x = 0; x < row.size(); ++x) {
            f({static_cast<int>(x), static_cast<int>(y)}, row[x]);
        }
    });
}
}

#endif
