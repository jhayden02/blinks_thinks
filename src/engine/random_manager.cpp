/***********************************************************************************************
*
*   random_manager.cpp - The library for the random manager module of the game object.
*
*   Copyright (c) 2026 Josh Hayden (@jhayden02)
*
*   Blink's Thinks is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License v3.0 as published
*   by the Free Software Foundation.
*
*   Blink's Thinks is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program.  If not, see <https://www.gnu.org/licenses/>.
*
***********************************************************************************************/

// Source.
#include "game.hpp"
#include "random_manager.hpp"

// Standard library.
#include <algorithm>
#include <unordered_set>

using engine::random_manager;

random_manager::random_manager()
{
    std::random_device random_generator_seed;
    m_random_generator.seed(random_generator_seed());
}

random_manager::~random_manager()
{
}

int random_manager::get_int(int min, int max)
{
    GAME_ASSERT(max - min > 0, "Invalid range supplied.");
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(m_random_generator);
}

vector<int> random_manager::get_int_sequence(size_t count, int min, int max, vector<int> exclude)
{
    GAME_ASSERT(
        [&]{
            const size_t range_size = max - min + 1;
            return count <= range_size;
        }(),
        "Requested more unique numbers than available range."
    );

    std::unordered_set<int> exclusion_set(exclude.begin(), exclude.end());

    vector<int> pool;
    for (int num = min; num <= max; ++num) {
        if (exclusion_set.find(num) == exclusion_set.end()) {
            pool.push_back(num);
        }
    }

    std::shuffle(pool.begin(), pool.end(), m_random_generator);
    return {pool.begin(), pool.begin() + static_cast<long>(count)};
}

Color random_manager::get_color()
{
    return m_bright_colors.at(get_int(0, m_bright_colors.size() - 1));
}

vector<Color> random_manager::get_color_sequence(size_t count)
{
    GAME_ASSERT(count <= m_bright_colors.size() ,"Requested more unique colors than available maximum.");

    // Copy game defined constant vector, shuffle it, then return the count asked for.
    vector<Color> palate = m_bright_colors;
    std::shuffle(palate.begin(), palate.end(), m_random_generator);
    return {palate.begin(), palate.begin() + static_cast<long>(count)};
}
