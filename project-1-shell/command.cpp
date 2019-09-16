/**
 * Contains the implementation of functions declared in command.h.
 */

#include "command.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <stdio.h>

using namespace std;


bool partition_tokens(vector<string> tokens, vector<command_t>& commands) {
    // Declare current commnad: CCM
	command_t* CCM = new command_t(); 		   
	for (size_t i = 0; i < tokens.size(); i++){
    //write to file
    if (tokens[i] == ">"){		
      // file destinations wrong
			if (CCM->output_type != WRITE_TO_STDOUT){
        perror("Wrong command");
        return false;
      }
			// ">" and ">>" in the first or last i of input, return false
      if (i == 0 || i == (tokens.size() - 1)){
				perror("Wrong command");
				return false;
			}
      // ">" and ">>" follow by symbols, return false
      if (tokens[i+1] == "<" || tokens[i + 1] == "|" || tokens[i+1] == ">>" || tokens[i+1] == ">"){
				perror("Wrong command");
				return false;
			}
      // write file name after ">" to output file
      CCM->outfile = tokens[i+1];
      CCM->output_type = WRITE_TO_FILE;
			i++;	
		}
    // Append to file
    else if (tokens[i] == ">>"){
      // file destinations wrong
			if (CCM->output_type != WRITE_TO_STDOUT){
        perror("Wrong command");
        return false;
      }
			// ">" and ">>" in the first or last i of input, return false
      if (i == 0 || i == (tokens.size() - 1)){
				perror("Wrong command");
				return false;
			}
      // ">" and ">>" follow by symbols, return false
      if (tokens[i+1] == "<" || tokens[i + 1] == "|" || tokens[i+1] == ">>" || tokens[i+1] == ">"){
				perror("Wrong command");
				return false;
			}
      // write file name after ">>" to output file
      CCM->outfile = tokens[i+1];
      CCM->output_type = APPEND_TO_FILE;
			i++;	
    }
    // read from file
    else if (tokens[i] == "<"){
      // file destinations wrong
			if (CCM->input_type != READ_FROM_STDIN){
        perror("Wrong command");
        return false;
      }
      // "<" in the first or last i of input, return false																		
      if (i == 0 || i == (tokens.size() - 1)){
				perror ("Wrong command");
				return false;
			}
      // "< follow by symbols, return false
      if (tokens[i+1] == "<" || tokens[i + 1] == "|" || tokens[i+1] == ">>" || tokens[i+1] == ">"){
        perror ("Wrong command");
        return false;
      }
      // read from file
			CCM->infile = tokens[i+1];
			CCM->input_type = READ_FROM_FILE;
			i++;
		} 
    // pipe 
    // "|" symbol represent pipe
    else if (tokens[i] == "|"){															
			// argv of command_t has length 0, return false
      if (CCM->argv.size() == 0){
				perror ("Wrong command");
				return false;
			}
      // Wrong output file destinations
			if (CCM->output_type != WRITE_TO_STDOUT){
        perror ("Wrong command");
				return false;
			}
      // first and last command is |, then wrong
      if (i == 0 || i == (tokens.size()-1)){
        perror ("Wrong command");
        return false;
      }
			CCM->output_type = WRITE_TO_PIPE;
			commands.push_back(*CCM);
			CCM = new command_t();
			CCM->input_type = READ_FROM_PIPE;
		} 
    else {						
      // add current token to commands.argv												  
			CCM->argv.push_back(tokens[i]);	
		}
	}
 	commands.push_back(*CCM);
	delete CCM;

  return true;
}


// Feel free to ignore everything below here. It's just code to allow you to
// cout a command in a nice, pretty format. =)


const char* input_types[] = {
  "READ_FROM_STDIN",
  "READ_FROM_FILE",
  "READ_FROM_PIPE"
};


const char* output_types[] = {
  "WRITE_TO_STDOUT",
  "WRITE_TO_PIPE",
  "WRITE_TO_FILE",
  "APPEND_TO_FILE"
};


ostream& operator <<(ostream& out, const command_t& cmd) {
  copy(cmd.argv.begin(), cmd.argv.end(), ostream_iterator<string>(out, " "));

  out << "\n    input:   " << input_types[cmd.input_type]
      << "\n    output:  " << output_types[cmd.output_type]
      << "\n    infile:  " << cmd.infile
      << "\n    outfile: " << cmd.outfile;

  return out;
}
