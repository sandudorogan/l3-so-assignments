# Operating Systems assignments
Here you can take some possible solutions for the Operating Systems homeworks.

## Getting Stared
This repo is based on the [course's stuff's one](https://github.com/systems-cs-pub-ro/so-assignments).
For all the features, like a nice checker for both functionality and the coding style, first clone their GitHub repo, then copy the files from this repo over the ones in the same folder.
Note: All the projects are made starting from the skeleton code, provided in their Git repo.
<p>
[Here](https://ocw.cs.pub.ro/courses/so) you can find the assignments texts, with all the specifics and the constraints. 
I'm attending a in Romanian curricula, hence, the assignment's text language.
If otherwise specified in the homework's Readme file, the homework obedes all of the constraints imposed. 
</p>

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
These are 32 bit systems. In some cases, like the ```1-multi``` project, you can't run it on a 64 bit system. It needs it's dynamic libraries, compiled on 32 bits systems.

### Installing
Depending on the system you are on, and if the project supports it, project can be either compiled with the ```GNUmakefile``` on Linux, or ```Makefile``` on Windows. Just type ```make -f <Makefile_name>``` in the console.

## Testing
If otherwise specified in the project Readme file, the tests can be obtained from the [stuff's GitHub page](https://github.com/systems-cs-pub-ro/so-assignments). Every project has it's own specific stress tests. Obviously, you can't test a projects with an other one's checker. The tests are located in the ```checker``` folder of the project's name directory.
<p>
For example, the checker for the ```1-multi``` project is in the ```~1-multi/checker/``` folder.
For the checker to run, you should copy the executable file in the ```checker``` folder.
</p>

### Break down
Tests are made for every general functionality required. Also, they include a coding style check and a running time check.

### Coding style
The checker uses the ```checkpatch``` script from [Linus Torvald's GitHub page.](https://github.com/torvalds/linux/blob/master/scripts/checkpatch.pl). Make sure you have it in your PATH environment variable before running the rest. It will ask you the path to the code files.
The codyng style standard we shall adhere to is the Linux Kernel one.

## Authors
* **SO Stuff** - *The skeleton* - [theirs GitHub](https://github.com/systems-cs-pub-ro/). [Team members](https://ocw.cs.pub.ro/courses/so)
* **Sandu Dorogan** - *Solutions*

## License
These projects are licensed under the [Creative Commons Attribution-ShareAlike](https://creativecommons.org/licenses/by-sa/3.0/) License.

## Aknowledgments 
* Thanks to the SO team for the experience provided and for your overall work.


