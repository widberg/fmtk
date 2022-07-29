Game Parameters
===============

Param/pp001.tsc
----------------------

The ``Param/pp001.tsc`` file contains a non-continuous sequence of 1429 ``PutParameter`` commands ranging from index 1 to 5655 of the form ``PP index value [platform]``. If platform is NOT excluded or NOT ``PC``, the parameter is not set. Platforms include ``PC``, ``PS3``, and ``X360``. The size of the array containing the parameters is ``0x2000``. Therefore, valid indices are in the range ``[0x0000, 0x1FFF]``.

Hot Reload
----------

The ``Param/pp001.tsc`` file can be hot reloaded by editing and saving ``Param/pp001.tsc`` then running the ``ReadParameters 1`` command via the in-game developer command palette for the changes to take place without restarting the game. Only code reading parameters after the reload will use the new values. Code that reads a parameter once at startup will not receive the new value.

Values
------

The value of a game parameter can be expressed in several ways. By default every parameter is ``0``.

The first, and most common, way to express the value is with a numeric argument. If the argument is numeric, then the numeric representation of the argument is used as the value of the parameter.

Next, if the string representation of the argument matches the pattern ``R\d*G\d*B\d*A\d*``, then the value will be the packed RGBA representation of the argument. That is, a 4 byte array where each byte is the value of the corresponding RGBA component in that order. Component values are clamped in the range [0, 255]. This feature does not appear to be used in FUEL.

Finally, if none of the previous conditions were matched, then the value is assumed to be an arbitrary string and the CRC-32 hash of the string representation of the argument is used as the value.

Parameter Table
---------------

.. csv-table::
   :file: _files/fuelgameparam.csv
   :header-rows: 1
