# Learning to use Syscalls using C

## Task 1

### Description

This piece of code will take an input source file, create a directory named **Assignment**, and reverse the source file and change the case of characters and store it into a file named **output.txt** in the **Assignment** directory.

### Running Instructions

* gcc task1.c
* ./a.out [source_file]

## Task 2

### Description

This piece of code will,
* Create a symlink to the file **Assignment/output.txt** in the **Assignment** directory, named **link**
* Verify the creation of **Assignment** directory, **Assignment/output.txt** file and **Assignment/link** symlink
* Check permission for the above mentioned directory, file and symlink after checking that they exist
* Check if **Assignment/output.txt** was created, reversed, and toggle-cased.


### Running Instructions

* gcc task2.c
* ./a.out [file_to_check_against_the_reversed_file]
