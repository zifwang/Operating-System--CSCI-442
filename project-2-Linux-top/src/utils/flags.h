//Code for defining and parsing command-line flags.
#pragma once

// define sorting options
enum opts {PID, CPU, MEM, TIME};

// build a sort struct 
struct sort_options {
    int delay_time;
    enum opts sortKeys;
};
extern struct sort_options keys;

// print help table
void print_help(int exit_num);

// get input from users
void opts_get(int argc, char **argv);