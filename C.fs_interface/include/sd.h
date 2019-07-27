/*
 * Copyright (C) 2018 bzt (bztsrc@github)
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 */


#define SD_OK		        0							// NO error
#define	SD_ERROR		1							// General non specific SD error
#define	SD_TIMEOUT		2							// SD Timeout error
#define	SD_BUSY			3							// SD Card is busy
#define	SD_NO_RESP		5							// SD Card did not respond
#define	SD_ERROR_RESET		6							// SD Card did not reset
#define	SD_ERROR_CLOCK		7							// SD Card clock change failed
#define	SD_ERROR_VOLTAGE	8				// SD Card does not support requested voltage
#define	SD_ERROR_APP_CMD	9				// SD Card app command failed						
#define	SD_CARD_ABSENT		10							// SD Card not present
#define	SD_READ_ERROR		11
#define	SD_MOUNT_FAIL	        12
int sd_init();
int sd_readblock(unsigned int lba, unsigned char *buffer, unsigned int num);
