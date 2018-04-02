# Operating Systems assignments
Here you can take some possible solutions for the Operating Systems homeworks.

## Getting Stared
This repo is based on the [course's stuff's one](https://github.com/systems-cs-pub-ro/so-assignments).
Note: All the projects are made starting from the skeleton provided on this page.

[Here](https://ocw.cs.pub.ro/courses/so) you can find the assignments with all the specifics and the constraints. 
I'm attending a in Romanian curricula, hence, the assignment's text is in the Romanian.
If otherwise specified in the homework's Readme file, the homework obedes all of the constraints. 

### Prerequisites
All of the assignments are developed on either:
* Ubuntu Linux 16.04
  - kernel 4.4
  - glibc 2.23
  - gcc version 5.4
or
* Windows 7 Professional
  - Visual C++ Express 2010
  - cl.exe
These are 32 bit systems. In some cases, like the '''1-multi''' project, you can't run it on a 64 bit system. It needs it's dynamic libraries, compiled on 32 bits.

### Installing
Depending on the system you are on, and if the project supports it, project can be either compiled with the '''GNUmakefile''', on Linux, or '''Makefile''' on Windows.

## Testing
If otherwise specified in the project, the tests can be optained on the [stuff's GitHub page](https://github.com/systems-cs-pub-ro/so-assignments) from the same folder as the project's name, in the folder '''checker'''.
For example, the checker for the '''1-multi''' project is in the '''~1-multi/checker/''' folder.
For the checker to run, you should copy the executable file in the '''checker''' folder.

### Break down
Tests are made for every general functionality required. Also, they include a coding style check and a running time check.

### Coding style
The checker uses the '''checkpatch''' script from [Linus Torvald's GitHub page.](https://github.com/torvalds/linux/blob/master/scripts/checkpatch.pl). Make sure you have it in your PATH variable before running the rest. It will ask the path to the code.
The standard used is Linux Kernel.

## Authors
* **SO Stuff** - *The skeleton* - [theirs GitHub](https://github.com/systems-cs-pub-ro/) [Team members](https://ocw.cs.pub.ro/courses/so)
* **Sandu Dorogan** - *Solutions*

## License
These projects are licensed under the [Creative Commons Attribution-ShareAlike] License.

## Aknowledgments 
* Thanks to the SO team for the experience provided and for your overall work.


