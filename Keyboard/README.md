# External Keyboard for Timex Sinclair 1000 and Sinclair ZX81 computers
It is well known how awful the keyboard on these machines is.  It was fairly common back in the day for people to modify their machines to connect an external keyboard.  I decided to follow in that tradition, but with a more modern approach.  There were a few design requirements:
* Should physically resemble the factory keyboard as much as reasonably possible
* Needs to be a comfortable size to use with two hands
* Needs to have a good tactile feel
* Only requires readily available components (with the exception of the board)
* Requires no case modification
* Requires only reversible modification(s) to the board
* Doesn't use the edge connector in the back

The last requirement was a personal choice.  It would have been easy to create a board that connected to the edge connector in the rear, but with the printer and the 16K RAM pack, things were already getting pretty long, and I was concerned about the leverage such a long chain could exert on the system board.  Plus finding stock on a dual row, 46 position (2x23) card edge connector can be pretty challenging.  It is an oddball size and not readily stocked by the usual suppliers.

In the end I designed a board that matches the pinout of the two connectors on the system board.  The idea is to solder a ribbon cable to the back of each and run it out the expansion card slot.  The schematic of the PCB shows that the circuit is identical to the one used by the membrane keyboard.  This allows for a straightforward interface between the system and the keyboard.  Additionally, The PCB is designed to use any widely available 5 pin MX keyswitch.  These can be found from any number of suppliers.  I found some keycaps that have a clear overlay on them, this allows for customization to match the original layout.  The keycap insert files have both the US and UK keys in them, so only print out the ones that you need.  Chances are that the PDF is the one that you want, but I included the PPT in case someone wanted to modify them.  The PCB has two connectors that correspond to the ones on the system board, they are intended to be JST XH connectors.

All of the necessary files should be available here, including:
* Schematic
* PCB layout
* Gerbers
* PDF of the keycap inserts
* PPT of the keycap inserts

Once I finish designing the case, I will include STEP and STL files for it here as well.
