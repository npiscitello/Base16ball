#base16ball
*Teaching binary conversions one curveball at a time!*

It's the bottom of the ninth, 2 outs, and you step onto the plate. You are the Computer Science department's last hope to win the championship! You steady yourself for the pitch, and here it comes. But wait, what's this? Is that - hex printed on the ball? This is no ordinary baseball game! Convert hexadecimal and octal to binary (or vice versa) to hit the ball and win the game for your team!

This game is a final project for Wentworth Institute of Technology course ELEC3150, taught by Dr. Marpaung.

###Table of Contents
1. [Specifications and Scope](#specscope)
2. [Implementation](#implement)
3. [Installation](#install)
4. [Execution](#exec)
5. [Gameplay](#docs)

<div id='specscope'/>
##Specifications and Scope
This game is a baseball-themed way to exercise your binary, oct, and hex conversion skills. Each `ball` will have a number and a format attached to it - your job is to convert the number to the requested format. If you do this fast enough, you'll hit the ball! If you get the right answer but take a little too long, the pitcher threw a ball. If you input the wrong answer or don't answer at all, you'll get a strike!
###Requirements (this game will...):
* be text based, with all input and output occuring on a terminal
* compile with no warnings or errors on Wentworth's Turing system
* entertain the user for at least one hour
* exhibit advanced programming techniques in its source, including but not limited to:
  * data structures (`struct`)
  * linked lists
  * inheritance
  * polymorphism
* be fully documented such that a user will be familiar with all features of the game after reading
  the documentation
* have a 'save' feature
* present hex -> bin, oct -> bin, bin -> oct, and bin -> hex 'balls' in any user-selected combination
* animate the 'ball' being pitched
* have provisions for hits, balls, or strikes

###Close Goals (this game should...):
* animate the batting action
* have additional provisions for a single, double, triple, or home run

###Far Goals (this game could...):
* have both batting and fielding play modes
* display the baseball diamond with runners on base

<div id='implement'/>
##Implementation
###Class Headers
* `include/ball.h` - manages interactive gameplay state, generating and storing the questions and answers and related data
* `include/umpire.h` - generates, stores, and manages instances of the `ball` class. Checks users' answers against the stored correct answer.
* `include/scoreboard.h` - takes care of clerical aspects of the game, handling score and strike state and managing the protobuf message containing the leaderboard
* `proto/leaderboard.proto` - This 'class' is actually a [Google protocol buffer message](https://developers.google.com/protocol-buffers/). It handles persistant storage and serialization and stores the score during the game.

###Source Files
* `src/main.cc` - this is where the magic happens. The `main()` function lives here, coordinating all the above classes to provide stable game mechanics while using the `ncurses` library to display a captivating gameplay interface.
* `src/scorereader.cc` - this is a utility to view the leaderboard. It began life as a development tool but became useful as a companion app to the game.
* `src/ball_testapp.cc` - used during development to verify the functionality of the `ball` class, it generates and displays many different types of 'balls'
* `src/umpire_testapp.cc` - very similar to the `ball` testapp; spins up instances of the `umpire` class and displays the result.

<div id='install'/>
##Installation
This game relies on the `protobuf` library [from Google](https://github.com/google/protobuf) and [the ncurses library](https://www.gnu.org/software/ncurses/). Both are widely available in the package management systems of many distributions (for example, `sudo pacman -S protobuf ncurses` on Arch) or as buildable source downloads. Specific dependencies are as follows:

* Google's protobuf compiler, `protoc` (to compile `.proto` files to `.pb.cc` and `.pb.h` files for use with C++ source)
* Google's protobuf runtime library, `libprotobuf.so`
* Google's protobuf headers (in `/usr/include/google/protobuf/` by default on Arch)
* core and menu ncurses runtime libraries (`libncurses.so`, `libmenu.so`)
* core and menu ncurses headers (`ncurses.h` and `menu.h`, both in `/usr/include` by default on Arch)

**NOTE:** *These libraries and headers are not installed in the default locations on WIT's Turing server! Instead, they have been built and installed to `/home/piscitellon/custom_builds/install`. The absolute paths for the libraries and includes have been placed in the Makefile so this game should automatically build anywhere on the Turing system.*

**NOTE:** *The protocol buffer compiler, `protoc`, is not installed in the system binary folder of WIT's Turing server! It is instead installed in `/home/piscitellon/custom_builds/install/bin`. The absolute path has been placed in the Makefile so this game should automatically build anywhere on the Turing system.*

On systems that meet the dependency requirements, installation should be as simple as cloning in the game source and running `make` (to compile only the game binary) or `make all` (to compile the game and supporting binaries, including `scorereader` and testapps) in the `base16ball` directory. On the Turing server at WIT, special steps may be required; see the above notes. If the build completed successfully, you should see this somewhere in the ouput:

	==========================
	 Game essentials compiled
	==========================

<div id='exec'>
##Execution
**NOTE:** *The required runtime libraries are not automatically in the system search path on WIT's Turing server! They are located in `/home/piscitellon/custom_builds/install/lib`. Be sure to add that path to your `LD_LIBRARY_PATH` before executing the game.*

Following a successful build, you should see a `bin` folder in the root directory of the repository containing `main` (and possibly other executables) and a `base16ball` file in the root symlinked to `bin/main`. That's the game - run `./base16ball` to start your baseball career! Additional steps may be neccessary on WIT's Turing server, see the note above.

<div id='docs'>
##Gameplay