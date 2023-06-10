//$ symbol fuel::GetInputState, "LPCVOID (__stdcall *)(int a)";
//$ address fuel::GetInputState, [retail], @0x00559EC0;

//$ symbol fuel::GetKeyState, "KeyState *(__userpurge *)@<eax>(LPCVOID pState@<eax>, short key_code)";
//$ address fuel::GetKeyState, [retail], @0x00419C60;

KeyState *GetKeyState(short key_code)
{
	return real_fuel_GetKeyState(real_fuel_GetInputState(0), key_code);
}

// Controls are 2 packed 16-bit ints. The high order short is the dik code and the low short is flags (0xFFFD -3 or less disabled) (0xFFFF -1 enabled dik) (0xFFFE -2 Something maybe controller button/trigger) (0 or greater does something more complicated maybe controller joystick)

// Control.XX.YY.Primary/Secondary
// XX [0, NbContext)
// YY [0, 32)
// Not all used. Room for custom controls

// Control.
//   00.
//     00.
//       Primary A 0
//       Secondary 0x0000FFFD
//     01.
//       Primary D 1
//       Secondary 0x0000FFFD 
//     02.
//       Primary W 2
//       Secondary 0x0000FFFD
//     03.
//       Primary S 3
//       Secondary 0x0000FFFD
//     04.
//       Primary Left 4
//       Secondary 0x0000FFFD
//     05.
//       Primary Right 5
//       Secondary 0x0000FFFD
//     06.
//       Primary Up 6
//       Secondary 0x0000FFFD
//     07.
//       Primary Down 7
//       Secondary 0x0000FFFD
//     08.
//       Primary NUM 1 8
//       Secondary 0x0000FFFD
//     09.
//       Primary Escape 9
//       Secondary 0x0000FFFD
//     10.
//       Primary R 10
//       Secondary 0x0000FFFD
//     11.
//       Primary Return 11
//       Secondary 0x0000FFFD
//     12.
//       Primary Space 12
//       Secondary 0x0000FFFD
//     13.
//       Primary  L Control 13
//       Secondary 0x0000FFFD
//     14.
//       Primary NUM 0 14
//       Secondary 0x0000FFFD
//     15.
//       Primary F 15
//       Secondary 0x0000FFFD
//     16.
//       Primary E 16
//       Secondary 0x0000FFFD
//   01.
//     00.
//       Primary S 18
//       Secondary 0x0000FFFD
//     01.
//       Primary A 19
//       Secondary 0x0000FFFD
//     02.
//       Primary W 20
//       Secondary 0x0000FFFD
//     03.
//       Primary D 21
//       Secondary 0x0000FFFD
//     04.
//       Primary T 22
//       Secondary 0x0000FFFD
//     05.
//       Primary Y 23
//       Secondary 0x0000FFFD
//     06.
//       Primary U 24
//       Secondary 0x0000FFFD
//     07.
//       Primary I 25
//       Secondary 0x0000FFFD
//   02.
//     00.
//       Primary Up 27
//       Secondary 0x0000FFFD
//     01.
//       Primary Down 28
//       Secondary 0x0000FFFD
//     02.
//       Primary Right 29
//       Secondary 0x0000FFFD
//     03.
//       Primary Left 30
//       Secondary 0x0000FFFD
//     04.
//       Primary Return 31
//       Secondary 0x0000FFFD
//     05.
//       Primary E 32
//       Secondary 0x0000FFFD
//     06.
//       Primary Escape 33
//       Secondary 0x0000FFFD
//     07.
//       Primary T 34
//       Secondary 0x0000FFFD
//     08.
//       Primary Y 35
//       Secondary 0x0000FFFD
//     09.
//       Primary U 36
//       Secondary 0x0000FFFD
//     10.
//       Primary I 37
//       Secondary 0x0000FFFD
//     11.
//       Primary Z 38
//       Secondary 0x0000FFFD
//     12.
//       Primary X 39
//       Secondary 0x0000FFFD
//     13.
//       Primary H 40
//       Secondary 0x0000FFFD
//     14.
//       Primary C 41
//       Secondary 0x0000FFFD
//     15.
//       Primary Back 42
//       Secondary 0x0000FFFD
//     16.
//       Primary R 43
//       Secondary 0x0000FFFD
