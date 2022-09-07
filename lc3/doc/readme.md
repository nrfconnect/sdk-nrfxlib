# T2 SOFTWARE LC3

Integration and distribution information for T2 Software, Inc.'s LC3 codec library.

## OVERVIEW:

This distribution provides a means for integrating into your application, as well as an application for using the LC3 codec on a desktop computer.

## DIRECTORIES:

- apps
   - Demo/utility applications
- codec
   - Library and header for use in your application
- doc
   - API documentation
- platform
   - Platform abstraction layer

## INTEGRATION:

The LC3 library can be integrated into your application via the following steps:

1. Port memory functions to your platform if needed
2. Update your project with the necessary files

### PLATFORM

The directory "platform/os/baremetal" contains an abstraction layer that uses the stdlib heap functions (malloc/free).

If you need to use a different heap, then create a new directory platform/os/[PLATFORM] where [PLATFORM] is the name of your platform and modify the functions as needed.

If you will be providing your own buffers directly to the LC3 APIs, then the heap functions will not be called. You may stub these platform functions if malloc() and free() are not present in your project in that case.

### BUILD

The following are the necessary components for integrating LC3 into your project:

- Link
	- codec/[CODEC_TYPE]/lib/[PLATFORM]libLC3.a
- Header file
	- codec/inc/LC3API.h
- Include directories
	- platform/os/inc
	- platform/os/[PLATFORM]/inc
- C Files
	- platform/os/[PLATFORM]/src/osal.c

Also ensure you are using the appropriate compiler flags for either hard or soft float and that the corresponding math library is linked.

## APPLICATION:

### T2_LC3

T2_LC3 is an application that allows you to use the LC3 codec on a desktop computer. It is able to both encode WAV files into a binary format, and decode binary files back into WAV files.

Builds of T2_LC3 are provided for PC.

#### Usage

Usage: [mode] [options] [input] [output] [bitrate]

Where [mode] is:

- -E for encode mode.
- -D for decode mode.
- Omitted for encode/decode mode (encode, then decode).

Where [options] are all optional and may include:

- -bps [bit depth]
	- Sets the output bit depth, defaults to 16 bits. 16, 24, or 32 are allowed.
	- Valid in encode/decode or decode mode.

- -frame_ms [value]
	- Set the frame size, defaults to 10 msec.  10 - 10 msec; 7.5 - 7.5 msec.

- -swf [file]
	- Uses a variable bit rate pattern read from the given file. The file format is binary with every two little endian bytes representing the total number of bytes per second for all channels. The pattern defined by the file is repeated when the codec reaches the end of the file. Valid in encode or encode/decode mode.

- -epf [file]
	- Uses a pattern file designating lost frames. The file format is binary with every non-zero 16 bit value indicating a frame loss. The pattern defined by the file is repeated when the codec reaches the end of the file. Valid in decode or encode/decode mode.

- -edf [file]
	- Creates a frame loss pattern file by capturing the decode frame results. The file format is binary with every non-zero 16 bit value indicating a detected errored. Valid in decode or encode/decode mode.

- -dc [value]
	- Enables delay compensation by removing the first 1/4 frame of the decoder output when the value is set to 1. Disables delay compensation by including the first 1/4 frame of the decoder output when the value is set to 0. Default value is enabled.

Where [input] is:

- A WAV file in encode or encode/decode mode.
- A binary file in decode mode.

Where [output] is:

- A binary file in encode mode.
- A WAV file in decode or encode/decode mode.

Where [bitrate] is:

- Bits per second in encode or encode/decode mode.
- Ignored in decode mode.

#### Example Commands

An example encode command is:

- T2_LC3 -E input.wav tmp.bin 80000

An example decode command is:

- T2_LC3 -D tmp.bin output.wav

An example encode/decode command (10 msec frame) is:

- T2_LC3 input.wav output.wav 80000

An example encode/decode command (7.5 msec frame) is:

- T2_LC3 -frame\_ms 7.5 input.wav output.wav 80000

An example encode/decode command with a specified output bit depth is:

- T2_LC3 -bps 24 input.wav output.wav 80000

An example encode/decode command with a specified output bit depth and delay compensation disabled is:

- T2_LC3 -bps 24 -dc 0 input.wav output.wav 80000