

/****************************************************************
 * Constants
 ****************************************************************/
 
#define SYSFS_GPIO_DIR "/sys/class/gpio"
#define POLL_TIMEOUT 100 /* 0.1 seconds */
#define MAX_BUF 64

void signal_handler(int sig);
int gpio_export(unsigned int gpio);
int gpio_unexport(unsigned int gpio);
int gpio_set_dir(unsigned int gpio, unsigned int out_flag);
int gpio_set_value(unsigned int gpio, unsigned int value);
int gpio_get_value(unsigned int gpio, unsigned int *value);
int read_analog1(unsigned int *value);
int gpio_set_edge(unsigned int gpio, char *edge);
int gpio_fd_open(unsigned int gpio);
int gpio_fd_close(int fd);
int geti2c(int i2cbus, int address, int daddress);
int set_pwm_mux(char *port, char *mux);
int set_pwm_param(int pwm, char *val, char *param);
