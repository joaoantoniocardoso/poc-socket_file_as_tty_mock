# poc: socket file as tty mock

Suppose an external electronics hardware is connected to your computer using a USB-Serial converter as `/dev/ttyACM0`. 
By connecting to this tty with a serial communication program like the [minicom](https://linux.die.net/man/1/minicom), a communication (bidirectional) with the hardware CLI starts.

If the embedded firmware is now running on the host machine as a simulator, how can we mock the same interaction using a terminal emulator?

This code is a proof-of-concept exploring a simple solution using a stream socket (TCP) binded to a file, accessible in a similar fashion using minicom.

Another possible solution is to create a [pty](https://man7.org/linux/man-pages/man7/pty.7.html), but the socket is a way more popular/portable/maintainable interface.
