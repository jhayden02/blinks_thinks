# ![Blink's Thinks](res/img/title_text.png)

## Index
- [Description](#description)
- [Play Online](#play-online)
- [Mission Statement](#mission-statement)
- [Contribution](#contribution)
- [Building](#building)
- [License](#license)

## Description
Blink's Thinks is a self-hosting point-and-click puzzle game.

It runs on a simple C++ engine that uses raylib + raylib modules at its core. It is currently
compilable for Windows, Linux, and web (Emscripten). The project uses GitHub Pages to host the
web-compiled game in a separate branch, 'gh-pages'. GitHub Workflows re-compiles the game to
this branch on each push.

## Play Online
To play the most recent commit of the game hosted on GitHub Pages, go [here](https://jhayden02.github.io/blinks-thinks/).

## Mission Statement
I got inspired to start this project when I saw some [tsoding](https://github.com/tsoding)
making web assembly games with raylib and hosting them on GitHub pages. I wanted to make
something simple, but with character. This isn't an 'example' project or template code-- this
is my creative work (code, design, music, etc). I hope this project falls somewhere between
complete noob and industry-standard 1000 contributor FOSS project (which seems to be the two
heavy ends of the spectrum on GitHub).

## Contribution
Since this project is such a personal thing to me, I'm not interested in accepting blind pull
requests of feature contributions, such as level design or music. If you have an idea you would
like to see added, I'd be happy to hear your idea over email. Bugfixes are always open to submit.

## Building

See [BUILDING.md](BUILDING.md) for complete build instructions.

## License
Blink's Thinks is licensed under the GNU General Public License v3.0. You should have received
a copy of the license along with this program. If not, see <https://www.gnu.org/licenses>.

This project uses raylib, along with a small snippet of raylib example code for the intro
animation. Both of these are licensed under an unmodified zlib/libpng license, which is an
OSI-certified, BSD-like license that allows static linking with closed source software.
Both are copyright (c) 2014-2025 Ramon Santamaria (@raysan5).
