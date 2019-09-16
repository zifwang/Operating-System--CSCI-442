/**
 * This file contains the definition of the Shell class, which is responsible
 * for all of the functionality of your shell. I did my best to document all the
 * methods and members, so spend some time reading the comments.
 *
 * The implementation of this class is split into several files:
 *  - shell_core.cpp:
 *      Already mostly implemented; needs only a few additions. You can also
 *      add any helper functions that have no better place here.
 *
 *  - shell_builtins.cpp (deliverable 1):
 *      Contains implementations of your shell's builtins.
 *
 *  - shell_tab_completion.cpp (deliverable 2):
 *      Contains implementations of tab completion methods. I've implemented the
 *      readline callbacks, which delegate to a pair of methods that you will
 *      need to implement.
 *
 *  - shell_cmd_execution.cpp (deliverable 3):
 *      Contains implementaions related to executing external commands. My
 *      implementation had several additional helper methods in this file. Much
 *      of the code you write will be here.
 *
 * You're free to add, change, remove, or reorganize code however you want, so
 * long as you implement the required functionality.
 */

#pragma once
#undef _GNU_SOURCE
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include "command.h"

/**
 * An external reference to the execution environment (ENV vars). For more info,
 * run 'man environ' in a terminal.
 */
extern char** environ;


class Shell {
// Public API (shell_core.cpp)
public:

  /**
   * Returns the singleton instance of this class.
   */
  static Shell& getInstance() { return instance; }

  /**
   * Accepts and executes input in a loop. Type 'exit' or press CTRL-D to exit.
   *
   * @return The return value of the last command
   */
  int loop_and_handle_input();

// Constructor (shell_core.cpp)
private:

  /**
   * Private constructor to enforce use of static getInstance() method.
   */
  Shell();

  /**
   * Disallow copy and assignment (You know: singleton instance, and all...).
   */
  Shell(const Shell&);
  void operator =(const Shell&);

// SHELL UTILITY FUNCTIONS (shell_core.cpp)
private:

  /**
   * Returns an appropriate prompt to use for return_value, the return value of
   * the previous command.
   *
   * @param return_value The return value of the previous command
   * @return The prompt to display to the user
   */
  std::string get_prompt(int return_value);

  /**
   * Attempts to parse and execute the given line.
   *
   * @param line The command entered by the user
   * @return The return value from running the command, as an integer
   */
  int execute_line(char* line);

  /**
   * Tokenizes the user input (splits it into strings on whitespace).
   *
   * @param line The string to tokenize
   * @return The resulting tokens (argv)
   */
  std::vector<std::string> tokenize_input(char* line);

  /**
   * Examines each token and sets an env variable for any that are in the form
   * of key=value. Stops at the first token not in the form of key=value.
   *
   * @param argv The vector of arguments
   */
  void local_variable_assignment(std::vector<std::string>& argv);

  /**
   * Replaces any command token that matches an alias with that alias' value.
   *
   * @param argv The vector of arguments
   */
  void alias_substitution(std::vector<std::string>& argv);

  /**
   * Substitutes any tokens that start with a '$' with their appropriate value,
   * or erases the token if there is no corresponding variable.
   *
   * @param argv The vector of arguments
   */
  void variable_substitution(std::vector<std::string>& argv);

  /**
   * Executes a line of input by either calling execute_external_command or
   * directly invoking the built-in command.
   *
   * @param argv The vector of arguments
   * @return The return code of the operation
   */
  int dispatch_command(std::vector<std::string>& argv);

// BUILTINS (shell_builtins.cpp)
private:

  /**
   * Lists all the files in the specified directory (argv[1]). If no argument is
   * given, the current working directory is used instead.
   *
   * @param argv The vector of arguments
   * @return The return code of the operation
   */
  int com_ls(std::vector<std::string>& argv);


  /**
   * Changes the current working directory to the specified directory (argv[1]),
   * or to the user's home directory if no argument is provided.
   *
   * @param argv The vector of arguments
   * @return The return code of the operation
   */
  int com_cd(std::vector<std::string>& argv);


  /**
   * Displays the current working directory.
   *
   * @param argv The vector of arguments
   * @return The return code of the operation
   */
  int com_pwd(std::vector<std::string>& argv);


  /**
   * If called without an argument, then any existing aliases are displayed.
   * Otherwise, the argument is assumed to be a new alias and an entry is made
   * in the alias map.
   *
   * @param argv The vector of arguments
   * @return The return code of the operation
   */
  int com_alias(std::vector<std::string>& argv);


