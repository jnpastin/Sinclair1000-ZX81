'
'Load the machine code into memory and execute it.  This is easier than poking it.
'
10 LET X=PEEK 16388+256*PEEK 16389+1	'Start loading the program at the new RAMTOP + 1.
20 LET Y=X				'Keep this value to run the machine code from, this is faster than recalculating it.
30 LET A$="3E2DD73E2AD73E31D7D73E34D73E37D73E31D73E29D73E1BD7C9S"
					'Declare the string of machine code in HEX with an "S" as a terminator (not a valid HEX character).
40 IF A$="" THEN INPUT A$		'This should never evaluate as true, just in to catch any errors and provide an opportunity to break the loop.
50 IF A$="S" THEN GOTO 100		'Jump out of the loop when the terminator is caught.
60 POKE X,16*CODE A$+CODE A$(2)-476	'Poke the two digit HEX value as a decimal into the memory location at X. 476=(16*28)+28, with 28 being the character offset of "0".
70 LET X=X+1				'Increment the address counter.
80 LET A$=A$(3 TO )			'Drop off the first two characters of the string
90 GOTO 40				'Iterate the loop
100 RAND USR Y				'Execute the machine code
