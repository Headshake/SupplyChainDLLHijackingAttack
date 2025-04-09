# Supply Chain DLL Hijacking Attack

Supply Chain attack targets an organization's trusted third-party vendors or suppliers to gain access to their systems or data, often by compromising software or hardware used by the target organization.
The DLL hijacking refers to the dynamic linked library (DLL) that is a shared library that Windows use to execute different application to save memory resources, it loads functions that are needed to properly run the application.

# DLL hijacking KeePassXC version 2.6s 
KeePassXC is an opensource password manager, both versions 2.6.0 and 2.6.6 load missing DLL, e.g. opengl32.dll, version.dll, userenv.dll.
Therefore, creating the missing DLL and place in the same directory of the application, it automatically load the DLL in the same directory of the application.

## Download Procman to monitor the processes and libraries 
Run KeePassXC app while running Procman, search for processes that are shown "NAME NOT FOUND".


