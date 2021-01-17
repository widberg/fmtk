@echo off

unLZ.exe 00000014.dat 00000014.unp

unLZ.exe 0000053a.dat 0000053a.unp

getbytes.exe 0000003c.xml 0 64
unLZ.exe 0 0000003c.unp

