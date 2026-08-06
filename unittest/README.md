## How to build
1. goto turtle directory
2. run command `cmake -S. -B../bd -DCMAKE_BUILD_TYPE=Debug`
3. run command `cmake --build ../bd`

## gdb debug
1. run command `gdb ../bd/turtleTest`
2. use `b <file name>:<line number>` to set breakpoint
3. use `r` to run
4. use `p <variable name>` to print variable
5. use `l' to print current source code context
6. use `<enter>` to continue last command
7. use `c` to continue running from the current break point
8. use `bt` to print stack trace
9. use `s` to step into function
10. use `n` to step next line of code (without stepping into function)
11. use `q` to quit
