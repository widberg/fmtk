Mod Release Guidelines
======================

So you've finished making your FMTK mod. Congratulations! I'm sure you're eager to share it with the world. In the interest of consistency and accessability, I have authored several optional guidelines for you to consider before publishing your mod. It is important to remember that your mod is your property and you are free to disregard these guidelines.

Mod DB
------

To make searching for mods as easy as possible, I ask that you publish your mod to the `FUEL game on Mod DB <https://www.moddb.com/games/fuel/mods>`_. While I would much prefer a more popular website like `Nexus mods <https://www.nexusmods.com/>`_, the tradition of publishing FUEL mods to Mod DB was started by "FUEL: REFUELED" and upheld by "FUEL: RESHADED".

Open Source
-----------

In the interest of developing a large body of reference material, I ask that you make public portions or the entirety of your mod's source code on `GitHub <https://github.com/>`_. This will provide new modders with a wealth of information to reference while learning how to mod the game. Also this will let me see how the FMTK APIs are being used and where they can be improved. If you wrote scripts to help you while you developed your mod, I ask that you release those too so that they may by considered for inclusion in FMTK. Be sure to `choose a license that you think is fair for your property <https://choosealicense.com/>`_.

Report Bugs
-----------

FMTK is a complicated piece of software and it is bound to have bugs. When you or your mod's users encounter these bugs please `open an issue on the FMTK repository <https://github.com/widberg/fmtk/issues>`_ so that they may be tracked and fixed. It is tempting to implement a quick workaround to avoid the bug in FMTK but it is much better for the health of the community to fix these problems and we can't fix them if we don't know about them.

Give Feedback
-------------

Now that you've spent some time with the FMTK APIs you probably have some feedback. Let us hear it! If an API doesn't feel right or a feature would be useful to have, `open an issue on the FMTK repository <https://github.com/widberg/fmtk/issues>`_ so that we can track, discuss, and improve it.

Add Don't Replace
-----------------

While it is often easier to ``alias`` everything, that doesn't play nice with other mods that might depend on that file being stock. Try to use the provided APIs for adding content and avoid replacing existing content whenever possible. If there is something you would like to do but there isn't and API to do it without overwriting something, make a feature request. We pride ourselves on our commitment to mod compatibility and it is hard to uphold that commitment when mod developers feel the need to overwrite existing content to do what they want.

Test Your Mod
-------------

Before putting your mod out there for the world to see you should give it to a small group of people to play with for awhile. This will provide bugs an opportunity to show themselves before release. Giving it to other people is a good way to test compatibility with other mods that they might have installed that you don't. And since you have a direct line of contact with the people helping you test, it is much easier to communicate and fix bugs.
