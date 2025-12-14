'
'Set RAMTOP
'This is used to reserve space at the top of memory for the machine code to live.  This value will differ depending on the amount of RAM in the machine, so it needs to be calculated.
'
10 LET X=PEEK 16388+256*PEEK 16389	'Read the current value of RAMTOP.
20 LET X=X-100				'Reserve 100 bytes by reducing RAMTOP by 100.
30 LET LOW=X-INT (X/256)*256		'Get the low byte value of the new RAMTOP.  This is the remainder of the modulo of the new value by 256.
40 LET HIGH=INT (x/256)			'Get the high byte value of the new RAMTOP.  This is the integer portion of the new value divided by 256.
50 POKE 16388,LOW			'Set the new RAMTOP
60 POKE 16389,HIGH
70 NEW					'Start a new program.  RAMTOP will persist the NEW operation

