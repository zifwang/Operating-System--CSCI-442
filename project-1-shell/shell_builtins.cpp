/**
 * This file contains the implementations of the builtin functions provided by
 * the shell (or will, once you've finished implementing them all).
 */

#include "shell.h"
#include <iostream>
#include <cstdlib>
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
#include <readline/readline.h>
#include <readline/history.h>
#include <iterator>
#include <sys/wait.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>
#include <ctype.h>

using namespace std;


int Shell::com_ls(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  int length = argv.size();
  // string his;
  // if (length == 1){
  //   //cout << argv[0] <<endl;
  //   his = argv[0];
  // }
  // else{
  //   //cout << argv[0] << argv[1] << endl;
  //   his = argv[0];
  //   for(int i = 1; i < length; i++){
  //     his = his + " " + argv[i];
  //   }
  // }
  // // Add command to history.
  // add_history(his.c_str());
  // Create a variable for work directory.
  DIR* dirp;
  struct dirent *directory;
  string dir;
  if (length == 1){
    //Open Project-1 work space directory
    dirp = opendir(".");
    if (dirp){
      while ((directory = readdir(dirp)) != NULL){
        printf("%s\n", directory->d_name);
      }
      closedir(dirp);
    }
  }
  // Need to be modified
  else if(length == 2){
    //Open Project-1 work space directory
    dirp = opendir(argv[1].c_str());
    if (dirp != NULL){
      while ((directory = readdir(dirp)) != NULL){
        printf("%s\n", directory->d_name);
      }
      closedir(dirp);
    }
    else {
      return 1;
    }
  }
  else{
    return 1;
  }
  
  return 0;
}


int Shell::com_cd(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  // how to check I am in home or bin directory ??
  int length = argv.size();
  // string his;
  // if (length == 1){
  //   //cout << argv[0] <<endl;
  //   his = argv[0];
  // }
  // else{
  //   //cout << argv[0] << argv[1] << endl;
  //   his = argv[0];
  //   for(int i = 1; i < length; i++){
  //     his = his + " " + argv[i];
  //   }
  // }
  // // Add command to history.
  // add_history(his.c_str());
  // Variables
  string sDirectory;
  //int length = argv.size();
  if (length == 1){
    sDirectory = "/home/zifwang";
    // cout << sDirectory << endl;
    // Navigate to the home directory
    chdir(sDirectory.c_str());
  }
  else if (length == 2){
    sDirectory = argv[1];
    // cout << sDirectory;
    // Navigate to the directory specified by the user
    DIR* dirp;
    dirp = opendir(argv[1].c_str());
    if (dirp != NULL) {
      chdir(sDirectory.c_str());
    }
    else{
      return 1;
    }
  }
  else {
    return 1;
  }
  return 0;
}


int Shell::com_pwd(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  // cout << "pwd called" << endl; // delete when implemented
  int length = argv.size();
  // string his;
  // if (length == 1){
  //   //cout << argv[0] <<endl;
  //   his = argv[0];
  // }
  // else{
  //   //cout << argv[0] << argv[1] << endl;
  //   his = argv[0];
  //   for(int i = 1; i < length; i++){
  //     his = his + " " + argv[i];
  //   }
  // }
  // // Add command to history.
  // add_history(his.c_str());
  // Load current work directory
  if(length == 1){
    char buff[FILENAME_MAX];
    GetCurrentDir( buff, FILENAME_MAX );
    std::string current_working_dir(buff);
    if(buff == NULL){
      perror("No path found");
      return 1;
    }
    cout << current_working_dir << endl;
  }
  else {
    return 1;
  }
  // End loading current work directory
  return 0;
}


