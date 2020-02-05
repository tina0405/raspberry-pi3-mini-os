# raspberry-pi3-mini-os
* goal: build a component-based os. Make kernel only have basic services. Added Extra functions by inserting component.

* shell command:
  1. ls: list file and directory.
  2. cd [directory]: enter the directory.
  3. cd : enter the root.
  4. incom [file name]: insert component
  5. rmcom [component's name]: remove component
  6. swap [file name] [component's name]: swap new_file to old component
  7. lscom: list component
  8. run: run application

* Document: https://hackmd.io/s/S1Ab0A9pX#
* Reference:
1. [Learning operating system development using Linux kernel and Raspberry Pi](https://github.com/s-matyukevich/raspberry-pi-os)
2. [Driver reference](https://github.com/bztsrc/raspi3-tutorial)
