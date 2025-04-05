# Supply Chain DLL Hijacking Attack

KeePassXC is an opernsource password manager, both versions load missing dynamic linked library (DLL), which is a shared library from Windows default directory.
Therefore, creating the missing DLL and place in the same directory of the application, it automatically load the DLL from the same directory of the application.
