# MeshButtons

MeshButtons is a dedicated software service which provide to the MeshBox navigation functionality in Kodi, via its rotary encoders and push buttons.

It achieves this by reading the GPIOs status and sending commands to Kodi’s Event Server and it can be compiled and ran under any Linux platform.

It can work with two rotary encoders (both with click) and two push buttons in its current form, but, due to its design, it can easily be modified for other configurations.

For reading the GPIO pins associated with encoders and buttons MeshButtons uses memory mapped registers, via dev/mem and mmap, for faster acces while for sending commands to Kodi, MeshButtons uses Kodi Event Server.

It uses separate thread instances for polling each encoder and button and main thread is responsible for sending commands (actions) to Kodi Event Server. Communication between thread instances and main thread is done via an internal defined queue.

Using Kodi Event Server requires Kodi Event Client library to be installed on the system where MeshButtons is compiled.
To do this on a machine running Linux Debian or any of its derivatives the following command must be used:

<b>sudo apt-get install kodi-eventclients-dev</b>

In order to compile MeshButtons, first ARM Cross compiler toolchain must be installed, then the following command must be used:

<b>arm-linux-gnueabihf-g++ -Wall "Mesh_Buttons.cpp" -o "Mesh_Buttons" -I "mesh_functions.h" "mesh_functions.cpp" -pthread</b>

To install MeshButtons executable / binary on the target Raspberry Pi please refer to the software section.

<a href="https://github.com/mi-pogo/MeshBox/wiki"> >> MeshBox Wiki </a>
