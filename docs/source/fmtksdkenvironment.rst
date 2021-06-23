Native Mod Environment
======================

When the game loads it will recursively check each subdirectory of the ``<mods>/native`` directory for files with the extension ``.dll``. If found it will load the mod. Directories and files starting with the character ``_`` will not be searched or loaded and neither will their subdirectories, this provides a way to disable mods without deleting them. You can rename individual mods to begin with the character ``_`` or move them to a directory named something like ``_disabled``.
