DiegOS-GUI
==========

This is the codebase of the Graphical User Interface of the **DiegOS** project.
It was designed with interfaces in mind, to be able to compile and build
on hardware and software.

Architecture
------------

The structure of a **GUI** is similar to a tree, the root is the core of the
message handling and the rendering and the objects' coordination while branches
and leaves are dedicated to draw and execute commands.

The basic concept of the tree is ownership, or inheritance: root object is the master
of the tree, but not the owner of all objects.
The root owns its children, which are owner of their own children, in a recursive fashion.
The design pattern best suiting this structure is the *composite* pattern.
In this pattern the container object inherits all properties from the basic object, and can therefore
implement the same features or behaviors.
Still, it has the capability to handle a collection of objects, dispatching actions, messages, inserting
or removing them from the container.
Basic object are described by the *View* class.
Containers are described by the *GroupView* class.

Al visible (and not visible) objects are therefore inserted in a hierarchy were the root element is on top,
all other objects are linked into a list of siblings, each of them being the root element of additional linked lists
of siblings.
Every object points to the owner to be able to communicate with the root object, hence to be able to route messages
or commands to any other object.
A *GroupView* object can be the root element, having *View* or *GroupView* objects as children linked in a list.

Every *GroupView* object can run an event loop; running the event loop implies running the graphical engine (or any other
global feature).

Visual aspect, color palettes and rendering are customizable.
**Singleton** and **factory** patterns have been used to create run-time configurable interfaces, it is then possible to switch
between different color themes, different graphical shapes, and different rendering engines.

Build and run it!
-----------------

The provided makefiles target both Linux and Windows as host targets.
Generic hardware support leverages on *SDL2* library.
You'll need to either build your own *SDL2* by pulling a repository and compiling it, or install binaries.
How to accomplish this task is out of the scope of this readme.
**DiegOS-GUI** successfully compiled linked and run on Windows 10 and Slackware Linux, as a 32- or 64-bit application.

The future
----------

Next step is to integrate the library with **DiegOS** and add some hardware support using the native graphical driver interface.
Stay tuned!


                                   


   

