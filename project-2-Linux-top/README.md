Project 2: Build top
Name: Zifan Wang
Files: All coding files are in the src folder.
    - info subfolder:
        - cpu_info.{h,cpp}: get a collection of CPU and system architecture dependent
                            items, for each supported architecture a different list.
        - load_average_info.{h,cpp}: load average figures giving the number of jobs in 
                            the run queue (state R) or waiting for disk I/O (state D) 
                            averaged over 1, 5, and 15 minutes.
        - memory_info.{h,cpp}: get statistics about memory usage on the system.
        - process_info.{h,cpp}: get information about a single process.
        - system_info.{h,cpp}: information about a system at a given point in time.
    - utils subfolder: 
        - flags.{h,cpp}: get user input. Enter "-h" print help table.
                                         Enter "-d Time" delay command.
                                         Enter "-s Sort_keys" sort command.
        - formatting.{h,cpp}: format information to be suitable to display
        - sort_function.{h,cpp}: when user input "-s sort_key", sort_function makes
                                program to display the program table by order based on
                                sort_key.
        - statistics.{h,cpp}: calculate cpu utilization
    - main.cpp: get user input and display top
Unusual features: 
    - is able to display help table when top is running.
Time: 12-15h
Terminal Size: 64*21 or larger.