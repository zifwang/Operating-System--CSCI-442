# To build the program (which is called simulator by default), simply type:
#   make
#
# To clean up and remove the compiled binary and other generated files, type:
#   make clean
#
# To build AND run the shell, type:
#   make run
#

# The name of your binary.
NAME = simulator

# Flags passed to the preprocessor.
CPPFLAGS += -Wall -MMD -MP -Isrc -g -std=c++11

# ALL .cpp files.
SRCS = $(shell find src -name '*.cpp')
OBJS = $(SRCS:src/%.cpp=bin/%.o)
DEPS = $(SRCS:src/%.cpp=bin/%.d)

# Default target. Build your 'mytop' program, using the real /proc filesystem.
$(NAME): $(OBJS)
	$(CXX) $(CPP_FLAGS) $^ -o $(NAME)

# Build and run the program.
run: $(NAME)
	./$(NAME) example_simulation

# Remove all generated files.
clean:
	rm -rf $(NAME)* bin/

# Ensure the bin/ directories are created.
$(SRCS): | bin

# Mirror the directory structure of src/ under bin/
bin:
	mkdir -p $(shell find src -type d | sed "s/src/bin/")

# Build objects.
bin/%.o: src/%.cpp
	$(CXX) $(CPPFLAGS) $< -c -o $@

# Auto dependency management.
-include $(DEPS)
