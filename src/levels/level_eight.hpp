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

class level_eight : public engine::scene
{
    public:
        level_eight();
        void update() override;

    private:
        static constexpr int m_choice_count = 6, m_fib_seq_len = 5;

        // Will return a fibbonacci sequence of length 'length', started by 'num_one' + 'num_two',
        // exiting if the returned length results in a number larger than an int can store.
        vector<int> get_fib_seq(int num_one, int num_two);

        button* m_correct_button;
};