int Shell::com_alias(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  // cout << "alias called" << endl; // delete when implemented
  // Implement History command
  int length = argv.size();
  // string his;
  // if (length == 1){
  //   //cout << argv[0] <<endl;
  //   his = argv[0];
  // }
  // else{
  //   //cout << argv[0] << argv[1] << endl;
  //   his = argv[0];
  //   for(int i = 1; i < length; i++){
  //     his = his + " " + argv[i];
  //   }
  // }
  // // Add command to history.
  // add_history(his.c_str());

  //Below implement alias
  string inputString;
  string map_key;
  string map_value;
  if (length == 1){
    // print alias
    std::map<std::string, std::string>::iterator it = aliases.begin();
    for (it= aliases.begin(); it!=aliases.end(); it++){
      cout << "alias " << it->first << "=" << "'"<< it->second << "'"<< endl;
    }
  }
  else if(length ==2){
    int pos = 0;
    int lengthString = 0;
    std::map<std::string, std::string>::iterator it = aliases.begin();
    for(int i = 1; i < length; i++){
      inputString = argv[i];
      lengthString = inputString.size();
      pos = inputString.find("=");
      //cout << pos << endl;
      if (pos == -1){
        return 1;
      }
      else{      // Get the Map key.
        map_key = inputString.substr(0,pos);
        map_value = inputString.substr(pos+1,lengthString);
        //cout << map_key << endl;         // Debug use
        //cout << map_value << endl;       // Debug use
        if (aliases.empty()){
          aliases[map_key] = map_value;
        } 
        else{ 
          for (it= aliases.begin(); it!=aliases.end(); it++){
          // cout << it->first << endl;  //Debug use
            if (map_key != it->first){
              aliases[map_key] = map_value;
            }
            else{
              it->second = map_value;
            }
          }
        }
      }
      //cout << lengthString << endl;  // Debug use
      //cout << inputString << endl;   // Debug use
      //cout << pos << endl;           // Debug use

    }
  }
  else {
    return 1;
  }
  return 0;
}


int Shell::com_unalias(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  // cout << "unalias called" << endl; // delete when implemented
  // Implement History command
  int length = argv.size();
  // string his;
  // if (length == 1){
  //   //cout << argv[0] <<endl;
  //   his = argv[0];
  // }
  // else{
  //   //cout << argv[0] << argv[1] << endl;
  //   his = argv[0];
  //   for(int i = 1; i < length; i++){
  //     his = his + " " + argv[i];
  //   }
  // }
  // // Add command to history.
  // add_history(his.c_str());

  // Below Implement Unalias
  if (length == 1){
    return 1;
  }
  else if(length == 2){
    string unalias_command = argv[1];
    //cout << argv[1] << endl; // Debug use
    if (unalias_command == "-a"){
      aliases.clear();
    }
    else{
      std::map<std::string, std::string>::iterator it = aliases.begin();
      for (it= aliases.begin(); it!=aliases.end(); it++){
        if(unalias_command == it->first){
          aliases.erase(unalias_command.c_str());
        }
        else{
          // I try in the linux terminal, an unalias command is able to support multiple DEL of alias
          // For example, (unalias a b c) is a valid command in the linux.
          return 1;
        }
      }    
    }
  }
  else{
    return 1;
  }
  return 0;
}


int Shell::com_echo(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  int length = argv.size();
  // string his;
  // if (length == 1){
  //   //cout << argv[0] <<endl;
  //   his = argv[0];
  // }
  // else{
  //   //cout << argv[0] << argv[1] << endl;
  //   his = argv[0];
  //   for(int i = 1; i < length; i++){
  //     his = his + " " + argv[i];
  //   }
  // }
  // // Add command to history.
  // add_history(his.c_str());
  // Implement echo command
  //int length = argv.size();
  if (length == 1) {
    cout << "\n";
  }else{
      for (int i = 1; i < length; i++){
    cout << argv[i] << " ";
  }
  cout << "\n";
  }
  return 0;
}


int Shell::com_history(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  //cout << "history called" << endl; // delete when implemented
  int length = argv.size();
  // string his;
  // if (length == 1){
  //   //cout << argv[0] <<endl;
  //   his = argv[0];
  // }
  // else{
  //   //cout << argv[0] << argv[1] << endl;
  //   his = argv[0];
  //   for(int i = 1; i < length; i++){
  //     his = his + " " + argv[i];
  //   }
  // }
  // // Add command to history.
  // add_history(his.c_str());
  // Print history
  if (length == 1){
    register HIST_ENTRY **the_list;
    register int i;
    the_list = history_list ();
    if (the_list){
      for(i = 0; the_list[i]; i++){
        printf("  %d %s\n", i+history_base, the_list[i]->line);
      }
    }
  }
  else{
    return 1;
  }
  return 0;
}


int Shell::com_exit(vector<string>& argv) {
  // TODO: YOUR CODE GOES HERE
  // The exit command will stop the program
  int length = argv.size();
  // string his;
  // if (length == 1){
  //   //cout << argv[0] <<endl;
  //   his = argv[0];
  // }
  // else{
  //   //cout << argv[0] << argv[1] << endl;
  //   his = argv[0];
  //   for(int i = 1; i < length; i++){
  //     his = his + " " + argv[i];
  //   }
  // }
  // // Add command to history.
  // add_history(his.c_str());
  // Implement exit
  if (length == 1){
    exit(0);
  }
  else{
    return 1;
  }
  return 0;
}
