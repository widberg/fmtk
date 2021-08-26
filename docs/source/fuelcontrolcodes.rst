Control Codes
=============

Control codes for translation text and other strings.

Some strings accept C standard library ``printf`` format specifiers.

General Control Codes
---------------------

.. code-block:: c

   ~

Newline.

.. code-block:: c

   ²

Space.

.. code-block:: c

   ^

``^`` followed by 3 digits, ``\^\d{3}``, will set the color. ``^RGB`` will result in ``Color(R / 9 * 255, G / 9 * 255, B / 9 * 255)``. The one exception to this rule is ``^000`` which resets the color to the default, white. Since ``^000`` is a special case, you must use ``^001`` to achieve the closest thing to the color black. Advanced note: Characters outside of the ``0-9`` range are allowed; however, the ASCII value of the characters are used so technically ``^XYZ``, ``\^.{3}``, will result in ``Color(std::clamp((X - '0') / 9 * 255, 0, 255), std::clamp((Y - '0') / 9 * 255, 0, 255), std::clamp((Z - '0') / 9 * 255, 0, 255))``.

.. code-block:: c

   §

``§`` followed by 3 digits, ``\§\d{3}``, will set the text size. ``§HXY`` can be interpreted as a one digit header value followed by a two digit size value. When the header is zero, the size value may range from 0, text is so small it is not visible, to 99, the largest size. When the header value is non-zero, the text is first scaled inversely to the header value, 1 is the largest and 9 is the smallest, then it is scaled again inversely by the size value, 0 is the largest size for that header value and 99 is the smallest size for that header value. Advanced note: the same behavior for non-digit characters applies as above.

.. code-block:: c

   µ

Right justify. Align the text to the right side of the panel and use weird indent on new-line behavior. If a line in a string with ``µ`` in it does not have ``µ`` in it, it will be indented. The last line will always be indented.

.. code-block:: c

   £

Sets some one time thing related to the width of the rest of the string or something. I really have no idea what this does.

Trans Text Specific Control Codes
---------------------------------

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

The two variants of this control code are ``BITMAP_GAMESPY`` and ``BITMAP_BINK`` although ``BITMAP_BINK`` is recognized by not being ``BITMAP_GAMESPY`` rather than being an explicit check for that value. This control code is used to display the logo of the respective entities during the credits.

.. code-block:: c

   END OF CREDITS

When the full string is ``END OF CREDITS``, the credits end.
