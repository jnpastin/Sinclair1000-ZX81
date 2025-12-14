'
'Load the machine code into memory and execute it.  This is easier than poking it.
'
10 LET X=18322				'Start loading the program at the ORG defined in the assembly
20 LET Y=X				'Keep this value to run the machine code from, this is faster than recalculating it.
30 LET A$="3A0C404F3A0D40473A10406F3A114067ED427DD619C2B24779DE0C4FD2B24705606911CC471AFEFFCAC347020313C3B7477D320C407C320D40C976762D2A3131343731291B76FFS"
					'Declare the string of machine code in HEX with an "S" as a terminator (not a valid HEX character).
40 IF A$="" THEN INPUT A$		'This should never evaluate as true, just in to catch any errors and provide an opportunity to break the loop.
50 IF A$="S" THEN GOTO 100		'Jump out of the loop when the terminator character is caught
60 POKE X,16*CODE A$+CODE A$(2)-476	'Poke the two digit HEX value as a decimal into the memory location at X. 476=(16*28)+28, with 28 being the character offset of "0".
70 LET X=X+1				'Increment the address counter.
80 LET A$=A$(3 TO )			'Drop off the first two characters of the string
90 GOTO 40				'Iterate the loop
100 RAND USR Y				'Execute the machine code
