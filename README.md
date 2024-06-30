# Lexicographic-Sort-Concurrency

## Description:
This concurrent program consists of three processes that sort a list of seven distinct letters into lexicographic order. Specifically, this program asks the user to enter seven distinct letters first and then puts those letters in an array for sorting to be performed by three processes (P1, P2, P3). Each process is responsible for sorting three letters and putting them in the correct position in the array. By the end of the last process (P3), the entire array is sorted into lexicographic order and printed to the user. 

The user can choose to go to debug mode after they enter seven distinct letters. They must type "Y" to go to debug mode, where they can view additional information about each process. Specifically, each process will tell the user if it swapped any of the letters in the array or not. 

This program is made up of three files:<br><br>
&nbsp;&nbsp;&nbsp;&nbsp;**semun.h**:		A header file to initialize the semaphores<br>
&nbsp;&nbsp;&nbsp;&nbsp;**CSORT.h**:		A header file that contains the shared memory between all 3 processes<br>
&nbsp;&nbsp;&nbsp;&nbsp;**CSORT.c**:	 	Contains the code for the program<br><br>

&nbsp;&nbsp;&nbsp;&nbsp;**Makefile**:	Allows the user to compile and run the program


## Installation

Linux Operating System is required. 


If any other operating system (e.g., Windows, MAC OS) is being used, Oracle VM VirtualBox can be used to run Linux.


If Linux is not already installed on your operating system, use the following link to install it:

https://www.instructables.com/How-to-install-Linux-on-your-Windows/


## Usage

**Step 1:** Clone the repository to your local machine:
```
git clone https://github.com/YashKapoor1102/Lexicographic-Sort-Concurrency.git
```

**Step 2:** Ensure the "Terminal" is opened.

**Step 3:** Go to the folder where you cloned the repository by typing the following command:
```
cd Lexicographic-Sort-Concurrency
```

**Step 4:** Once you are in the folder, type the following command to compile all the source files:

	$ make

**Step 5:** To run the program, type the following command:

	$ ./CSORT

	This will ask you to enter seven distinct letters one by one. For example, if you want to 
	test the list -> (X, A, z, M, W, y, D), you will enter "X" as letter 1, "A" as letter 2, 
	"z" as letter 3, "M" as letter 4, "W" as letter 5, "y" as letter 6, and finally, "D" as 
	letter 7. 

**Step 6:** After you have entered seven distinct letters, type "Y" to go into debug mode to track how each process performs the swaps in the array. Otherwise, type "N" to proceed normally and 
	only get the program to print out the sorted array of letters.

**Step 7:** Wait a few seconds until the program finishes sorting the list into lexicographic order.
	It will print out the sorted list of letters after all processes have finished executing. 


Once you are done using this application, you can type the following command in the terminal to clean up your files:

$ make clean


## Credits
- Yash Kapoor

	
