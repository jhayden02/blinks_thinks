/***********************************************************************************************
*
*   random_manager.hpp - Randomization management for the game engine.
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

#pragma once

#include <raylib.h>
#include <random>
#include <vector>

using std::vector;

namespace engine
{

class random_manager
{
    public:
        random_manager();
        ~random_manager();

        int get_int(int min, int max);
        vector<int> get_int_sequence(size_t count, int min, int max, vector<int> exclude = {});

        Color get_color();
        vector<Color> get_color_sequence(size_t count);

        std::default_random_engine get_generator() { return m_random_generator; }

    private:
        std::default_random_engine m_random_generator;

        inline static const vector<Color> m_bright_colors =
        {
            GOLD, ORANGE, PINK, RED, LIME, SKYBLUE, PURPLE, VIOLET
        };
};

} // NAMESPACE ENGINE.
