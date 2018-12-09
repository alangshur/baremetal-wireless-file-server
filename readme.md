### Project Writeup

* Project title: *Wireless File Server*
* Team members: *Alex Langshur*
* Main Features (files in /src/):
  - **Note:** For the most part, every single module/component below was entirely self-inspired and built in-house. The map module below was modeled after that of an open-source third party and I was inspired by various articles on checksum and data packeting on how to implement my own wireless and wired communcation protocols.
  - *storage.c*: This module is a low-level wrapper to the "ff.h" module. It automates a variety of complex tasks such as rapidly reading, writing, updating, and deleting files, as well as safely managing the base structure of the SD-based File Allocation Table platform. 
  - *filesys.c*: This module is an interface for the above storage module. It automates a set of even more complex tasks, allows for comprehensive error-checking, and provides a clean interface to the server file system.
  - *map.c*: This module was inspired by the RXI type-safe hash map (MIT license). I built-up the underlining hash map features around the node-based RXI model and added some additional key features for a more powerful map data structure. 
  - *compress.c*: This module provides a relatively simple and decently fast compression and decompression algorithm. I didn't realize this until after I had built the compression module, but the algorithm I used is closely related to the run-length encoding algorithm. Rather than use any applicable run length, however, my algorithm is built upon the conjecture that, statistically, most packets of the same letter occur in groups of five or less.
  - *receiver.c*: This module provides a bare metal protocol for a 433 MHz UHF wireless receiver. It has *a lot* of built-in tools (a lot more than I actually ended up using in the final product), and provides a powerful protocol to sync the receiver with a transmitter, as well as a means to validate and respond to packet checksums.
  - *transmitter.c*: This module is the counterpart to the above receiver module. It provides a protocol to calibrate, wakeup, and sync with a 433 Mhz GPIO-based receiver line at a variable baud rate. 
  - *wire.c*: This module is an expansive wire-based communication protocol. It allows for transmission between two Rapsberry pi's in either direction at a variable bit rate. In addition, it has a suite of utilities that culminate into a extremely powerful multi-Pi data handshake. This handshake sends data and acknowledgments back and forth over the cable-connection until the content is agreed upon and a series of shared Fletcher checksums are approved.
  - *socket.c*: Like a network socket, this module provides the central system for communication between a server Rasberry Pi and client Raspberry pi. It ties every single module above into a wildly smart CRUD (create, read, update, and delete) utility for modifying data on the server-side and from the client-side.
* Other Features:
  - *Multi-media Redundancy*: I originally started off using a strictly wireless means of communicating between the two Pi's. However, after one of my 433 MHz UHF receivers stopped picking up most signals, I decided to add wire-based data transfer as a redundancy. It took *a lot* more time to implement, but the final system relies on both the wireless and wired data streams to approve checkums and negotiate the validity of data. Both are used in tandem to effectively and rapidly "fact check" one another.
  - *Portability & Ease-of-use*: One thing that I've found really important since first learning about Java and JVM in high school is the notion of software portability. I decided when I set out on this project that I would make it as portable as possbile. I ditched environment variables and moved all the necessary code to run the program into the repo. Furthermore, I setup a pretty smart Makefile that allows the user to compile and write directly to an SD card (without dragging files around). Finally, any program (from CRUD) can be executed in a single, clean command in your terminal. Since we can't access command-line arguments directly from main with our Raspberry Pi setup, I built a system to pass command-line arguments to the Makefile and then format the correct files with custom, compile-time directives. 
  
