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

Encountered when `res/imgs/icon2.png` is not found. This may be extended to all of the other used assets, however I am too lazy to implement that many checks right now.

Because all of the assets are gone, the GUI BSoD will not output _anything_. Therefore, the game will use `std::cout` instead, so try running the game through the terminal if it decides not to open.

0x4 - Invalid Scale Factor
--------------------------

Encountered when the scale factor variable is set to a value outside of the intended range. This is because the resolutions are hardcoded<sup>1</sup>. This error should also not be encountered during normal operation.

0x5 - Invalid Player ID
-----------------------

Encountered whenn the player ID variable is set to a value outside of the intended range. Right now, encountering this makes sense as one of the characters is unimplemented. However, once this changes, you should not be able to encounter this as the data type used is `bool`.

<sup>1</sup>Subject to change.
