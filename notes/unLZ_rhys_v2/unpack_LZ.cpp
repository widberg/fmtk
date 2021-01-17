
// sub_5FB6E0
byte* __thiscall unpack_LZ( _DWORD* this, byte* _inpptr, byte* _outptr ) {
  byte *outptr; // ebp
  byte *inpptr; // eax
  int lenB3; // ecx
  int lenB2; // edx
  byte *inpptr1; // eax
  int lenB23; // ecx
  int lenB1; // edx
  int lenbits; // ecx
  int _lenbits; // edx
  char v12; // bl
  int v13; // edi
  int v14; // edx
  unsigned __int16 v15; // cx
  int v16; // edx
  int match_distance; // esi
  int match_len; // edx
  byte *v19; // edi
  bool v20; // cc
  int v21; // [esp+10h] [ebp-Ch]
  byte *endptr; // [esp+14h] [ebp-8h]
  _DWORD *_this; // [esp+18h] [ebp-4h]
  byte* v24; // [esp+20h] [ebp+4h]
  int __lenbits; // [esp+24h] [ebp+8h]

  outptr = _outptr;
  endptr = _outptr + this[1];
  _this = this;
  inpptr = _inpptr + 8;

ReadLenBits:
  lenB3 = *inpptr;
  lenB2 = inpptr[1];
  inpptr1 = inpptr + 1;
  lenB23 = lenB2 + (lenB3 << 8);
  lenB1 = *++inpptr1;
  lenbits = inpptr1[1] + ((lenB1 + (lenB23 << 8)) << 8);
  inpptr = inpptr1 + 2;

  _lenbits = lenbits;
  __lenbits = _lenbits;

  lenbits = lenbits & 3;
  v12 = 14 - lenbits;
  mask = 0x3FFF >> lenbits;
  mask0 = mask;

  v24 = 0;

  while(1) {

    if( _lenbits >= 0 ) {

      // copy literal
      *outptr++ = *inpptr++;

    } else {

      // copy match
      v15 = inpptr[0]<<8;
      v14 = inpptr[1];
      inpptr += 2;
      v16 = v14 | v15;
      match_distance = (v16 & mask) + 1;
      match_len = (v16 >> v12) + 3;
      if( match_len ) {
        match_ptr = outptr - match_distance;
        do{ --match_len; *outptr++ = *match_ptr++; } while ( match_len );
        mask = mask0;
      }

    }

    if( endptr == outptr || *((_BYTE *)_this + 13) && outptr > inpptr ) return inpptr;

    _lenbits = 2 * __lenbits;
    __lenbits *= 2;

    ++v24;
    if( v24>=30 ) goto ReadLenBits;
  }
}