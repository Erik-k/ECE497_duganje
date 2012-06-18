/* Josh Dugan 9/12/12 
basic pwm functions to set a pin mux value and moify a pwm parameter
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "miniproj.h"

/****************************************************************
 * set_pwm_mux
 ****************************************************************/
//sets port selection mux to the value of "mux"
int set_pwm_mux(char *port, char *mux)
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf(buf, sizeof(buf), "/sys/kernel/debug/omap_mux/%s", port);
 
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("omap_mux");
		return fd;
	}
 
	write(fd, mux, sizeof(mux)); 
	close(fd);
	return 0;
}

/****************************************************************
 * set_pwm_mux
 ****************************************************************/
//sets the parameter of pwm to the value of val
int set_pwm_param(int pwm, char *val, char *param) 
{
	int fd, len;
	char buf[MAX_BUF];

	len = snprintf(buf, sizeof(buf), "/sys/class/pwm/ehrpwm.%d:0/%s", pwm, param);
 
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("ehrpwm");
		return fd;
	}
 
	write(fd, val, sizeof(val)); 
	close(fd);
	return 0;

}
