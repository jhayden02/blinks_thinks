/***********************************************************************************************
*
*   entity.cpp - The base class for all game entities.
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
#include "entity.hpp"

using engine::entity;

entity::entity(Vector2 position, int layer, Vector2 speed)
    :
    m_position(position),
    m_layer(layer),
    m_speed(speed)
{}

void entity::update()
{
    // update the position of the entity according to the movement speed.
    m_position = {m_position.x + m_speed.x, m_position.y + m_speed.y};
}
