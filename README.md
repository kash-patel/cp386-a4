# CP386 Assignment 4

An implementation of the Banker's Algorithm in C. Allows for a maximum of four resource types and a variable number of customers/threads.

## Motivation

Many computer processes require the use of multiple resources of multiple types at once to complete their work. However, using those resources haphazardly can result in the system being deadlocked, and unable to continue.

The Banker's Algorithm solved this problem by ensuring that each process' request for resources is satisfied only if it does not result in a potential deadlock in the future, and allows all processes to be serviced.

## Installation

No installation required, but it does require the Linux environment.

Downlad the files to a Linux environment (virtual machine or WSL works too) and ensure that Question1.c, sample4_in.txt, and Makefile are in the same directory.

## How to use

Navigate to the directory to which the files are downloaded and open a terminal.

Running the `make` command will compile the program file and run the output file with the input `10 5 7 8`.

## Screenshots

## Features

## Test Cases

## Usage Examples

## About the Developer

## Credits

## License