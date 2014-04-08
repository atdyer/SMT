#SMT - ADCIRC Subdomain Modeling Tool
-------------------------------------------------------------------------------

##About SMT
-------------------------------------------------------------------------------
![](https://raw.githubusercontent.com/atdyer/SMT/master/images/quadtree.png)


##Compiling
-------------------------------------------------------------------------------

### Linux/Mac OS X

Make sure the Qt development libraries are installed:

* In Ubuntu/Debian: `sudo apt-get install libqt4-dev libqt4-opengl-dev zliblg-dev`
* In Fedora:        `yum install qt-devel`
* In Arch Linux:    `pacman -S qt`
* In Mac OS X with [Homebrew](http://mxcl.github.io/homebrew/): `brew install qt`

Or install the Qt Application Framework by choosing the appropriate system
and installing from the [Qt Downloads Page](http://qt-project.org/downloads)

Compile by running:

    $ qmake (or qmake-qt4 on some systems, like Fedora)
    $ make

You can now use SMT by running bin/SMT

### Windows

Install the Qt Application Framework by choosing the appropriate system
and installing from the [Qt Downloads Page](http://qt-project.org/downloads) 

***Further Windows instructions coming soon***

  
##Installing
---------------------------------------------------

### Linux/Mac OS X

To install SMT, you can run `make install`. By default SMT will install
to `/usr/local`. You can change this prefix when running qmake, and/or you can
change the install root when running make install, as follows:

Use `/usr` instead of `/usr/local`:

    $ qmake -r PREFIX=/usr
  
(Recursive flag, -r, needed when it's not the first time you're running qmake
because this affects nested files)

Install to some specified directory:

    $ make install INSTALL_ROOT=/home/username/adcirc-programs
    
### Windows

***Windows installation instructions coming soon***

