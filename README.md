#base16ball
*Teaching binary conversions one curveball at a time!*

It's the bottom of the ninth, 2 outs, and you step onto the plate. You are the Computer Science 
department's last hope to win the championship! You steady yourself for the pitch, and here it 
comes. But wait, what's this? Is that - hex printed on the ball? This is no ordinary 
baseball game! Convert hexadecimal and octal to binary (or vice versa) to hit the ball and win the 
game for your team!

This game is a final project for Wentworth Institute of Technology cource ELEC3150, taught by
Dr. Marpaung.

##Specifications and Scope
This game is a baseball-themed way to exercise your binary, oct, and hex conversion skills. Each
'ball' will have a number and a format on it - your job is to convert the number to the requested
format. If you do this fast enough, you'll hit the ball! If you get the right answer but take a
little too long, the pitcher threw a ball. If you input the wrong answer or don't answer at all,
you'll get a strike! A 'ball' might look something like the examples shown below:
```
  ______     ______
 /      \   /      \
|  0xFF  | | 110101 |
| binary | |   oct  |
 \______/   \______/
```
###Requirements (this game will...):
* be text based, with all input and output occuring on a terminal
* compile with no warnings or errors on Wentworth's Turing system
* entertain the user for at least one hour
* exhibit advanced programming techniques in its source, including but not limited to:
  * data structures (`struct`s)
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

##Implementation
There will be a 'ball' and an 'umpire' class. The 'ball' class will create objects of the 'ball'
containing data such as the text to display, the class of problem (hex -> bin, etc.), and the
answer. The 'umpire' class will be in charge of generating balls and judging input. There's also
going to be a 'manager' class as a wrapper; it will handle play/pause, save, and statistics storage.