  /**
   * Removes aliases. If "-a" is provided as an argument, then all existing
   * aliases are removed. Otherwise, the argument is assumed to be a specific
   * alias to remove and if it exists, that alias is deleted.
   *
   * @param argv The vector of arguments
   * @return The return code of the operation
   */
  int com_unalias(std::vector<std::string>& argv);


  /**
   * Prints all arguments to the terminal.
   *
   * @param argv The vector of arguments
   * @return The return code of the operation
   */
  int com_echo(std::vector<std::string>& argv);


  /**
   * Displays all previously entered commands, as well as their associated line
   * numbers in history.
   *
   * @param argv The vector of arguments
   * @return The return code of the operation
   */
  int com_history(std::vector<std::string>& argv);


  /**
   * Exits the program.
   *
   * @param argv The vector of arguments
   * @return The return code of the operation
   */
  int com_exit(std::vector<std::string>& argv);

// TAB COMPLETION (shell_tab_completion.cpp)
private:

  /**
   * Populates the given matches vector with all the ENV and local variables
   * that match the given text.
   *
   * @param text The text against which to match
   * @param matches The vector to fill with matching variables
   */
  void get_env_completions(const char* text, std::vector<std::string>& matches);

  /**
   * Populates the given matches vector with all the 1) builtin functions,
   * 2) aliases, and 3) executable files on the user's $PATH that match the
   * given text.
   *
   * @param text The text against which to match
   * @param matches The vector to fill with matching variables
   */
  void get_command_completions(
      const char* text,
      std::vector<std::string>& matches);

  /**
   * This is the function we registered as rl_attempted_completion_function. It
   * attempts to complete with a command, variable name, or filename.
   *
   * @param text The string for which to suggest completion values
   * @param start The start index of text within rl_line_buffer
   * @param end The end index of text within rl_line_buffer
   * @return An array of matching strings
   */
  static char** word_completion(const char* text, int start, int end);

  /**
   * Generates environment variables for readline completion. This function will
   * be called multiple times by readline and will return a single cstring each
   * time. Delegates the actual completion logic to get_env_completions, which
   * is an instance method and gets called only once per completion attempt.
   *
   * @param text The text entered by the user
   * @param state 0 the first time this function is called; otherwise, non-0
   * @return A single matching environment variable
   */
   static char* env_completion_generator(const char* text, int state);

  /**
   * Generates commands for readline completion. This function will be called
   * multiple times by readline and will return a single cstring each time.
   * Delegates the actual completion logic to get_command_completions, which
   * is an instance method and gets called only once per completion attempt.
   *
   * @param text The text entered by the user
   * @param state 0 the first time this function is called; otherwise, non-0
   * @return A single matching command
   */
  static char* command_completion_generator(const char* text, int state);

  /**
   * Pops the last value off the given vector and returns the result.
   *
   * @param matches The list of matches
   * @return The last value in matches, or NULL if empty
   */
  static char* pop_match(std::vector<std::string>& matches);

// EXTERNAL COMMAND EXECUTION (shell_cmd_execution.cpp)
private:

  /**
   * Executes an external command (rather than a builtin). You will need to
   * fork() off a new process and use one of the exec() functions to execute the
   * external program. If the command contains pipes, you will have to fork()
   * and exec() multiple times. Finally, you must handle pipes and redirects as
   * by redirecting the I/O as required.
   *
   * @param argv The vector of arguments
   * @return The return code of the operation
   */

  int execute_external_command(std::vector<std::string>& argv);
  /**
   * partition_tokens implementation on the resulting tokens, and prints the resulting commands
   */
  // bool partition_tokens(std::vector<std::string> argv, std::vector<command_t>& commands);

  
// CONSTANTS AND MEMBERS (shell_core.cpp)
private:

  // Define 'builtin_t' as a type for built-in functions.
  typedef int (Shell::*builtin_t)(std::vector<std::string>&);

  /**
   * The singleton instance of this class. Readline's callbacks must be regular
   * (non-instance) methods, so store an instance for easy access. Forgive me!
   */
  static Shell instance;

  /**
   * A mapping of internal commands to their corresponding functions.
   */
  std::map<std::string, builtin_t> builtins;

  /**
   * A mapping of variables (local to the shell) and their corresponding values.
   */
  std::map<std::string, std::string> localvars;

  /**
   * A mapping of aliases and their corresponding values.
   */
  std::map<std::string, std::string> aliases;
};
