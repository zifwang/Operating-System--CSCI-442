#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include "../src/utils/flags.h"

void print_help(int exit_num) {
    printf("Help for Interactive Commands:\n"
           "options            descriptions\n"
           "-h --help          to print the help table\n"
           "-d --delay         to delay between updates\n"
           "-s --sort-key      to sort by PID, CPU, MEM or TIME\n"
        );
    exit(exit_num);
}

// keys declear here
struct sort_options keys;

void opts_get(int argc, char **argv) {
    // An array of long-form options
    static struct option long_options[] = {
        // Because the third argument is NULL for these flags, getopt_long will
        // return the fourth argument when these are encountered. If the long-form
        // flag corresponds to a short-form flag, use the short-form character and
        // you can handle both cases (long and short) the same way.
        {"help", no_argument, NULL, 'h'},
        {"delay", required_argument, NULL, 'd'},
        {"sort-key", required_argument, NULL, 's'},

        // Terminate the long_options array with an object containing all zeroes.
        {0, 0, 0, 0}
    };
    
    // struct keys default
    keys.delay_time = 10;
    keys.sortKeys = CPU; 
    
    // getopt_long stores the latest option index here,you can get the flag's
    // long-form name by using something like long_options[option_index].name
    int option_index = 0;
    
    // Process the next command-line flag. the return value here is the
    // character or integer specified by the short / long options.
    int flag_char = getopt_long(
        argc,           // The total number of arguments passed to the binary
        argv,           // The arguments passed to the binary
        "d:s:m:h",      // Short-form flag options
        long_options,   // Long-form flag options
        &option_index); // The index of the latest long-form flag
    
    if(flag_char != -1) {
        switch (flag_char) {
            //printf("%s",optarg);
            case 0:
                break;
            case 'd':
                sscanf(optarg, "%d", &keys.delay_time);
                break;
            case 's':
                // Compare cpu
                if (!strcmp(optarg, "CPU")) {
                    keys.sortKeys = CPU;
                    break;
                }
                // Compare PID
                if (!strcmp(optarg, "PID")) {
                    keys.sortKeys = PID;
                    break;
                }
                // Compare MEM
                if (!strcmp(optarg, "MEM")) {
                    keys.sortKeys = MEM;
                    break;
                }
                // Compare TIme
                if (!strcmp(optarg, "TIME")) {
                    keys.sortKeys = TIME;
                    break;
                }
                //perror("Cannot find sorting keys!\n");
                printf("Cannot find sorting keys!\n");
                print_help(2);
            case 'h':
                print_help(0);
            default:
                printf("Option unknown\n");
                print_help(2);
        }
   }
}
