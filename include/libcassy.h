#ifndef LIBCASSY_H
#define LIBCASSY_H

//
// includes
//

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <time.h> // only for tm struct

#ifdef CA_LOCAL

#include <string.h>

#ifdef CA_LIBUSB
#include <libusb.h>
#endif

#ifdef CA_DEVNODES
#include <unistd.h>
#include <fcntl.h>
#include <linux/serial.h>
#include <sys/ioctl.h>
#include <termios.h>
#endif

#endif

//
// constants
//

#include "ca_constants.h"

//
// datatypes
//

#include "ca_datatypes.h"

//
// declarations
//

#include "ca_declarations.h"

#endif
