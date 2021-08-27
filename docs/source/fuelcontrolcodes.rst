Control Codes
=============

Control codes for TransText and other strings.

Some strings accept C standard library ``printf`` format specifiers.

General Control Codes
---------------------

.. code-block:: c

   ~

Newline.

.. code-block:: c

   ²

em space. Invisible non-whitespace character with a width of approximately 1 space, `` ``, character scaled to the current font size. Useful for when you want to print a space but cannot have ``char c`` such that ``std::isspace(c) == true``.

.. code-block:: c

   |

Zero-width space. Invisible non-whitespace character with a zero width. This may have other uses.

.. code-block:: c

   £

1px indent. Invisible non-whitespace character with a width of approximately 1 pixel. Regardless of where in the line ``£`` occurs, the space will be applied to the beginning of the line.

.. code-block:: c

   ^

``^`` followed by 3 digits, ``\^\d{3}``, will set the color. ``^RGB`` will result in ``Color(R / 9 * 255, G / 9 * 255, B / 9 * 255)``. The one exception to this rule is ``^000`` which resets the color to the default, white. Since ``^000`` is a special case, you must use ``^001`` to achieve the closest thing to the color black. Advanced note: Characters outside of the ``0-9`` range are allowed; however, the ASCII value of the characters are used so technically ``^XYZ``, ``\^.{3}``, will result in ``Color(std::clamp((X - '0') / 9 * 255, 0, 255), std::clamp((Y - '0') / 9 * 255, 0, 255), std::clamp((Z - '0') / 9 * 255, 0, 255))``.

.. code-block:: c

   §

``§`` followed by 3 digits, ``\§\d{3}``, will set the text size. ``§HXY`` can be interpreted as a one digit header,  ``H``, value followed by a two digit size value, ``XY``. When the header is zero, the size value may range from 0, text is so small it is not visible, to 99, the largest size. When the header value is non-zero, the text is first scaled inversely to the header value, 1 is the largest and 9 is the smallest, then it is scaled again inversely by the size value, 0 is the largest size for that header value and 99 is the smallest size for that header value. Advanced note: the same behavior for non-digit characters applies as with ``^``.

.. code-block:: c

   µ

Right justify. Following the first occurrence of ``µ`` in the string, align the text to the right side of the panel and use weird "indent on new-line" behavior. If a line following the first occurrence of ``µ`` in the string has characters before the first occurrence of ``µ`` in the line, those characters will be indented. If a line following the first occurrence of ``µ`` in the string has characters after the first occurrence of ``µ`` in the line, those characters will not be indented regardless of if indented characters exist previously on the line. This can lead to weird overlapping text on the same line with the right combination of ``~`` and ``µ`` characters. The last line will always be indented if there exists a line containing ``µ`` before it regardless of ``µ`` being present in the last line.

TransText Specific Control Codes
--------------------------------

The following control codes only have an effect when used in TransText strings.

.. code-block:: c

   STR_

The full sequence of ``STR_\S+``, where the part following ``STR_`` is a registered ``LanguageHandleSTR``, is replaced with the registered substitute. The substitute may be text or a bitmap. This is mainly used to insert button icons into TransText strings.

Credits Specific Control Codes
------------------------------

The following control codes only have an effect during the credits sequence.

.. code-block:: c

   $

When ``$`` is the only character in a string, the string is empty. This is used during the credits to add padding between sections.


.. code-block:: c

   BITMAP_

The two observed variants of this control code are ``BITMAP_GAMESPY`` and ``BITMAP_BINK``. This control code is used to display the logo of the respective entities during the credits.

.. code-block:: c

   END OF CREDITS

When the full string is ``END OF CREDITS``, the credits end.

Multibyte Characters
---------------------

Some strings are interpreted as 8-bit Extended ASCII (EASCII) while others are interpreted as UTF-8. If the control codes listed above are not working, it is possible that the string is expecting the character as UTF-8 and not EASCII. For example, ``§`` is ``\xA7`` in EASCII and ``\xC2\xA7`` in UTF-8. You may find `this table <https://kellykjones.tripod.com/webtools/ascii_utf8_table.html>`_ to be a useful reference for ASCII and UTF-8 comparisons.
