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
The root owns its children, which are owner of their own childer, in a recursive fashion.
The design pattern best suiting this structure is the *composite* pattern.
In this pattern the container object inherits all properties from the basic object, and can therefore
implement the same features or behaviors.
Still, it has the capability to handle a collection of objects, dispatching actions, messages, inserting
or removing them from the container.


