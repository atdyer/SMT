| A note to ADCIRC Developers and SMT Users |
|:-------------------------------------------:|
| SMT is in active development, and as such, I expect that you will probably run into some speedbumps or find some bugs that I've overlooked. Should this be the case, I encourage you to report these problems/bugs to me (either by email or through github) so that they can be fixed for future users. Additionally, be sure to pull the code frequently as bug fixes and updates to the software will be happening continuously. |
| -Tristan |

#ADCIRC Subdomain Modeling Tool
-------------------------------------------------------------------------------

Welcome to the ADCIRC Subdomain Modeling Tool (SMT) github repository. This page will contain
the latest version of the SMT code and [user manual](https://github.com/atdyer/SMT/wiki/SMT-User-Manual). Please check
back frequently for updates!

##About SMT
-------------------------------------------------------------------------------
![](https://raw.githubusercontent.com/atdyer/SMT/master/images/caseStudy_allCreated.png)

This section will contain a brief description of SMT. Soon...

##Compiling and Running SMT
-------------------------------------------------------------------------------

### Linux/Mac OS X

#####Install Qt Development Libraries

There are a number of methods for installing the Qt Development Libraries, and the method
you choose will be dependent on your system and how comfortable you are working in a 
command-line environment. If you are comfortable using the command-line, the easiest
method is to use your distro's package manager:

* In Ubuntu/Debian: `sudo apt-get install libqt4-dev libqt4-opengl-dev libglew1.6-dev`
* In Fedora:        `yum install qt-devel`
* In Arch Linux:    `pacman -S qt`
* In Mac OS X with [Homebrew](http://mxcl.github.io/homebrew/): `brew install qt`

If you'd rather not use the command-line, Qt provides installers on their website.
Install the Qt Application Framework by choosing the appropriate system and installing
from the [Qt Downloads Page](http://qt-project.org/downloads).

#####Update Graphics Drivers (optional, but recommended)

*Instructions coming soon*

#####Download the Source Code

The source code is available for download from the GitHub page. In the right column of
this page, click the Download ZIP button to download the full source code. Once you have
downloaded and unzipped this file, you are ready to compile.

#####Compile the Source Code

In order to compile the source code, open a command terminal and navigate to the directory
containing the SMT source code:

    $ cd /path/to/install/dir/SMT/

To ensure that you are in the correct directory, use the `ls` command to list all files
in the present directory. If the file SMT.pro is listed, you are ready to compile.

Compile by running:

    $ qmake (or qmake-qt4 on some systems, like Fedora)
    $ make

Note that compiling will probably take a few minutes to complete.

#####Run SMT

You can now use SMT by running the newly created executable.
Assuming you are still in the SMT/ directory:

    $ ./SMT

### Windows

Install the Qt Application Framework by choosing the appropriate system
and installing from the [Qt Downloads Page](http://qt-project.org/downloads) 

***Further Windows instructions coming soon***

##Using SMT
-------------------------------------------------------------------------------

Check out the [User Manual](https://github.com/atdyer/SMT/wiki/SMT-User-Manual) for help on using SMT.


