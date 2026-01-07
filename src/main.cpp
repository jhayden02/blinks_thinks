/***********************************************************************************************
*
*   main.cpp - The entry point and program runtime.
*
*   Copyright (c) 2025 Josh Hayden (@BlinkDynamo)
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
#include "game_levels.hpp"

// Standard library.
#include <cmath>

int main(void)
{
    engine::game& game_inst = engine::game::get_instance();
    game_inst.set_next_level(new intro_raylib());
    game_inst.run();
    return 0;
}
