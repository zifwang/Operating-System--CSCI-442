# To build the shell (which is called MyShell by default), simply type:
#   make
#
# To clean up and remove the compiled binary, type:
#   make clean
#
# If you doing development on a Mac, then be aware that Mac OS X has a different
# version of the readline library than Alamode.
#
# To compile on a Mac, follow these instructions:
#   - Install readline from source. I used Homebrew (http://brew.sh):
#       brew install readline
#
#   - Take note of where Homebrew says it wrote the lib and include folders.
#     Update CPP_FLAGS and LDFLAGS if your locations are different.
#
#   - Then type:
#       make
#
# WARNING: You may develop your shell on any machine you want, but it *MUST*
# compile and run on the Alamode lab computers!
#
# If you have trouble compiling, feel free to post a question on Piazza.

CC = g++
OBJS = *.cpp
HEADERS = *.h
NAME = MyShell
COMMON_FLAGS = -Wall -l readline

ifeq ($(shell uname),Darwin)
	CPP_FLAGS = $(COMMON_FLAGS) -I/usr/local/opt/readline/include
	LDFLAGS = -L/usr/local/opt/readline/lib
else
	CPP_FLAGS = $(COMMON_FLAGS)
	LDFLAGS =
endif

all: $(NAME)

$(NAME): $(OBJS) $(HEADERS)
	$(CC) $(OBJS) -o $(NAME) $(CPP_FLAGS) $(LDFLAGS)

debug: $(OBJS) $(HEADERS)
	$(CC) $(OBJS) -o $(NAME) $(CPP_FLAGS) $(LDFLAGS) -g

run: $(NAME)
	./$(NAME)

clean:
	rm -rf $(NAME)*
