### Project Proposal

* Project title: **Smart Wireless File Server**
* Team members: **Alex Langshur (Responsibilities: Everything)**
* Description: **This project will be a baremetal file server system. The system will be built arround two Raspberry Pi's -- one connected through UART to the computer (client) and one powered remotely (server). Both will be setup with radio transmitters and receievers that communicate using a custom transmission protocol (handshake, data packeting, error checking) over the 433MHz UHF band. The system will use customized compression and encryption schemes, as well as a smart server utility built around a 64GB SD card for storing and retrieving files.**
* Hardware required *(everything has been ordered)*:
  - **2x 433MHz Radio Transmitter**
  - **2x 433MHz Radio Receiver**
  - **2x Raspberry Pi A+**
  - **4x NPN Transistor**
  - **64GB SD Card**
  - **Breadboards, wires, etc...**
* Funds requested: **USD 20** *(the expenses go a bit over but no worries)*
* Major task breakdown:
  - **Get Pi's to communicate in two directions** *(backup plan is WiFi library)*
  - **Build wireless data transfer protocol** *(backup plan is WiFi library)*
  - **Build client features:**
    - **User interface**
    - **File compression system**
    - **File encryption system**
  - **Build server features:**
    - **File assembly system**
    - **SD card storage system**
* Goals for this week:
  - **Get two-direction communication working**
  - **Get data transfer protocol working** *(reach)*
* Goals for next week:
  - **Get file storage system working**
  - **Get compression and encryption scheme working**
* Goals for finals week:
  - **Get UI working**
  - **Wrap everything together**
* References and resources used
  - **electronics-diy.com** *(documentation for radio parts)*
  - **sparkfun.com** *(documentation for radio parts on Arduino)*
  - **CS107E SD storage library** *(storage system)*
  - **techiedelight.com** *(insparation for copmression and encryption schemes)*
  - **tutorialspoint.com** *(inspiration for data packeting protocol)*
