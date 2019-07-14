#include "game_logic.h"
#include <utility>
#include <algorithm>
#include <iterator>

namespace Game_logic {
namespace {
using Point_vector_iterator = Sdl::Point_vector::const_iterator;

using Predicate_f = std::function<bool(Sdl::Point)>;

bool any_of_points(const Tetromino& tetromino, Predicate_f f) {
    auto positions = block_positions(tetromino);
    return std::any_of(positions.cbegin(), positions.cend(), f);
}

bool all_of_points(const Tetromino& tetromino, Predicate_f f) {
    auto positions = block_positions(tetromino);
    return std::any_of(positions.cbegin(), positions.cend(), f);
}
}

Tetromino_block_offsets::Tetromino_block_offsets(const Blocks& blocks) noexcept
    : blocks_{&blocks} {}

const Sdl::Point_vector& Tetromino_block_offsets::get() const noexcept {
    return blocks_->at(ang_);
}

void Tetromino_block_offsets::rotate(Rotation rot) noexcept {
    ang_ = rotate_angle(ang_, rot);
}

Angle rotate_angle(Angle ang, Rotation rot) noexcept {
    auto ang_raw = static_cast<int>(ang);
    ang_raw += (rot == Rotation::Clockwise) ? -90 : 90;
    return static_cast<Angle>(first_quadrant(ang_raw));
}

int first_quadrant(int ang) noexcept {
    if (ang == -90) {
        return 270;
    }

    if (ang == 360) {
        return 0;
    }

    return ang;
}

Sdl::Point_vector block_positions(const Tetromino& tetromino) {
    auto result = tetromino.block_offsets_.get();

    for (auto& e : result) {
        e += tetromino.pos_;
    }

    return result;
}

void safe_move(Tetromino& tetromino,
               const Tetromino_table& tbl,
               Sdl::Point move_by) noexcept {
    tetromino.pos_ += move_by;

    if (!on_screen(tetromino) || touches_table(tbl, tetromino, {}))
        tetromino.pos_ -= move_by;
}

void safe_rotate(Tetromino& tetromino,
                 const Tetromino_table& tbl,
                 Rotation rot) noexcept {
    auto new_tetromino = tetromino;
    new_tetromino.block_offsets_.rotate(rot);

    if (on_screen(new_tetromino) &&
        !table_overlaps_tetromino(tbl, new_tetromino)) {
        tetromino = std::move(new_tetromino);
    }
}

void Tetromino_table::insert(const Tetromino& tetromino) noexcept {
    for (const auto& block_pos : block_positions(tetromino)) {
        if (on_screen(block_pos)) {
            Util::at(blocks_, block_pos) = tetromino.color_;
        }
    }
}

int Tetromino_table::clear_rows() noexcept {
    auto rows_cleared = clear_and_count_rows();

    if (rows_cleared > 0)
        apply_gravity();

    return rows_cleared;
}

auto Tetromino_table::blocks() const noexcept -> const Matrice& {
    return blocks_;
}

void Tetromino_table::clear_row(Row& row) noexcept {
    for (auto& block : row) {
        block = std::nullopt;
    }
}

void Tetromino_table::apply_gravity() noexcept {
    Util::for_each_row(blocks_, [this](auto n, auto& row) {
        if (n == 0 || !empty(row) || empty(Util::row(this->blocks_, n - 1)))
            return;

        this->shift_row(n - 1);
        this->clear_row(Util::row(blocks_, n - 1));
        this->apply_gravity();
    });
}

void Tetromino_table::shift_row(std::size_t n) noexcept {
    Util::row(blocks_, n + 1) = Util::row(blocks_, n);
}

int Tetromino_table::clear_and_count_rows() noexcept {
    int counter = 0;

    Util::for_each_row(blocks_, [this, &counter](auto, auto& row) {
        if (filled(row)) {
            ++counter;
            this->clear_row(row);
        }
    });

    return counter;
}

auto block_exists_lambda() noexcept {
    return [](const auto& block) { return block ? true : false; };
}

bool filled(const Tetromino_table::Row& row) noexcept {
    return std::all_of(row.cbegin(), row.cend(), block_exists_lambda());
}

bool empty(const Tetromino_table::Row& row) noexcept {
    return std::none_of(row.cbegin(), row.cend(), block_exists_lambda());
}

bool touches_table(const Tetromino_table& table,
                   const Tetromino& tetromino,
                   Sdl::Point relative) noexcept {
    return any_of_points(tetromino, [&table, relative](const auto& pos) {
        return table_has_point(table, pos + relative);
    });
}

bool table_has_point(const Tetromino_table& table, Sdl::Point point) noexcept {
    bool found = false;

    Util::for_each(table.blocks(),
                   [point, &found](auto pos, const auto& oc) -> void {
                       if (found || !oc)
                           return;

                       if (pos == point)
                           found = true;
                   });

    return found;
}

bool table_overlaps_tetromino(const Tetromino_table& tbl,
                              const Tetromino& tetromino) noexcept {
    return touches_table(tbl, tetromino, {0, 0});
}

bool overflown(const Tetromino_table& table) noexcept {
    /*
     * Overflow happens if there's a block (or multiple blocks)
     * in the topmost row.
     */
    const auto& top_row = Util::row(table.blocks(), 0);

    return std::any_of(top_row.cbegin(), top_row.cend(),
                       [](const auto& e) { return e ? true : false; });
}

bool on_screen(Sdl::Point point) noexcept {
    /*
     * Casting to an unsigned type is okay here, because we
     * first checked to make sure it's larger than 0.
     */

    return point.x >= 0 && static_cast<std::size_t>(point.x) < table_width;
}

bool on_screen(const Tetromino& tetromino) noexcept {
    return all_of_points(tetromino,
                         [](const auto& pos) { return on_screen(pos); });
}

bool touches_ground(const Tetromino& tetromino) noexcept {
    return any_of_points(
        tetromino, [](const auto& pos) { return pos.y == table_height - 1; });
}

Tetromino_factory::Tetromino_factory() {
    add_block_proto(Tetromino_type::I, Angle::Zero,
                    {{-2, 0}, {-1, 0}, {0, 0}, {1, 0}});

    add_block_proto(Tetromino_type::I, Angle::Ninety,
                    {{0, -2}, {0, -1}, {0, 0}, {0, 1}});

    add_block_proto(Tetromino_type::O, Angle::Zero,
                    {{0, 1}, {1, 0}, {0, 0}, {1, 1}});

    add_block_proto(Tetromino_type::S, Angle::Zero,
                    {{-1, 1}, {0, 1}, {0, 0}, {1, 0}});

    add_block_proto(Tetromino_type::S, Angle::Ninety,
                    {{0, -1}, {0, 0}, {1, 0}, {1, 1}});

    add_block_proto(Tetromino_type::T, Angle::Zero,
                    {{0, 1}, {0, 0}, {-1, 0}, {1, 0}});

    add_block_proto(Tetromino_type::T, Angle::Ninety,
                    {{0, -1}, {-1, 0}, {0, 1}, {0, 0}});

    add_block_proto(Tetromino_type::T, Angle::One_eighty,
                    {{1, 0}, {0, 0}, {-1, 0}, {0, -1}});

    add_block_proto(Tetromino_type::T, Angle::Two_seventy,
                    {{0, -1}, {0, 0}, {0, 1}, {1, 0}});

    add_block_proto(Tetromino_type::Z, Angle::Zero,
                    {{-1, 0}, {0, 0}, {0, 1}, {-1, 1}});

    add_block_proto(Tetromino_type::Z, Angle::Ninety,
                    {{0, 1}, {0, 0}, {1, 0}, {1, -1}});

    add_block_proto(Tetromino_type::J, Angle::Zero,
                    {{-1, 0}, {0, 0}, {1, 0}, {1, 1}});

    add_block_proto(Tetromino_type::J, Angle::Ninety,
                    {{0, 1}, {0, 0}, {0, -1}, {1, -1}});

    add_block_proto(Tetromino_type::J, Angle::One_eighty,
                    {{-1, -1}, {0, 0}, {-1, 0}, {1, 0}});

    add_block_proto(Tetromino_type::J, Angle::Two_seventy,
                    {{-1, 1}, {0, 1}, {0, 0}, {0, -1}});

    add_block_proto(Tetromino_type::L, Angle::Zero,
                    {{-1, 0}, {-1, 1}, {0, 0}, {1, 0}});

    add_block_proto(Tetromino_type::L, Angle::Ninety,
                    {{0, -1}, {0, 0}, {0, 1}, {1, 1}});

    add_block_proto(Tetromino_type::L, Angle::One_eighty,
                    {
                        {-1, 0}, {0, 0}, {1, 0}, {1, -1},
                    });

    add_block_proto(Tetromino_type::L, Angle::Two_seventy,
                    {{-1, -1}, {0, -1}, {0, 0}, {0, 1}});

    two_versions(Tetromino_type::I);
    one_version(Tetromino_type::O);
    two_versions(Tetromino_type::S);
    two_versions(Tetromino_type::Z);
}

Tetromino Tetromino_factory::create(Sdl::Point pos) {
    return {pos, Tetromino_block_offsets{random_blocks()}, random_color()};
}

const Tetromino_block_offsets::Blocks& Tetromino_factory::random_blocks() const
    noexcept {
    auto max = static_cast<int>(Tetromino_type::S);
    std::uniform_int_distribution<int> dist{0, max};

    return block_protos_.at(static_cast<Tetromino_type>(dist(rd_)));
}

SDL_Color Tetromino_factory::random_color() const noexcept {
    std::uniform_int_distribution<std::size_t> dist{0,
                                                    color_protos_.size() - 1};

    return color_protos_[dist(rd_)];
}

void Tetromino_factory::two_versions(Tetromino_type type) {
    auto& blocks = block_protos_[type];
    blocks[Angle::One_eighty] = blocks[Angle::Zero];
    blocks[Angle::Two_seventy] = blocks[Angle::Ninety];
}

void Tetromino_factory::one_version(Tetromino_type type) {
    auto& blocks = block_protos_[type];
    blocks[Angle::Ninety] = blocks[Angle::One_eighty] =
        blocks[Angle::Two_seventy] = blocks[Angle::Zero];
}

void Tetromino_factory::add_block_proto(Tetromino_type type,
                                        Angle ang,
                                        Sdl::Point_vector points) noexcept {
    block_protos_[type][ang] = std::move(points);
}
}  // End Game_logic
