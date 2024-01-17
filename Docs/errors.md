Error Codes
===========

0x0 - Generic Error
-------------------

Exactly what it says on the tin. Used as a placeholder error, if ever.

0x1 - Missing Function Error
----------------------------

Encountered when the user attempts to access a feature that is unimplemented. Should not be encountered very often.

0x2 - Invalid Level ID
----------------------

Encountered when the level ID variable is set to a value outside of the intended range. This error should not be encountered during normal operation.

Since this is a PC game, this error is here to stay, unfortunately.

0x3 - Missing Assets Error
--------------------------

Encountered when any game resource is missing.

Because all of the assets are gone, the GUI BSoD will not output _anything_. Therefore, the game will use `std::cout` instead, so try running the game through the terminal if it decides not to open, or randomly closes.

0x4 - Invalid Scale Factor
--------------------------

Encountered when the scale factor variable is set to a value outside of the intended range. This is because the resolutions are hardcoded<sup>1</sup>. This error should also not be encountered during normal operation.

0x5 - Invalid Player ID
-----------------------

Encountered whenn the player ID variable is set to a value outside of the intended range. Right now, encountering this makes sense as one of the characters is unimplemented. However, once this changes, you should not be able to encounter this as the data type used is `bool`.

0x6 - Invalid Save Checksum
---------------------------

Encountered when the save file checksum and expected checksum does not match.

This can be for a variety of reasons, including corruption and intentional hex-editing. For those who intentionally manipulate save files, the corrected checksum is also displayed. Otherwise, you may have to start fresh.

This error is also displayed through `std::cout`, and only when the program is starting up, so consider running the game through the terminal if it refuses to start.

0x7 - Invalid Save File Version
-------------------------------

Encountered when the expected save file version and the recieved version do not match.

The game currently is not aiming to maintain backwards compatibility<sup>1</sup>.

<sup>1</sup>Subject to change.
