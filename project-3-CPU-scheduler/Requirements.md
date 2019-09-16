# Project 3 - CPU Scheduling Simulator

For this project, you'll implement a next-event simulation capable of simulating
CPU scheduling for a set of different algorithms.


## Building and running the program

To build the main program (called 'simulator' by default), type:

`make`

To both build and run the program with the example simulation file and no
command line flags, you can use the following shortcut:

`make run`

If you want to run your program with flags, you can be explicit:

`./simulator --verbose --per_thread example_simulation`

or short-form:

`./simulator -vt example_simulation`


## Coding on a Mac

You can build this program fully on a Mac, but ensure your program compiles and
runs on Alamode machines (without warnings) prior to submitting it.

You might need to install Boost. I did it using HomeBrew via:

`brew install boost`

You can also choose to rewrite the output methods (in src/util/logger.cpp) to
not use Boost. That's a thing. ...that some people might do. Because... why not!
