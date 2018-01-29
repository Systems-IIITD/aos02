#ifndef MAIN_H
#define MAIN_H

#include <linux/ioctl.h>

#define MODULE_NAME "aos"
#define MAJOR_NUM 100

#define REGISTER_HANDLER     _IOR(MAJOR_NUM, 1, int)
#define UNREGISTER_HANDLER   _IOR(MAJOR_NUM, 2, int)

#endif
