/**
 * Contains implementations of methods responsible for high-level behavior of
 * your shell.
 *
 * You shouldn't need to add much to this file, but there are a few places that
 * need your attention.
 */

#include "shell.h"
#include <iostream>
#include <cstdlib>
#include <iostream>
#include <readline/history.h>
#include <readline/readline.h>
#include <sstream>

using namespace std;


// Initialize the singleton instance of the Shell class.
Shell Shell::instance;


Shell::Shell() {
  // Tell readline that we want its help managing history.
  using_history();

  // Tell readline that we want to try tab-completion first.
  rl_attempted_completion_function = word_completion;

  // Tell readline that $ should be left attached when performing completions.
  rl_special_prefixes = "$";

  // Register the builtin methods.
  builtins["ls"] = &Shell::com_ls;
  builtins["cd"] = &Shell::com_cd;
  builtins["pwd"] = &Shell::com_pwd;
  builtins["alias"] = &Shell::com_alias;
  builtins["unalias"] = &Shell::com_unalias;
  builtins["echo"] = &Shell::com_echo;
  builtins["exit"] = &Shell::com_exit;
  builtins["history"] = &Shell::com_history;
}


int Shell::loop_and_handle_input() {
  // The return value of the last command executed.
  int return_value = 0;

  while (true) {
    // Get the prompt to show, based on the return value of the last command.
    string prompt = get_prompt(return_value);

    // Read a line of input from the user
    char* line = readline(prompt.c_str());

    // If the pointer is null, then EOF has been received (ctrl-d) and the shell
    // should exit.
    if (!line) {
      cout << endl;
      break;
    }

    // If the command is non-empty, attempt to execute it.
    if (line[0]) {
      return_value = execute_line(line);
    }

    // Free the memory for the input string.
    free(line);
  }

  return return_value;
}


string Shell::get_prompt(int return_value) {
  // TODO: YOUR CODE GOES HERE
  string username = getenv("USER");
  if (getenv("USER") == NULL){
    return (">> Opps, Something Wrong!");
  }else{
    if (return_value != 0){
      return username + " :(" + " Something Wrong Happen! \n" + username + " :( ";
    }else{
      return username + " :)";
    }
  }
  //return " "; // replace with your own code
}


int Shell::execute_line(char* line) {
  // TODO: expand the command from history using !!, !-N, etc
  // HINT: leverage readline! This should only be a couple lines of code.
  // cout << &line << endl; //Debug use
  char *expansion;  //Create output of history_expand function
  int result;

  result = history_expand (line, &expansion);
  // cout << result << endl;
  if (result){
    // fprintf (stderr, "%s\n", expansion);
    cout << expansion << endl;
  }

  if (result < 0 || result == 2){
    free (expansion);
    perror("Error: History Expansion Does Not Work");
    //continue;
  }

  // TODO: save the command to history (again, leverage readline!)
  add_history(expansion);

  // Tokenize the input string.
  vector<string> tokens = tokenize_input(expansion);

  // Handle local variable declarations.
  local_variable_assignment(tokens);

  // Handle the use of an alias.
  alias_substitution(tokens);

  // Substitute variable references.
  variable_substitution(tokens);

  // Execute the command.
  return dispatch_command(tokens);
}


vector<string> Shell::tokenize_input(char* line) {
  vector<string> tokens;
  istringstream token_stream(line);
  string token;

  while (token_stream >> token) {
    tokens.push_back(token);
  }

  // Search for quotation marks, which are explicitly disallowed.
  for (size_t i = 0; i < tokens.size(); i++) {
    if (tokens[i].find_first_of("\"'`") != string::npos) {
      cerr << "\", ', and ` characters are not allowed." << endl;

      tokens.clear();
    }
  }

  return tokens;
}


void Shell::local_variable_assignment(vector<string>& tokens) {
  vector<string>::iterator token = tokens.begin();

  while (token != tokens.end()) {
    string::size_type eq_pos = token->find("=");

    // Stop at the first token not in the form: key=value.
    if (eq_pos == string::npos) {
      break;
    }

    string key = token->substr(0, eq_pos);
    string value = token->substr(eq_pos + 1);
    localvars[key] = value;

    // Erase the token and advance to the next one.
    token = tokens.erase(token);
  }
}


void Shell::alias_substitution(vector<string>& tokens) {
  // TODO: implement alias handling
  map<string,string>::iterator it;
  it = aliases.find(tokens[0]);
  if(it != aliases.end()){
    tokens[0] = it->second;
  }
}


void Shell::variable_substitution(vector<string>& tokens) {
  vector<string>::iterator token;

  for (token = tokens.begin(); token != tokens.end(); ) {
    if (token->at(0) == '$') {
      string var_name = token->substr(1);

      if (getenv(var_name.c_str()) != NULL) {
        *token = getenv(var_name.c_str());
      } else if (localvars.find(var_name) != localvars.end()) {
        *token = localvars.find(var_name)->second;
      } else {
        token = tokens.erase(token);
        continue;
      }
    }

    ++token;
  }
}


int Shell::dispatch_command(vector<string>& argv) {
  int return_value = 0;

  if (argv.size() != 0) {
    map<string, builtin_t>::iterator cmd = builtins.find(argv[0]);

    if (cmd == builtins.end()) {
      return_value = execute_external_command(argv);
    } else {
      return_value = ((this->*cmd->second)(argv));
    }
  }

  return return_value;
}
