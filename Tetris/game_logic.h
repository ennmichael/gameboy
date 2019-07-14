#pragma once

/*
 * Code for the actual game elements.
 */

#include "matrice.h"
#include "Sdl-Plus-Plus/drawing.h"
#include "Sdl-Plus-Plus/flow.h"
#include <optional>
#include <vector>
#include <random>
#include <chrono>
#include <cstddef>
#include <utility>
#include <array>
#include <cmath>
#include <unordered_map>

#include <iostream>

namespace Game_logic {
using Score = int;

/*
 * Rotation:
 * used with tetrominos to tell weather they should be rotated
 * clockwise or counter-clockwise.
 */
enum class Rotation : bool { Clockwise, Counter_clockwise };

enum class Angle { Zero = 0, Ninety = 90, One_eighty = 180, Two_seventy = 270 };

/*
 * Tetromino_block_offsets:
 * represents all the blocks of a tetromino offset from
 * it's current position, including (0,0)
 */
class Tetromino_block_offsets {
   public:
    using Blocks = std::unordered_map<Angle, Sdl::Point_vector>;

    explicit Tetromino_block_offsets(const Blocks&) noexcept;

    const Sdl::Point_vector& get() const noexcept;
    void rotate(Rotation) noexcept;

   private:
    const Blocks* blocks_;  // This is a pointer instead of a reference
                            // so that assignment would be permitted.
                            // It is never null.
    Sdl::Point_vector no_points_{};
    Angle ang_{Angle::Zero};
};

Angle rotate_angle(Angle, Rotation) noexcept;
int first_quadrant(int) noexcept;

/*
 * Tetromino:
 * represents a complete tetromino,
 * including it's position and color.
 */
struct Tetromino {
    Tetromino(const Tetromino&) = default;
    Tetromino(Tetromino&&) = default;

    Tetromino& operator=(const Tetromino&) = default;
    Tetromino& operator=(Tetromino&&) = default;

    Sdl::Point pos_;
    Tetromino_block_offsets block_offsets_;
    SDL_Color color_;
};

/*
 * block_positions:
 * returns the block_positions that a tetrkomino takes up, i.e.,
 * every offset + the tetromino position
 */
Sdl::Point_vector block_positions(const Tetromino&);

using Optional_color = std::optional<SDL_Color>;

constexpr std::size_t table_width = 8;
constexpr std::size_t table_height = 16;
constexpr std::size_t tetromino_count = 7;

/*
 * Tetromino_table:
 * all the positions taken up by tetromino blocks.
 */
class Tetromino_table {
   public:
    using Matrice = Util::Matrice<Optional_color, table_width, table_height>;
    using Row = Matrice::value_type;

    void insert(const Tetromino&) noexcept;
    /*
     * clear_rows:
     * check which rows are filled and clear them, returning the
     * number of rows cleared.
     */
    int clear_rows() noexcept;
    const Matrice& blocks() const noexcept;

   private:
    void clear_row(Row&) noexcept;
    /*
     * apply_gravity:
     * shifts all blocks down if they have empty
     * lines beneath them.
     */
    void apply_gravity() noexcept;
    void shift_row(std::size_t) noexcept;
    int clear_and_count_rows() noexcept;

    Matrice blocks_{};
};

/*
 * safe_move:
 * moves the tetromino by the given point,
 * making sure it doesn't exit the screen or
 * overlap with the table.
 */
void safe_move(Tetromino&, const Tetromino_table&, Sdl::Point) noexcept;

/*
 * safe_rotate:
 * does the same checking as safe_move, except
 * it rotates the tetromino.
 */
void safe_rotate(Tetromino&, const Tetromino_table&, Rotation) noexcept;

bool filled(const Tetromino_table::Row&) noexcept;
bool empty(const Tetromino_table::Row&) noexcept;

/*
 * touches_table:
 * tells weather a tetromino touches a table
 * depending on the given point parameter.
 *
 * example: touches_table(my_table, my_tetromino, { 0, 1 })
 * checks weather any block of my_tetromino has a block below it
 * in my_table.
 */
bool touches_table(const Tetromino_table&,
                   const Tetromino&,
                   Sdl::Point) noexcept;

/*
 * table_has_point:
 * checks if there's a block in the
 * table taking up the given point.
 */
bool table_has_point(const Tetromino_table&, Sdl::Point) noexcept;

/*
 * table_overlaps_tetromino:
 * check if the tetromino and the table overlap.
 */
bool table_overlaps_tetromino(const Tetromino_table&,
                              const Tetromino&) noexcept;

/*
 * overflown:
 * tells weather the table has been overflown with tetrominos,
 * usually meaning a game-over.
 */
bool overflown(const Tetromino_table&) noexcept;
bool on_screen(Sdl::Point) noexcept;
bool on_screen(const Tetromino&) noexcept;
bool touches_ground(const Tetromino&) noexcept;

enum class Tetromino_type { I, O, T, J, L, S, Z };

/*
 * Tetromino_factory:
 * creates random tetrominos on demand.
 */
class Tetromino_factory {
   public:
    Tetromino_factory();
    Tetromino create(Sdl::Point);

   private:
    const Tetromino_block_offsets::Blocks& random_blocks() const noexcept;
    SDL_Color random_color() const noexcept;
    void two_versions(Tetromino_type);
    void one_version(Tetromino_type);
    void add_block_proto(Tetromino_type, Angle, Sdl::Point_vector) noexcept;

    using Block_protypes =
        std::unordered_map<Tetromino_type, Tetromino_block_offsets::Blocks>;

    Block_protypes block_protos_{};
    std::vector<SDL_Color> color_protos_{Sdl::color_red(), 
                                         Sdl::color_blue(),
                                         Sdl::color_green(), 
                                         Sdl::color_yellow()};
    mutable std::random_device rd_{};
};
}  // End Game_logic
