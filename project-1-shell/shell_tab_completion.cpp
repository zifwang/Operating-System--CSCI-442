/**
 * This file contains implementations of the functions that provide
 * tab-completion for the shell.
 *
 * You will need to finish the implementations of the completion functions,
 * though you're spared from implementing the high-level readline callbacks with
 * their weird static variables...
 */

#include "shell.h"
#include <cstdlib>
#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>
#ifdef WINDOWS
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif
#include <dirent.h>    //use to load file name from directory
#include <stdio.h>     /* defines FILENAME_MAX */ //use to load file name from directory
#include <sys/types.h> //use to load file name from directory
#include <vector>
#include <string>
#include <unistd.h> // use in cd command
using namespace std;

//extern char** environ;
void Shell::get_env_completions(const char* text, vector<string>& matches) {
  string text_string = text;
  text_string = text_string.substr(1,text_string.length()-1);
  // Compare with environ
  // if not same add to matches
  for(char **environment = environ; *environment != 0; environment++){
    string env = *environment;
    string::size_type position = env.find("=");
    string tem_sub_str = env.substr(0,text_string.length());
    if(text_string.compare(tem_sub_str) == 0){
      matches.push_back("$"+ env.substr(0,position));
    }
  }
  // Compare with local variable
  // if not same add to matches
  std::map<std::string, std::string>::iterator it;
  it = localvars.begin();
  while(it != localvars.end()){
    string temp_str = it->first;
    string temp_sub_str = temp_str.substr(0,text_string.length());
    if(text_string.compare(temp_sub_str) == 0){
      matches.push_back("$"+ temp_str);
    }
    it++;
  }
}

// This Function probably needed to be modify later
void Shell::get_command_completions(const char* text, vector<string>& matches) {
  // Add Built-in Command to mathces vector
  std::map<std::string, builtin_t> ::iterator it_builtins;
  for (it_builtins= builtins.begin(); it_builtins!=builtins.end(); it_builtins++){
    //cout << it->first << endl;
    matches.push_back(it_builtins->first);
  }

  // Add aliases that user declared
  std::map<std::string, std::string>::iterator it_aliases;
  for (it_aliases= aliases.begin(); it_aliases!=aliases.end(); it_aliases++){
    matches.push_back(it_aliases->first);
  }

  // remove different name from matches
  string text_string;
  size_t textLength = strlen(text);
  for(unsigned int i = 0; i < textLength; i++){
    text_string.push_back(*(text+i));
  }
  // cout << text_string << endl;
  std::vector<string>::iterator it = matches.begin();
  while(it != matches.end()){
    string string_in_matches = *it;
    string sub_string_in_matches = string_in_matches.substr(0,textLength);
    if(text_string.compare(sub_string_in_matches) != 0){
      it = matches.erase(it);
    }
    else{
      it++;
    }
  }

  // Add any programs that are in the user’s $PATH environment variable
  string curr_dir;
  DIR* dirp = NULL;
  struct dirent *directory;

  curr_dir = getenv("PATH");

  char *previous = new char[curr_dir.length()+1];
  char *pch;
  pch = strtok(strcpy(previous,curr_dir.c_str()),":");
  
  while(pch != NULL){
      // open file directory
    dirp = opendir((const char*)pch);
    if(dirp){
      while((directory = readdir(dirp)) != NULL){
        string path(pch);
        path.append("/");
        path.append(directory->d_name);
        //cout << path << endl;
        if(access(path.c_str(),X_OK) == 0){
          if(strcmp(directory->d_name, ".")!=0 && strcmp(directory->d_name, "..")!=0){
            if(((string)directory->d_name).find(text_string) ==0){
              //cout << directory->d_name << endl;
              matches.push_back(directory->d_name);
            } 
          }
        }
      }
    }
    pch = strtok(NULL," :");
    //cout << pch << endl;
  }
  
} 



char** Shell::word_completion(const char* text, int start, int end) {
  char** matches = NULL;

  if (text[0] == '$') {
    matches = rl_completion_matches(text, env_completion_generator);
  } else if (start == 0) {
    matches = rl_completion_matches(text, command_completion_generator);
  } else {
    // We get directory matches for free (thanks, readline!).
  }

  return matches;
}


char* Shell::env_completion_generator(const char* text, int state) {
  // A list of all the matches.
  // Must be static because this function is called repeatedly.
  static vector<string> matches;

  // If this is the first time called, construct the matches list with
  // all possible matches.
  if (state == 0) {
    getInstance().get_env_completions(text, matches);
  }

  // Return a single match (one for each time the function is called).
  return pop_match(matches);
}


char* Shell::command_completion_generator(const char* text, int state) {
  // A list of all the matches.
  // Must be static because this function is called repeatedly.
  static vector<string> matches;

  // If this is the first time called, construct the matches list with
  // all possible matches.
  if (state == 0) {
    getInstance().get_command_completions(text, matches);
  }

  // Return a single match (one for each time the function is called).
  return pop_match(matches);
}


char* Shell::pop_match(vector<string>& matches) {
  if (matches.size() > 0) {
    const char* match = matches.back().c_str();

    // Delete the last element.
    // matches.pop_back();

    // We need to return a copy, because readline deallocates when done.
    char* copy = (char*) malloc(strlen(match) + 1);
    strcpy(copy, match);

    // Delete the last element.
    matches.pop_back();
    return copy;
  }

  // No more matches.
  return NULL;
}
