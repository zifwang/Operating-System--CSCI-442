/**
 * Contains the main() routine of what will eventually be your version of top.
 */
#include <getopt.h>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <algorithm>
#include <ncurses.h>
#include <string>
#include "../src/utils/formatting.h"
#include "../src/utils/statistics.h"
#include "../src/utils/flags.h"
#include "../src/utils/sort_function.h"
#include "../src/info/system_info.h" 
#include "../src/info/process_info.h"
 
using namespace std;

/**
 * format help table
 */
void help_table_display();

/**
 * Gets a character from the user, waiting for however many milliseconds that
 * were passed to timeout() below. If the letter entered is q, this method will
 * exit the program.
 */
// press q to exit program
void exit_if_user_presses_q();
// press h to display the help table and exit program
void print_help_if_user_press_h();

/**
 * Function to display table 
 * print table header and table contents
 */
void display_table(SystemInfo& sys);


/**
 * Entry point for the program.
 */
int main(int argc, char **argv) {
  // set flags
  opts_get(argc,argv);
  // ncurses initialization
  initscr();
  // Start color functionality
  start_color();
  // Set black color
  short blc = 0;
  init_color(blc,0,0,0); // set color
  // set color pair
  init_pair(1, COLOR_WHITE, blc);      //white text on a black
  init_pair(2, blc, COLOR_WHITE);      //black text on a white           
  // Don't show a cursor.
  curs_set(FALSE);
  
  // Set getch to return after 1000 milliseconds; this allows the program to
  // immediately respond to user input while not blocking indefinitely.
  timeout(100*keys.delay_time);

  SystemInfo sys_old = get_system_info();
  SystemInfo sys = sys_old;
  sys_old.processes.clear();
  for (CpuInfo& cpu: sys_old.cpus) {
        cpu.idle_time -= keys.delay_time* 10 / sysconf(_SC_CLK_TCK);
  }

      

  while (true) {

    attron(COLOR_PAIR(1));
    // get cpu_percent
    cpu_utilization(sys, sys_old);
    std::sort(sys.processes.begin(), sys.processes.end(),my_sorting);
    wclear(stdscr);
    // Display uptime info
    printw("%s ", format_uptime_info(sys.uptime).c_str());
    // Display load_average info
    printw("%s\n", format_load_averg_info(sys).c_str());
    // Display cpu info
    for (size_t id = 0; id < sys.cpus.size(); id++){
      printw("%s\n", format_cpu_info(sys,sys_old,id).c_str());
    }
    // Display process info
    printw("%s\n", format_process_info(sys).c_str());
    // Display threads info
    printw("%s\n", format_threads_info(sys).c_str());
    // Display memory info
    printw("%s\n\n", format_memory_info(sys).c_str());
    // // Display table
    display_table(sys);

    // Redraw the screen.
    refresh(); 
    // End the loop and exit if Q is pressed
    exit_if_user_presses_q();
    print_help_if_user_press_h();
    // update sys
    sys_old = sys;
    sys = get_system_info();
  }

  // ncurses teardown
  endwin();

  return EXIT_SUCCESS;
}

void display_table(SystemInfo& sys) {
    int width, len, arg_1, arg_2;
    // getmaxyx macros store 
    // the current beginning coordinates 
    // and size of the specified window
    getmaxyx(stdscr, width, len);
    // Color table header
    attron(COLOR_PAIR(2));
    printw("%s\n", format_table_header());
    attroff(COLOR_PAIR(2));
    getyx(stdscr, arg_2, arg_1);
    for (ProcessInfo process: sys.processes) {
        // if file is R state, bold it
        if (process.state == 'R') {
          attron(A_BOLD); 
        }
        printw("%-*.*s", len, len, format_table_entrees(process));
        attroff(A_BOLD);
        arg_2++; // update arg_2
        // check y less then width - 1
        if (arg_2 >= width){
          break;
        }
    }
}

void exit_if_user_presses_q() {
  char c = getch();

  if (c == 'q') {
    endwin();
    exit(EXIT_SUCCESS);
  }
}

void print_help_if_user_press_h() {
  char c = getch();

  if (c == 'h') {
    endwin();
    print_help(0);
  }
}

