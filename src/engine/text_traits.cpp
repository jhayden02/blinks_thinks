/***********************************************************************************************
*
*   text_traits.cpp - The library for adding traits to existing text objects.
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
#include "text_traits.hpp"
#include "game.hpp"

using engine::game;
using engine::text_trait;
using engine::rotation_sin;

rotation_sin::rotation_sin(float speed, float depth)
    :
    m_speed(speed),
    m_depth(depth)
{}

void rotation_sin::update(text& txt)
{
    txt.set_rotation(sin(GetTime() * m_speed) * m_depth);
}
