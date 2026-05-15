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

class level_10 : public engine::scene
{
    public:
        level_10();
        void update() override;

    private:
        button* m_submit_box;
        button* m_holdable_number;
        button* m_submit_button;
        static constexpr int m_choice_count = 5;
        static constexpr int m_min_choice = 1;
        static constexpr int m_max_choice = 9;
        static constexpr int m_max_submit_box_numbers = 2;
        vector<button*> m_correct_button_layout;
};
