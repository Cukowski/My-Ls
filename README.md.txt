# Welcome to My Ls
Welcome to My Ls projeckt. 

Here I will try to explain basics of the program and how to make it work.

## Task
The task for the this assignment was to create a function that "my_ls" displays its name as well as any requested, associated information.

It basically needs to copy the ls function in terminal

## Description
To be able to solve this problem I used individual functions that receive the data, sort the data and print the requested data.

For each operand that names a file of type directory, my_ls displays the names of files contained within that directory, as well as any requested, associated information.

"my_ls" functions receives some basic commands such as "-a" "-t" "/dir" or all in the same time and prints whatever info required.

## Installation
You can use "make" and "make re" commands to install and activate the program.

Makefile has been flagged with -Wall -Wextra -Werror which is enough for this program.

You can use terminal to run the program "my_ls [-at] [file ...]"

## Usage
Basically after the "make" statement. Call the "./my_ls" and you can give it the argument you wanted.
my_ls [-at] [file ...]

```
./my_ls 
./my_ls -t
./my_ls -a 
./my_ls -a -t 
./my_ls -at 
 ./my_ls . 
 ./my_ls /tmp/
 ./my_ls dir1 dir2 
```

### The Core Team
aksoy_s
Selcuk Ata Aksoy 

<span><i>Made at <a href='https://qwasar.io'>Qwasar SV -- Software Engineering School</a></i></span>
<span><img alt='Qwasar SV -- Software Engineering School's Logo' src='https://storage.googleapis.com/qwasar-public/qwasar-logo_50x50.png' width='20px'></span>
