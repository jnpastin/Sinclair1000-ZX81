'
'Set RAMTOP
' This is used to reserve space at the top of memory for the machine code to live.  
' This value will differ depending on the amount of RAM in the machine, and the assembly needs an explicit ORG in order to work.
' For my 2K machine, I specified the ORG at 18322d or 4792h.  Machines with higher capacity will need to adjust accordingly.
' Either way, set RAMTOP just below the ORG
'
10 LET X=18320				'Define the new RAMTOP
20 LET LOW=X-INT (X/256)*256		'Get the low byte value of the new RAMTOP.  This is the result of the modulo of the new value by 256.
30 LET HIGH=INT (x/256)			'Get the high byte value of the new RAMTOP.  This is the integer portion of the new value divided by 256.
40 POKE 16388,LOW			'Set the new RAMTOP
50 POKE 16389,HIGH
60 NEW					'Start a new program.  RAMTOP will persist the NEW operation

