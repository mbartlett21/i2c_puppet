# I2C Puppet mods for Linux systems


This is a modification to the standard firmware.

The original version lacked certain characters, such as \<ESCAPE>, and the characters "<>{}[]^&%=\"
which made it difficult to use on Linux systems


This version of the firmware has been modified to support Linux systems.
The following changes have been made
- The backspace key works during the GUI login
- The Sym key now acts as a Control key, so SYM+C is Control-C
- The four top button keys are now used to provide the missing characters.

The original definiton of the keys were this. I'm including the
spacebar, newline/enter, backspace, Microphone and Spkeaer keys
because they provide characters not indicated on the key. For
instance, ALT + spacebar provides TAB, and ALT+NewLine is the pipe character.


|       | L1 | L2 | R1 | R2 | SPKR | Mic | BS | NL | SPACE |
|-------|----|----|----|----|------|-----|----|----|-------|
| none  |    |    |    |    |  $   |  ~  | \b | \n | SPACE |
| Alt   |    |    |    |    |  \`  |  0  |    | \| |  TAB  |
| Shift |    |    |    |    |  $   |     |    |    |       |
| Sym   |    |    |    |    |  $   |     |    |    |       |



This firmware adds the following mappings


|       | L1 | L2 | R1 | R2 | SPKR | Mic | BS | NL | SPACE |
|-------|----|----|----|----|------|-----|----|----|-------|
| none  |ESC | %  | =  | \\ |  $   |  ~  | \b | \n | SPACE |
| Alt   | >  | ]  | }  | &  |  \`  |  0  |    | \| |  TAB  |
| Shift | <  | [  | {  | ^  |  $   |  ~  | \b |    |       |
| Sym   | x  | x  | x  | x  |  $   |  ~  | \b |    |       |



I tried to make the bracket characters easierr to remember by using
Shift+ to indicate the left-pointing brackets, and Alt+ corresponds to
the same right-pointing bracket. You can redefine these keys if you
prefer a different arraingment.

Currently, SYM+\<top button keys> produces an 'x' to indicate some value can be inserted.
I'm considering mapping these to the 4 arrow keys


## Linux Debug tips

The keyboard has two "outputs" - one is the USB HID interface, the
other is the serial port.  Any printf() command goes to the serial
port, but not the USB HID keyboard. For instance, when using an
Arduino sketch, print() goes to the serial port.

When the keyboard is plugged into a Linux system, a new TTY interface
will appear. I usually use

	ls -lt /dev/tty* | head

to learn the name, as the newest port will appear first. On my system,
it's /dev/ttyACM0

So on one terminal window, I type

	cat -v </dev/ttyACM0

while on a second terminal window, I type

	cat -v

The first one will print all of the printf output, and the second will
show you how the keyboard works normally and what gets sent to the USB
keyboard when a key is pressed..

## Compiling firmware on Linux

I edit the files in \<GIT>/ic2_puppet/all/ using emacs.
I have the keystroke combination
"Control-C M" bound to compile, using

	(global-set-key "\C-cm" 'compile)

And when I press "Control-c M", emacs saves all files, and recompiles the
code. I have a small hub with switchable on/off ports, and restart the
keyboard into boot mode, and then do a "make install" to load the new
firmware. But you can also use vim. The makefile assumes the compiled version is in ../build.

## GUI Login

I found that I had to handle the backspace differently, as arturo did
for enter. This was necessary so that the backspace key would delete
the username or password characters.

## TODO

Currently - the SYM+Button keys are defined as the character "x" to indicate it's not been specified.
I'l like to make these keys to the 4 arrow keys.

Also - it might be possible to create key combinations by combining the modified keys, like SYM+Alt+key
