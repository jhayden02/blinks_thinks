/***********************************************************************************************
*
*   background.hpp - The library for creating and drawing backgrounds.
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

#pragma once

// Source.
#include "entity.hpp"

namespace engine
{

class background : public entity
{
    public:
    background(
        Color dark_color,
        Color light_color,
        int square_size
    );
    ~background();

    void update() override;
    void draw() override;

    static float get_scroll_offset() { return m_scroll_offset; }
    static void set_scroll_offset(float scroll_offset) { m_scroll_offset = scroll_offset; }

    private:
    Color m_dark_color;
    Color m_light_color;
    int m_square_size;

    static float m_scroll_offset;
};

} // NAMESPACE ENGINE.
