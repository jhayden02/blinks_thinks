# ![Blink's Thinks](res/img/title_text.png)

### A self-hosting point-and-click puzzle game.

Blink's Thinks is a simple C++ video game that uses a small custom raylib engine. It is
currently compilable on Windows and Linux, with web compilation (Emscripten) being possible
on both. The project uses GitHub Pages to host the web-compiled game in a separate branch,
[gh-pages](https://github.com/jhayden02/blinks-thinks/tree/gh-pages). GitHub Workflows
re-compiles the game to this branch on each push in the event source files are changed.

To play the most recent commit of the game hosted on GitHub Pages, go [here](https://jhayden02.github.io/blinks-thinks/).

I was inspired to start this project when I saw [tsoding](https://github.com/tsoding)
making web assembly games with raylib and hosting them on GitHub pages. I wanted to make
something simple, but with character. 

Since this project is such a personal thing to me, I'm not interested in accepting blind pull
requests of feature contributions, such as level design or music. If you have an idea you would
like to see added, I'd be happy to hear your idea over email. Bugfixes are always open to submit.

See [BUILDING.md](BUILDING.md) for complete instructions on how to build the game from source.

### License
Blink's Thinks is licensed under the GNU General Public License v3.0. You may read the full copy
of the license [here](LICENSE). 

This project uses raylib, along with a small snippet of raylib example code for the intro
animation. Raylib is licensed under an unmodified zlib/libpng license, which is an OSI-certified,
BSD-like license that allows static linking with closed source software. You should have recieved
a copy of that license inside the raylib submodule [here](external/raylib/LICENSE). 
