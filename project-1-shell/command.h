/**
 * Contains the definition of the command_t struct.
 */

#pragma once
#include <vector>
#include <string>
#include <ostream>


/**
 * Enum representing the possible types of input sources.
 */
enum InputType {
  READ_FROM_STDIN,
  READ_FROM_FILE,
  READ_FROM_PIPE
};


/**
 * Enum representing the possible types of output destinations.
 */
enum OutputType {
  WRITE_TO_STDOUT,
  WRITE_TO_PIPE,
  WRITE_TO_FILE,
  APPEND_TO_FILE
};


/**
 * Simple representation of a command to execute. Includes the command's
 * arguments as well as information about its input and output types.
 */
struct command_t {
  /**
   * The array of arguments representing the command to execute.
   */
  std::vector<std::string> argv;

  /**
   * Where this command should get its input.
   */
  InputType input_type;

  /**
   * Where this command should write its output.
   */
  OutputType output_type;

  /**
   * The file from which this command should read its input. May be empty.
   */
  std::string infile;

  /**
   * The file to which this command should write its output. May be empty.
   */
  std::string outfile;

  /**
   * Constructor. Defaults input_type and output_type to READ_FROM_STDIN and
   * WRITE_TO_STDOUT, respectively.
   */
  command_t() : input_type(READ_FROM_STDIN), output_type(WRITE_TO_STDOUT) {}
};


/**
 * Partitions the given vector of tokens into one or more commands based on
 * the position of pipes or file redirects.
 *
 * @param tokens The tokens to partition
 * @param commands The vector to fill with the partitioned commands
 * @return true if successfully partitioned; false otherwise
 */
bool partition_tokens(
    std::vector<std::string> tokens,
    std::vector<command_t>& commands);


/**
 * Enable the output operator (<<) to output commands in a readable format.
 */
std::ostream& operator <<(std::ostream& out, const command_t& command);
