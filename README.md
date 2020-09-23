# MeshButtons

MeshButtons is a dedicated software service which provide to the MeshBox navigation functionality in Kodi, via its rotary encoders and push buttons.

It achieves this by reading the GPIOs status and sending commands to Kodi’s Event Server and it can be compiled and ran under any Linux platform.

It can work with two rotary encoders (both with click) and two push buttons in its current form, but, due to its design, it can easily be modified for other configurations.

For reading the GPIO pins associated with encoders and buttons MeshButtons uses memory mapped registers via dev/mem and mmap for faster acces while for sending commands to Kodi MeshButtons uses Kodi Event Server library.

The latter one requires Kodi Event Client library to be installed on the system where MeshButtons is compiled.
To do this on a machine running Linux Debian or any of its derivatives use the following command:

<b>sudo apt-get install kodi-eventclients-dev</b>
