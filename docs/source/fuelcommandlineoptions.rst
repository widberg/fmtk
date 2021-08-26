Command Line Options
====================

.. glossary::

   -E
     Explicitly checked and has a global variable corresponding to it but the global variable is never used.

   -HS
     High-resolution shadow map. When present, the game will use a 2048x2048 shadow map instead of a 1024x1024 one.

   -I
     When present, several initialization steps are skipped and the game crashes during startup. Among others, the ``InstallGameFiles`` command is not registered when this option is present. The check for this option is followed by an empty function indicating that the functionality associated with it was stripped from the release build.

   -L
     When present, ``logfile.txt`` is used as a log file. There are 2 places in ``WinConsole_Z`` that could potentially write to this file but I have yet to observe this being used. It is likely that all logging calls were stripped in release builds.

   -mce
     Likely related to `Windows XP Media Center Edition <https://en.wikipedia.org/wiki/Windows_XP_Media_Center_Edition>`_. Appears to toggle Virtual Memory stuff.

   -W
     Launch in windowed mode with right-click menu.
