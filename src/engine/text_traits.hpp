/***********************************************************************************************
*
*   text_traits.hpp - The library for adding traits to existing text objects.
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

// Raylib.
#include "raylib.h"

namespace engine
{
// Forward declaration.
class text;

class text_trait
{
    public:
        virtual ~text_trait() = default;
        virtual void update(text& txt) = 0;
};

class rotation_sin : public text_trait
{
    public:
        rotation_sin(float speed, float depth);
        void update(text& txt) override;

    private:
        float m_speed;
        float m_depth;
};

} // NAMESPACE ENGINE.
