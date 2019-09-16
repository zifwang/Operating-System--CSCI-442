/**
 * This file contains implementations of the functions that are responsible for
 * executing commands that are not builtins.
 *
 * Much of the code you write will probably be in this file. Add helper methods
 * as needed to keep your code clean. You'll lose points if you write a single
 * monolithic function!
 */

#include "shell.h"
#include <iostream>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>  
#include <cstdlib>
#include <fcntl.h>

using namespace std;

// helper function to print tokens
void print_tokens_helper(vector<string>& tokens);
// helper function to print argv
void print_argv(vector<command_t>& commands);

int Shell::execute_external_command(vector<string>& tokens) {
  // Declare a return code
  int return_code = 0;
  // Declare the status of fork process
  int status = 0;
  // Declare an empty vector into which we'll put the partitioned commands.
  vector<command_t> cmds;
  // Make 2 pipes
  int pipe_read[2];
  int pipe_write[2];
  // int pipe_tmp[2];   // anthor pipe
  // Attempts to create a pipe. the_pipe must be available to both processes.
  // if (pipe(pipe_read) < 0 ||pipe(pipe_write) < 0 ) {
  //   perror("Error opening pipe");
  //   return 1;
  // }
  if(partition_tokens(tokens,cmds)){
    // helper function here to check commands.
  // print_argv(cmds);
  // Create a pointer to hold cmds
  command_t* CCM = new command_t();
  // hold open file
  int fr_fd = STDIN_FILENO;  // file_read
  int fw_fd = STDIN_FILENO;  // file_write
  int fa_fd = STDIN_FILENO;  // file_append
  for (size_t i = 0; i < cmds.size(); i++){
    *CCM = cmds[i];
    // get argv from CCM
    std::vector<std::string> argument = CCM->argv;
    // Declare pointers t to hold tokens[i]
    std::vector<char*> tok(argument.size()+1);
    for(unsigned int j =0; j !=argument.size(); j++){
      tok[j] = &argument[j][0];
    }
    // get input file and type
    string input_file = CCM->infile;
    // CCM->input_type;
    // get ouput file and type
    string output_file = CCM->outfile;
    // CCM->output_type;
    
    if(CCM->output_type == WRITE_TO_PIPE && pipe(pipe_write) == -1){
      perror("pipe wrong");
      exit(1);
    }

    // Step 1: Open a pipe
    pid_t pid = fork();
    
    // Error checking
    if(pid == -1){
      perror("Error: fork failed");
      exit(1);
    }
    /* Child process: 
    *   -Check file type decide write or read
    *   -Use dups to copy
    *   -execs command
    */
    else if(pid == 0){
      //check input_type and ouput_type
      // Read_From_File case
      if(CCM->input_type == READ_FROM_FILE){
        fr_fd = open(input_file.c_str(), O_RDONLY);
        // if fr_fd can read a file, then dup2 itith multiple pipes

        if (fr_fd>0){
          if(dup2(fr_fd,STDIN_FILENO)<0){
            perror("Error Reading");
            exit(1);
          }
        }else {
          perror("Error opening file");
          exit(1);
        } 
      }
      // Read_From_Pipe Case
      if(CCM->input_type == READ_FROM_PIPE){
        if(dup2(pipe_read[0],STDIN_FILENO)<0){
          perror("Error Reading");
          exit(1);
        }
      }
      // Write_To_Pipe Case
      if(CCM->output_type == WRITE_TO_PIPE){
        if(dup2(pipe_write[1],STDOUT_FILENO) < 0){
          perror("Error Writing");
          exit(1);
        }
      }
      // Write_To_File Case
      if(CCM->output_type == WRITE_TO_FILE){
        fw_fd = open(output_file.c_str(),O_WRONLY|O_CREAT,0644);
        if(fw_fd>0){
          if (dup2(fw_fd,STDOUT_FILENO) < 0){
            perror("Error Writing");
            exit(1);
          }
        }else{
          perror("Error opening file");
          exit(1);
        }
      }
      // Append_to_file Case
      if(CCM->output_type == APPEND_TO_FILE){
        fa_fd = open(output_file.c_str(),O_WRONLY|O_CREAT|O_APPEND,0644);
        if(fa_fd>0){
          if (dup2(fa_fd,STDOUT_FILENO) < 0){
            perror("Error Writing");
            exit(1);
          }
        }else{
          perror("Error opening file");
          exit(1);
        }
      }
    // Debug use to check whether it is work or not
    // cout << "I'm the child; I have something important to say." << endl;
    // execs command
      execvp(argument[0].c_str(),tok.data());
    }
    /* Parent 1 Process */
    else{
      // close pipes
      if(CCM->output_type == WRITE_TO_PIPE){
        close(pipe_write[1]);
      }
      if(CCM->input_type == READ_FROM_PIPE){
        close(pipe_read[0]);
      }
      // check error if waitpid less than 0
      if (waitpid(pid,&status,0) < 0){
        perror("waitpid");
        exit(1);
      }
      // Renew pipe_read by pipe_write
      if(CCM->output_type == WRITE_TO_PIPE){
        pipe_read[0] = pipe_write[0];
        pipe_read[1] = pipe_write[1];
      }
      //return ceil((float)status/(float)status+1);
      return_code =  WEXITSTATUS(status);
    }
  }
  }else{
    perror("ERROR");
    exit(1);
  }
  
  return return_code;
}

// helper1 function to print tokens
void print_tokens_helper(vector<string>& tokens){
  unsigned int length = tokens.size();
  for (unsigned int i = 0; i < length; i++){
    cout << tokens[i] << endl;
  }
}
// helper2 function to print argv in command_t
void print_argv(vector<command_t>& commands){
  cout << "The size of commands is: " << commands.size() << endl;
  command_t* CCM = new command_t();
  for (size_t j = 0; j < commands.size(); j++){
    *CCM = commands[j];
    std::vector<std::string> argument = CCM->argv;
    size_t length = argument.size();
    cout << "It is " << j << " argv in command_t struct. ";
    cout << "It has arguments: ";
    for (size_t i = 0; i < length; i++){
      cout << argument[i] << " ";
    }
    string is_infile = CCM->infile;
    if(is_infile.length() != 0){
      cout << "Infile: " << CCM->infile << endl;
      cout << "input_type: " << CCM->input_type << endl;
    }
    string is_outfile = CCM->outfile;
    if(is_outfile.length() != 0){
      cout << "outfile: " << CCM->outfile << endl;
      cout << "output_type: " << CCM->output_type << endl;
    }
    cout << endl;
  }
}