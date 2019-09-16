# Project 4

A simulation of a trival OS memory manager.

# Name: 
    Zifan Wang

# File List
    - All codes are wrote in c++ in src folder

        - main.cpp: the enter point to the memory simulation project.

        - flag_parser folder: contains flag_parser.{h,cpp} files and flag_parser_tests.cpp file. flag_parser{h,cpp} are used to handle flag parsing, as well as struct to hold the input results. flag_parser_tests.cpp is used to test whether the project can handle flag parsing successfully. 

        - frame holder: contains frame.{h,cpp} files and frame_tests.cpp file. frame.{h,cpp} files are used to define the frame class. frame_tests.cpp file is used to test whether the project can handle frame correctly.

        - page holder: contains page.{h,cpp} files and page_tests.cpp file. page.{h,cpp} are used to define the page class. And test file is used to do some testing.

        - page table folder: contains page_table.{h,cpp} to define the page table class and page_table_tests.cpp file to do testing stuff.

        - physical_address folder: contains physical_address.{h,cpp} to define the physical address of a process in the main memory and physical_address_tests.cpp is used to do some tests.

        - process folder: contains process.{h,cpp} files to define process class. process_test.cpp is used to do some tests.

        - simulation folder: contains simulation.{h,cpp} files are responsible for running the memory simulation.

        - virtual_address folder: conatins virtual_address.{h,cpp} to define the virtual address of a process in the main memory and virtual_address_tests.cpp is used to do some tests.

# Features:
    No unusual or interesting features in this program. Do this program by the same data struct and requirements in the starter code.

# Hour Spent:
    The total hour spent on this project should be less than 10 hours.

# Belay's anomaly:
    Definition: increasing the number of frames will result in an increase in the number of page faults for certain memory access patterns.

    The sim_3 file is to simulate the Belay's anomaly. Type: 
     ./mem-sim -v -sFIFO -f4 inputs/sim_3
     ./mem-sim -v -sFIFO -f5 inputs/sim_3
     to see the Belay's anomaly happen. 

    The page faults will increase to 36 from 33 when increase max_frame from 4 to 5.

    Due to FIFO algorithm does not take any pervious patterns into account and it is only based on the age of present pages, FIFO will override a frequent using page, resulting more page faults.