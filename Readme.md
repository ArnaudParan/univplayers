# UnivPlayers

Simplified version of playerctl for linux because I couldn't be bothered with the fact
that they don't support player shuffling, and their code relies so extensively on the Glib
which is cool for portability but that I don't find great for concise code and readability.

It's just the beginning of the project, so I think that I have various memory leaks that I
will solve in the future with valgrind.

An I will add tests with pytest when I get some time

It should be pretty straightforward to build, now the only dependency is gio in the glib,
if you can't compile check the Makefile out, I added the include files as they are on my computer
they might be different for you. I will make a nice cmake build stack when I get some time
and I also would like to get my code to be compatible for all platforms, I don't think it is
the case now. I use archlinux, and made a pacman package for that code, might do a debian
one when I get the time.
