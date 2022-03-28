# Assignment 4 - The Game of Life
This program simulates the generation of cells in a universe, and there are three rules, 
 
	1 Any live cell with two or three live neighbors survives.
	2 Any dead cell with exactly three live neighbors becomes alive cell.
	3 All other cells die, either due to loneliness or overcrowding.
 
## Building
First use “make” to build an executable program, and the Makefile is provided in the directory.
## Running
```
$ ./life <-flags> <file address>
```
Below are several options that could be used.

-t: Specify that the Game of Life is to be played on a toroidal universe.

-s: Silence ncurses. Enabling this option means that nothing should be displayed by ncurses.

-n generations: Specify the number of generations that the universe goes through. The default number of generations is 100.

-i input: Specify the input file to read in order to populate the universe. By default, the input should be stdin.
 
 
Example Input:
```
./life -i lists/barberpole.txt
```
Example Output:
ncurses will show 50000 generations of cell universe, then return output likes below in terminal.
```
.....oo
......o
...o.o.
.......
.o.o...
o......
oo.....
 
```
## Cleaning
Use
```
$ make clean
```
which is provided by Makefile.
## Bugs or error
If the input grid is so small that with only 1 * 1 or 1 * 2 or 2 * 2 size, error will occur.
 
 
 

