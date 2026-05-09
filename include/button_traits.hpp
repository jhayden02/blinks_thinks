/***********************************************************************************************

*   button_traits.hpp - The library for adding traits to existing button objects.
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
class button;

class button_trait
{
    public:
        virtual ~button_trait() = default;
        virtual void update(button& btn) = 0;
};

class sfx_press : public button_trait
{
    public:
        sfx_press(Sound *snd);
        void update(button& btn) override;

    private:
        Sound *m_sound;
};

class grows_when_hovered : public button_trait
{
    public:
        grows_when_hovered(int frame_duration = 10, float target_scale = 1.2f);
        void update(button& btn) override;

        void set_frame_duration(int frame_duration) { m_frame_duration = frame_duration; }
        void set_target_scale(float target_scale) { m_target_scale = target_scale; }

    private:
        int m_frame_duration;
        float m_current_scale;
        float m_target_scale;
        float m_default_scale;
};

class grabbable : public button_trait
{
    public:
        grabbable(Sound *sfx_hold = nullptr, Sound *sfx_release = nullptr);
        void update(button& btn) override;

    private:
        Sound *m_sfx_hold;
        Sound *m_sfx_release;
        bool m_is_grabbed;
        Vector2 m_grab_offset;
};

} // NAMESPACE ENGINE.
