/***********************************************************************************************
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

// Source.
#include "scene.hpp"

using engine::scene;
using engine::button;

class level_title : public engine::scene
{
    public:
        level_title();
        void update() override;

    private:

        class game_title_text
        {
            public:
                text* text_obj;

                static constexpr float initial_rotation = 0.0;
                static constexpr float rotation_speed = 5.0;
                static constexpr float rotation_depth = 2.5;

                static constexpr Color text_color = { 75, 255, 205, 255 };
        };
        game_title_text m_game_title_text;

        button* m_play_button;
};
