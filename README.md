# Bingo Book Generator

A simple Bingo Game Book Generator that creates printable Bingo cards using the libharu library. This tool generates a PDF containing a Bingo card (or multiple cards) that can be used for Bingo games.

# How to play

The game is played by two or more players, each having an individual Bingo card, which is a 10x10 grid with randomly placed numbers from 1 to 100.

The game proceeds as follows:

    Starting the game: The first player calls out a number between 1 and 100. All players then check their cards and mark the number.

    Taking turns: The second player calls out a number, and the process repeats, with each player taking turns to announce a number. After a number is called, all players mark it on their cards.

    Winning the game: The first player to complete 5 rows and 5 columns of marked numbers on their card wins.

# Requirements and supporting platforms

I'm a bit lazy, so in this repository, you'll only find a solution file with project files for Visual Studio. My setup specifications are as follows:
    Visual Studio Community 2022 (64-bit), version 17.12.0
    Windows 10 Pro 22H2
    C++ 20

If you're a Linux user, I’ve handled the Windows-specific parts in the laziest way possible. Instead of using a file save dialog, the window where you choose where to save the PDF is replaced by manually entering the file path and name (e.g., somepath/output.pdf).

As for the PDF library I use, you can find the link to it in the Acknowledgments section below, along with a ready CMake and installation guide to help you build it.

If anyone wants to make the project cross-platform and provide new Requirements and Build Instructions, feel free to contact me, and I'll make you a contributor to the repository.

# Acknowledgment

All of the code in the PDF_Support folder is not mine and I didn't wrote it.
The source of it is: https://github.com/libharu/libharu/tree/master
