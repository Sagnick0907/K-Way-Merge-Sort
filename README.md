# K-Way-External-Merge-Sort

# Seam-Carving
## Table of Content
  * [Problem Statement](#Problem-Statement)
  * [Description](#Description)
  * [Motivation and Goal](#Motivation-and-Goal)
  * [Technical Aspect](#technical-aspect)
  * [Technologies Used](#technologies-Used)


## Problem Statement
Given a file containing a large unsorted list of integers (Will not fit in your usual Laptop RAM) as input, generate an output file with non-descending sorted list of given integers.  

## Description
External Sorting is a class of algorithms used to deal with massive amounts of data that do not fit in memory. The question aims at implementing one such type: K-Way merge sort algorithm to sort a very large array. This algorithm is a perfect example of the use of divide and conquer where with limited resources large problems are tackled by breaking the problem space into small computable subspaces and then operations are done on them.  


## Motivation and Goal
K-way External Merge Sort efficiently sorts large datasets that exceed available memory capacity. The motivation lies in the need to handle massive datasets by optimizing external storage, typically disk drives. The goal is to divide the data into manageable chunks, sort each chunk internally, and merge them in a way that minimizes random access to external storage. The "K-way" aspect refers to concurrently processing a specified number of chunks during the merging phase, achieving a balance between sorting efficiency and external storage limitations.  

## Technical Aspect
Input format :  
- Input.txt contains space separated integers.  
- Program should take two command line arguments.  
- First command line argument is the input file path.  
- Second command line argument is the output file path.  
- For example: You have a directory named data in your current working directory inside which you have kept the input.txt file and you wish to create output.txt inside the data directory itself.Then, run the program using the following command.-
`./a.out ./data/input.txt ./data/output.txt`
Output Format :  
- Print the following details in the terminal window:  
- Number of integers in a temporary file.  
- Number of temporary files created.  
- Total time taken by the program up to 2 decimal places.  
- Output file should contain integers in non-descending sorted order.


## Technologies Used
- VS Code
- C++
- System calls
