/* Copyright (c) 2011, RidgeRun
 * All rights reserved.
 *
From https://www.ridgerun.com/developer/wiki/index.php/Gpio-int-test.c

 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the RidgeRun.
 * 4. Neither the name of the RidgeRun nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY RIDGERUN ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL RIDGERUN BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include "miniproj.h"
#include <poll.h>
#include <signal.h>	// Defines signal-handling functions (i.e. trap Ctrl-C)

/****************************************************************
 * Global variables
 ****************************************************************/
int keepgoing = 1;	// Set to 0 when ctrl-c is pressed


/****************************************************************
 * signal_handler
 ****************************************************************/
// Callback called when SIGINT is sent to the process (Ctrl-C)
void signal_handler(int sig)
{
	printf( "Ctrl-C pressed, cleaning up and exiting..\n" );
	keepgoing = 0;
}

/****************************************************************
 * Main
 ****************************************************************/
int main(int argc, char **argv, char **envp)
{
	struct pollfd fdset[2];
	int nfds = 2;
	int gpio_fd, timeout, rc;
	char *buf[MAX_BUF];
	unsigned int gpio;
	unsigned int val, analog;
	int len, temp;
	unsigned int lastval = 0;

	// Set the signal callback for Ctrl-C
	signal(SIGINT, signal_handler);

	set_pwm_mux("gpmc_a2", "6");
	
	set_pwm_param(1, "0", "duty_percent");
	set_pwm_param(1, "1", "run");
	set_pwm_param(1, "100", "period_freq");

	gpio_export(48);
	gpio_set_dir(48, 1);

	gpio_export(49);
	gpio_set_dir(49, 0);
	gpio_set_edge(49, "both");  // Can be rising, falling or both
	gpio_fd = gpio_fd_open(49);

	timeout = POLL_TIMEOUT;
 
	while (keepgoing) {
		gpio_get_value(49, &val);
		gpio_set_value(48, val);


		memset((void*)fdset, 0, sizeof(fdset));

		fdset[0].fd = STDIN_FILENO;
		fdset[0].events = POLLIN;
      
		fdset[1].fd = gpio_fd;
		fdset[1].events = POLLPRI;

		rc = poll(fdset, nfds, timeout);      

		if (rc < 0) {
			printf("\npoll() failed!\n");
			return -1;
		}
      
		if (rc == 0) {
			temp = geti2c(3, 0x4b, 0);
			
			if (temp < 25){
				set_pwm_param(1, "0", "duty_percent");
			}else if (temp == 25){
				set_pwm_param(1, "15", "duty_percent");
			}else if (temp == 26){
				set_pwm_param(1, "30", "duty_percent");
			}else if (temp == 27){
				set_pwm_param(1, "45", "duty_percent");
			}else if (temp == 28){
				set_pwm_param(1, "60", "duty_percent");
			}else if (temp == 29){
				set_pwm_param(1, "75", "duty_percent");
			}else if (temp == 30){
				set_pwm_param(1, "90", "duty_percent");
			}else{
				set_pwm_param(1, "100", "duty_percent");
			}
			
		}
            
		if (fdset[1].revents & POLLPRI) {
			lseek(fdset[1].fd, 0, SEEK_SET);  // Read from the start of the file
			len = read(fdset[1].fd, buf, MAX_BUF);

			gpio_get_value(49, &val);
			if ((!lastval) && (val)){
				read_analog1(&analog);
				temp = geti2c(3, 0x4b, 0);
				printf("Temperature: %d\nVoltage: %d\n", temp, analog);
			

			}
			lastval = val;
			
		}

		if (fdset[0].revents & POLLIN) {
			(void)read(fdset[0].fd, buf, 1);
		}

		fflush(stdout);
	}


	gpio_fd_close(gpio_fd);
	return 0;
}

