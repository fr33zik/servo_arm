This project contain source files to a video with servo arm controlled by rpi,fitkit

link:
https://youtu.be/zP1TQnDVWzc


raspberry pi
sources/rpi
demo -- "demo_*" in same folder and runs one based on inuput from pins(buttons)
	--moves arm in a patern
	--rpi_gpio -- only sets frequency jitter example
*tst -- gpio can control arm with buttons
     -- pigpio test for osciloscope

fitkit
sources/fitkit

These are not compilabe on they own.
They are top architecture for FPGA and main.c for MCU, these are two different projects.

fpga
For use create new project and paste contents to its root

mcu
For use create new project and repaste the code inside main.c.

needed resources can be found on http://merlin.fit.vutbr.cz/FITkit/
under SVN and/or Download sections