Installation
====================

TestProject provides different ways of installation.


Source Code
--------------

To compile and install TestProject from the source code, you should meet
the following requirements:

* `ASNeG OPC UA Stack`_ >= 4.0.0
* CMake
* C++ compiler with C++11 support

To install `ASNeG OPC UA Stack`_, follow the instructions in `the following document <https://opcuastack.readthedocs.io/en/release4/1_getting_started/installation.html>`_.


**Linux (Ubuntu or Debian)**

To compile the source code and install TestProject locally, you should 
run in the root directory of the sources:

::

  $ sh build.sh -t local -s ASNEG_INSTALL_DIR 

*ASNEG_INSTALL_DIR* is the path where ASNeG OPC UA Stack is installed.

By default, the server is installed into *~/.ASNeG* directory. You can change it with
option *-i* (run *sh build.sh* for more details). 

Now the TestProject can be started.

::
  
  $ export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/.ASNeG/usr/lib/
  $ OpcUaServer4 ~/.ASNeG/etc/OpcUaStack/TestProject/OpcUaServer.xml

The TestProject provides the following communication interfaces:


**Windows**

To compile the project, you should install MSBuild Tools, then run in the environment which
is suitable for your target platform (e.g., Native x86) the following command:

::

  $ build.bat -t local

If you would like to build x64 version, you should set the suitable CMake generator for your environment with option *-vs*. For example:

::

  $ build.bat -t local -vs "Visual Studio 15 2017 Win64"


By default, the server is installed into *C:\\ASNeG* directory. You also can change it with option *-i*:

::

  $ build.bat -t local -i C:\another\path\to\install

Now the TestProject can be started.

::
  
  $ set PATH=%PATH%;C:\ASNeG\lib
  $ OpcUaServer4 CONSOLE C:\ASNeG\etc\OpcUaStack\TestProject\OpcUaServer.xml

This example is for the default case where the stack and the server are installed in *C:\\ASNeG* directory.


DEP Packages
--------------

You can install TestProject by using a DEP package, which you can download `here <https://github.com/ASNeG/TestProject/releases/>`_ or build yourself by the following command:

::

  $ sh build.sh -t deb -s ASNEG_INSTALL_DIR 

Then TestProject is available for installation by the command:

::

  $ dpkg -i TestProject-x.y.z-x86_64.deb 

TestProject will be installed as a service, and you can check its status typing:

::

  $ service TestProject status


RPM Packages
-------------

Users of RPM distributives can install TestProject by using an RPM package, which can be downloaded `here <https://github.com/ASNeG/TestProject/releases/>`_ or built by the following command:

::

  $ sh build.sh -t rpm -s ASNEG_INSTALL_DIR 

Then TestProject is available for installing by the command:

::

  $ rpm -i TestProject-x.y.z-x86_64.rpm 

 

MSI Packages
--------------

On Windows, TestProject can be installed with MSI packages. The MSI packages belong to two kinds: a *usual* package without `ASNeG OPC UA Stack`_ and a *standalone* package including `ASNeG OPC UA Stack`_ and the all needed dependencies. The *usual* package is more suitable for a machine with several
OPC UA applications and you would like to update the stack and the dependencies for all of them. The *standalone* package is easy for distribution, so you
don't need to install anything except TestProject.

You can download the packages `here <https://github.com/ASNeG/TestProject/releases/>`_ or build them.

To build the *usual* package, use the command:

::

  $ build.bat -t msi

For building the *standalone* one:

::

  $ build.bat -t msi -S


TestProject is installed as a service, so you can check its status in Windows Service Manager.

Docker
-----------

You can use Docker image instead of installing the web server on you machine:

:: 

  $ docker build -t TestProject:latest . 
  $ docker run -d -p 4842:4842 TestProject:latest



References
-----------

* `ASNeG OPC UA Stack`_

.. _`ASNeG OPC UA Stack`: https://asneg.github.io/projects/opcuastack

