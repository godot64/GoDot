# GoDot
GoDot C64 Image Processing

![GoDot GUI](http://www.godot64.de/german/pictures/godot2017.gif "GoDot Main Screen")

## What is it?

If you like you can say **GoDot** is something like a Photoshop for the Commodore 64, a *SixtyPhourtoshop*, the only one on the Commodore, by the way. GoDot is a modular system which consists of **loaders** (for particular graphics file formats), **savers** (for particular graphics file formats, so you can convert formats one into another, see the [Table of Supported Formats][Conversions Table]), and **modifiers** (which do what their typename says, they *modify* the loaded data, so you can manipulate images in many, many ways using GoDot; see our [Table of Modifiers][Modifiers]).

## How to use it?

GoDot has a **mouse driven GUI** (can also be controlled by joystick or keyboard) which was derived from Amiga's Art Department Professional II (another Photoshop-like program). On this [page][Character Set] you can see how this was achieved. It runs on any device that is able to work like a C-64 (which includes emulators like [VICE][Emulator]).

## What do you need?

You need nothing more than **a stock Commodore 64** and a disk drive. Of course, the more peripheral hardware you have, the better. The best performance can be achieved when you use an REU RAM extension together with three or four (CMD) disk drives or even a SuperCPU. GoDot also supports graphics delivering devices like the Scanntronik Handyscanner or several video digitizers (see an [overview][Graphics Hardware]).

## How do you get it?

All source code files available from here are written using the ACME assembler. So first you have to download ACME and probably a facility to edit, run and thus test out the code you write, like Relaunch64. Following [this link][ACME] you will have both of them.

Then just clone this GoDot repository to your computer, add a path to ACME (and to VICE if you want to use an emulator), add a MAKE utility to your system ([Make for Windows][Make Link] if you run Windows), and run the Makefile in this repository.

You have several options:
- **make d81**<br>
  will build a D81 disk image filled with all necessary GoDot modules to have a running system, the D81 is placed in the build folder of the repository.
- **make files**<br>
  will let ACME assemble the repository files into the build folder, resulting in all GoDot modules, ready to transfer them into two or more D64 disk images of your wishes.
- **make start**<br>
  will assemble the whole repository into one D81 disk image, run it in VICE, and autostart GoDot.
- **make clean**<br>
  tidy up the build folder.
- **make rebuild**<br>
  will end up in a running GoDot on VICE, just like "make start", but doesn't care if you have previously used MAKE already.

## How to GoDot?

Go here: http://www.godot64.de, GoDot's website. It's German, but Google Translate will do. You'll find a most elaborate [manual][Manual] of GoDot's modifiers there, and some intensive [tutorials][Tutorials].

**Have fun using GoDot!**

Arndt

---
[Conversions Table]: http://www.godot64.de/german/formats.htm "GoDot's list of supported file formats"
[Modifiers]: http://www.godot64.de/german/m_allg.htm "GoDot's Standard Modifiers (there are also a number of more specific modifiers)"
[Character Set]: http://www.godot64.de/german/godset.htm "GoDot's origins"
[Graphics Hardware]: http://www.godot64.de/german/l_bdata.htm "Graphics Hardware"
[Emulator]: http://vice-emu.sourceforge.net/ "VICE - the Versatile Commodore Emulator"
[ACME]: http://www.emu64-projekt.de/acme/ "Download of ACME und Relaunch64"
[Make Link]: http://gnuwin32.sourceforge.net/packages/make.htm "GNU Make for Windows"
[Manual]: http://www.godot64.de/german/index.htm "GoDot Manual: How to GoDot"
[Tutorials]: http://www.godot64.de/german/tutorials.htm "GoDot Tutorials"
