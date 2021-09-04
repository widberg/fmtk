Game Parameters
===============

Param/pp001.tsc
----------------------

The ``Param/pp001.tsc`` file contains a non-continuous sequence of ``PutParameter`` commands ranging from index 1 to 2094 of the form ``PP index [value] [platform]``. If platform is NOT excluded or ``PC``, the parameter is not set. Platforms include ``PC``, ``PS3``, and ``X360``. By default every parameter is ``0xFFFFFFFF``. If the value is excluded, a value of ``0x00000000`` is used.

Hot Reload
----------

The ``Param/pp001.tsc`` file can be hot reloaded by editing and saving ``Param/pp001.tsc`` then running the ``ReadParameters 1`` command via the in-game developer command palette for the changes to take place without restarting the game. Only code reading parameters after the reload will use the new values. Code that reads a parameter once at startup will not receive the new value.

Parameter Table
---------------

.. csv-table::
   :file: _files/fuelgameparam.csv
   :header-rows: 1
