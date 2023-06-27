Command Line Options
====================

.. glossary::

   ``-E``
     Explicitly checked and has a global variable corresponding to it but the global variable is never used.

   ``-HS``
     High-resolution shadow map. When present, the game will use a 2048x2048 shadow map instead of a 1024x1024 one.

   ``-I``
     When present, several initialization steps are skipped and the game crashes during startup. Among others, the ``InstallGameFiles`` command is not registered when this option is present. The check for this option is followed by an empty function indicating that the functionality associated with it was stripped from the release build.

   ``-L``
     When present, ``logfile.txt`` is used as a log file. There are 2 places in ``WinConsole_Z`` that could potentially write to this file but I have yet to observe this being used. It is likely that all logging calls were stripped in release builds.

   ``-mce``
     Related to `Windows XP Media Center Edition <https://en.wikipedia.org/wiki/Windows_XP_Media_Center_Edition>`_. When the game is lauched with this command line option, the ``%SystemRoot%\ehome\ehshell.exe`` executable will be run upon exiting the game, or if Windows Virtual-Memory is not enabled, if the executable is present.

   ``-W``
     Launch in windowed mode with right-click menu.
