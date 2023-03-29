Vehicle Parameters
==================

Param/VehicleParam.tsc
----------------------

The ``Param/VehicleParam.tsc`` file starts with ``DisableConsole on`` then contains series of 679 ``VehiclePP`` commands each separated by a ``EndOfVehiclePP`` command. and ends with ``DisableConsole``. The ``DisableConsole`` bookend commands are to ensure that the command processor is accepting commands while the the file is run. Each sequence of 679 ``VehiclePP`` commands sets up the parameter buffer for an individual vehicle. Once all 679 ``VehiclePP`` commands for a vehicle have run, ``EndOfVehiclePP`` is run to commit the parameter buffer to the vehicle corresponding to the name given by parameter index 0.

FUEL Fandom Wiki VehiclesParam.tsc Entry
----------------------------------------

A few of the parameters have been investigated on the `FUEL Fandom Wiki VehiclesParam.tsc entry <https://fuel.fandom.com/wiki/VehiclesParam.tsc>`_. The information on this page has not been independently verified or reverse engineered but is linked to for completeness.

Hot Reload
----------

The ``Param/VehicleParam.tsc`` file can be hot reloaded by editing and saving ``Param/VehicleParam.tsc`` then running the ``ReloadVehicleParameters`` command via the in-game developer command palette and switching to another vehicle and back for the changes to take place without restarting the game. Only code reading parameters after the reload will use the new values. Code that reads a parameter once at startup will not receive the new value.

Debugging
---------

The ``DeBugInfos`` command will enable a debug view of the current vehicle's configuration.

Parameter Table
---------------

.. csv-table::
   :file: _files/fuelvehicleparam.csv
   :header-rows: 1
