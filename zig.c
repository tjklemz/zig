/***************************************************************************
 * Write data through zigbee.
 *
 * Software Junior Project
 * Jeffrey Daniels
 * Spring 2013
 ***************************************************************************/

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int fd_t;

fd_t open_zigbee()
{
	struct termios attribs;
	fd_t zig;

	// open the device to write to
	zig = open("/dev/ttyAMA0", O_RDWR | O_NDELAY | O_NOCTTY);
	
	// get any default attributes of that device
	tcgetattr(zig, &attribs);

	// override any attributes we need (e.g. baud rate, char size)
	attribs.c_cflag = (CS8 | CREAD | CLOCAL | B9600);
	attribs.c_oflag = 0;
	attribs.c_lflag = 0;
	attribs.c_iflag = (IGNPAR | ICRNL);
	
	tcflush(zig, TCIFLUSH);

	// set those suckers!
	tcsetattr(zig, TCSANOW, &attribs);

	return zig;
}

int main()
{
	fd_t zig;
	int amt;
	char * str = "hello, world!";
	char * gotit = "GOT IT!";
	char * stop = "stop";
	char buf[256] = {0};

	zig = open_zigbee();

	for(;;) {	
		// now actually do work (send a string/data)
		write(zig, str, strlen(str));
		amt = read(zig, buf, 255);
		if(amt > 0) {
			buf[amt] = 0;
			if(strcmp(buf, stop) == 0) {
				write(zig, gotit, strlen(gotit));
				break;
			}
		}
		usleep(500000);
	}

	// make sure to close the file
	close(zig);

	return 0;
}

