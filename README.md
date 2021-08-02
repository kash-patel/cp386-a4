# CP386 Assignment 4

An implementation of the Banker's Algorithm in C. Allows for a maximum of four resource types and a variable number of customers/threads.

## Motivation

Many computer processes require the use of multiple resources of multiple types at once to complete their work. However, using those resources haphazardly can result in the system being deadlocked, and unable to continue.

The Banker's Algorithm solved this problem by ensuring that each process' request for resources is satisfied only if it does not result in a potential deadlock in the future, and allows all processes to be serviced.

## Installation

No installation required, but it does require the Linux environment.

Downlad the files to a Linux environment (virtual machine or WSL works too) and ensure that `Question1.c`, `sample4_in.txt`, and `Makefile` are in the same directory.

## How to use

### Running the program

Navigate to the directory to which the files are downloaded and open a terminal.

Running the `make` command will compile the program file and run the output file with the input `10 5 7 8`, representing how many of each of the four resource types the program will start with.

The program uses the file `sample4_in.txt` to make a list of processes and their maximum required resources.

### Commands

The program accepts five commands.

1. `RQ p a b c d`, where `p`, `a`, `b`, `c`, and `d` are integers, simulates process `p` making a request for resources. `a`, `b`, `c`, and `d` represent the number of resources of each type requested by process `p`. An error is shown if a) the request exceeds the process' maximum required reources and b) the request leaves the system in an unsafe state.
2. `RL p a b c d`, where `p`, `a`, `b`, `c`, and `d` are integers, simulates process `p` releasing resources. `a`, `b`, `c`, and `d` represent the number of resources of each type released by process `p`.
3. `Status` prints a vector showing how many of each resource are available, as well as matrices representing the number of each type of resource a) required in total by, b) allocated to, and c) still needed by each process.
4. `Run` calculates the safe sequence in which each process can be satisfied, and starts threads to satisfy each process in that sequence.
5. `Exit` exits the program.

### Customisation

The file `sample4_in.txt` can be modifed; it is a simple comma-separated file showing the maximum of each resource type each process needs. *Do not rename this file, as the program looks for `sample4_in.txt` to open it.*

To simulate fewer than four resource types, use `0` for the last columns (e.g. if you want to simulate only two resource types, the lines in the file should have the format `a,b,0,0` where `a` and `b` are integers).

You can compile and run the program file manually if you want to provide your own values for resource availability. Be sure to use the `-lrt` and `lpthread` compiler options. Provide your own resource availability values as space-separated arguments to the output file (e.g. if your output file is named `a.out`, a valid command would be `./a.out 1 2 3 4`).

Again, use `0` for the last arguments if you are simulating fewer than four resource types.

## Test Cases

The program has been tested with the provided `sample4_in.txt` file and the arguments in the Makefile (`10 5 7 8`).

## Usage Examples

## About the Developer

## Credits

## License