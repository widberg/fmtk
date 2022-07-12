Totem Script
============

Totem Script is a shell-like scripting language used in FUEL. Most of the textual game files are Totem Script scripts. Two of the extensions used for Totem Script are ``.tsc`` (Totem Script) and ``.pc`` in the ``trtext`` directory. In addition to being used in game files, the language is used in the in-game console and programmatically throughout the game executable. Each line of a Totem Script file is executed sequentially. If a line fails to execute then it is ignored and execution continues with the next line. Whitespace is ignored.

Command arguments have two types. The first type is a numeric argument. Numeric arguments are backed by a float value but in practice they are used as integers, booleans, and floats. The second type is a string argument. String arguments are unsurprisingly backed by a C-string value.

Commands
--------

All command names are case insensitive. Only the first ``15`` capital letters and numbers in each command name are required, this is called the "short form command name" and it is also case insensitive. However, I recommend using the full command name in scripts for readability.

See the :ref:`Commands documentation entry<Commands>` for a list of commands available in FUEL.

Directives
----------

Directives are like commands built into the script evaluator instead of registered by the game. All directives are case insensitive. Directives are only available when executing a script file, so they cannot be used from the in-game console.

.. code-block:: c

   #set name

Sets a variable.

.. code-block:: c

   #define name

Alias for ``#set``.

.. code-block:: c

   #unset name

Unsets a variable.

.. code-block:: c

   #if expression

If the expression evaluates as true then this block is executed. Otherwise, it is skipped.

An expression is a sequence of one or more variable names separated by the conditional operators ``||`` (Binary OR) and ``&&`` (Binary AND). If a variable name is set then it evaluates as true. Otherwise, it evaluates as false. Note that the expression is not escaped or quoted in any way. For example, ``#if CD_MAKE_BF || CD_VERSION`` will be satisfied when either ``CD_MAKE_BF``, ``CD_VERSION``, or both are set. But it will not be satisfied if neither are set.

Rather than having operator prescience, the expression is evaluated left to right with no operator prescience. For example, while you might expect the expression ``name1 || name2 && name3 || name4`` to be evaluated as ``((name1 || name2) && (name3 || name4))`` it is actually evaluated as ``(((name1 || name2) && name3) || name4)``. Note that parentheses are not supported in expressions, they are only used here to illustrate the order of operations. Also, the conditional expressions do not have short circuit evaluation, not that this matters since there are no side effects.

.. code-block:: c

   #ifdef expression

Alias for ``#if``.

.. code-block:: c

   #ifnot expression

Equivalent to ``#if`` except that it is satisfied when the expression evaluates as false.

.. code-block:: c

   #ifndef expression

Alias for ``#ifnot``.

.. code-block:: c

   #else

If the matching ``#if``, ``#ifdef``, ``#ifnot``, or ``#ifndef`` was unsatisfied, then this block is executed. Otherwise, it is skipped.

.. code-block:: c

   #endif

Close the matching ``#if``, ``#ifdef``, ``#ifnot``, ``#ifndef``, or ``#else`` block and resume unconditional execution.

Constants
---------

All constants are case insensitive.

.. code-block:: c

   true

Evaluates to a numeric argument of value 1.

.. code-block:: c

   false

Evaluates to a numeric argument of value 0.

.. code-block:: c

   on

Evaluates to a numeric argument of value 1.

.. code-block:: c

   off

Evaluates to a numeric argument of value 0.

Numeric Values
--------------

Integer, boolean, and float values can be used as arguments to commands. Integer and float values must be expressed in base 10 since there is no special handling for other bases. Boolean values can be expressed using the constants or numerically. A value of ``0`` represents false while any non-zero value represents true.

String Values
-------------

For basic strings with no whitespace, quotes, or backslashes, the string can be given as an argument verbatim, unquoted. For complex strings, a C-style string literal can be supplied as an argument to commands. A string literal is a sequence of characters enclosed by two ``"``s. To escape a ``"`` in a string literal use ``\"``. Likewise, a ``\`` can be escaped by using  ``\\``. Rather than splitting on the spaces, the entire string will be given as one argument. This is useful when you want to pass a command an argument with spaces in it.

Comments
--------

Totem Script supports C-style comments

.. code-block:: c

   //

Single line comment. Ignore all characters until the end of the line.

.. code-block:: c

   /*

Begin multi-line comment. Ignore all characters until the end multi-line comment sequence.

.. code-block:: c

   */

End multi-line comment.

Arguments
---------

.. code-block:: c

   %index

When a script file is invoked with the ``BSource`` command, tokens of the form ``%index`` will be replaced with the argument to ``BSource`` at index ``index``. For example if the command-line ``BSource UserGame.tsc Story Story`` was run, ``%0`` would be replaced with the name of the script (``UserGame.tsc``) and ``%1`` would be replace with the first argument to ``BSource`` following the script name (``Story``), and so on. Nested calls to ``BSource`` are handled appropriately using a stack.

Limitations
-----------

The number of usable characters in a buffer is ``1`` less than the length of the buffer to leave room for the null terminator. The buffer length for each line is 2048 characters. The buffer length for the command name is 1024 characters. The buffer length for the short form command name is 16 characters. Registered commands are stored in a linked list and thus there is no limitation on the number of registered commands. The buffer length for each argument is 1024 characters. The buffer length for variable names is 32 characters. There can be at most 32 variables set at any one time. A command can have at most 32 arguments including the name. Since integer and float arguments are backed by 32-bit floating point values they are subject to the limitations imposed the underlying data type.

Additionally, an eagle eyed reader will have noticed that Totem Script only supports sequential and conditional execution, and not iterative execution. This means that there are no loops, so if you want to run a command multiple times then you need to copy and paste it that many times or fall back to a higher level environment and execute the commands from there. You will almost never need to do this so it is an excusable omission from the language.
