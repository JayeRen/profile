student name: Jiayi Ren
ID: 1858485207
email addr: jiayiren@usc.edu
VS version: VS2015

hw2:
newest version: 
1. fix problem of empty pixel: because I set sorted vertex as v_sort, 
but I use v to compute val of ceil-y, which makes it impossible to be an
integer.

2. fix problem of color: I realize that *a[0] is different from (*a)[0]

*a[1] and *a[2] have no values while *a[0] == (*a) which points to an array[3] and has 3 values. 