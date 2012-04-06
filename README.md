This is an implementation of TeX in modern C++ written by Duane R. Bailey. However, the main purpose of the project at the moment is not the program itself, but rather an academic study of software engineering.

The project is organized into several "libraries", which are not meant to be loaded independently of each other (though some can be) but rather represent an originizational decision of mine. The code is designed to be modular: for example, the inner code fully supports working with utf-32. However, due to the constraints of time, most of this functionality is currently for nothing as I am focusing on implementing the semblance of a TeX program so I can learn how to write tests, documentation, maintainable code, etc. As a result, any non-ascii character is flagged as invalid, the program only accepts TFM files, only outputs DVI files, and currently barely works where it works at all.

Here is what is currently implemented:

+ Fairly complete non-math typesetting, with ligatures and kerning support.

+ Correct line breaking without hyphenation.

+ Simple page breaking.


However, I am hopefully laying the base for code to be written in the future, so that when I have time to devote to the project beyond my academic commitments, features can progress rapidly.

Happy TeXing!

Duane Bailey

<bailey.d.r@gmail.com>