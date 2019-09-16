/**
 * This file contains the main() function, which simply delegates to an instance
 * of the Shell class (see shell.h for documentation).
 *
 * After reading the shell.h documentation, search for TODO's to get an idea of
 * the code you'll need to add.
 */

#include "shell.h"


/**
 * The entry point to the shell.
 */
int main() {
  return Shell::getInstance().loop_and_handle_input();
}
