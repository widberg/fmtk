Developer Tools
===============

Command Palette
---------------

See the :ref:`Commands documentation entry<Commands>`.

Noclip
------

The ``SwitchCameraUser 0`` command can be used to enter and exit a freecam mode also know as the debug camera. When entering this mode the player's vehicle will be separated from the camera and remain active. When exiting this mode the player's vehicle will be teleported to the center of frame and normal gameplay will resume. There are several keys that can be used to control the debug camera. ``w``, ``a``, ``s``, and ``d`` can be used to move the camera relative to the direction it is facing. The ``y`` and ``i`` keys move the camera up and down very slowly relative to the direction the camera is facing. Note that ``y`` also opens the Command Palette so if you want to move up, I recommend opening the Palette and then closing it, holding the ``y`` key down as you close it to continue moving up. The arrow keys control the direction the camera is facing. Finally, holding down the ``e`` key will increase the effect of each of the other keys from ``0.25`` to ``5.00``, a 20x increase, this makes flying around a lot faster. Be careful when using ``e`` with the arrow keys as the camera will spin around wildly.

Teleport
--------

``ActiveDebugHUD`` is the command to enable this feature. The ``c`` key will maximize the minimap and the arrow keys will navigate the crosshair. The ``w`` and ``s`` keys will zoom in and out respectively. The ``t`` key will toggle the GPS Target at the crosshair. The ``enter`` key will set the teleport target at the crosshair (there is no visual feedback that this has happened). Press ``c`` again to minimize the minimap and teleport to the teleportation target if it was set.

Debug Tools
-----------

``ActiveDebugHUD`` is the command to enable this feature. The ``t`` key will toggle the visibility of this menu. The arrow keys and ``enter`` are used to navigate.

Native Popup Menu
-----------------

When the game is launched in windowed mode with the ``-W`` command line option, right-clicking on the game window will bring up a native style popup menu with developer options.

Immediate Mode Popup Menu
-------------------------

When the game is launched in windowed mode, ``EnablePopupMenu`` has been run, and ``EnableDebugViewport`` has been run, holding down the ``F1`` key will bring up an in-game GUI element that can be navigated with the numpad arrows, num lock is disregarded. The options in this menu are similar to the ones available in the Native Popup Menu.
