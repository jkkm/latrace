
/* /usr/include/termios.h */

typedef u_char cc_t;
typedef u_int  speed_t;
typedef u_int  tcflag_t;
typedef void termios;


speed_t cfgetospeed(termios *termios_p);
speed_t cfgetispeed(termios *termios_p);


int cfsetospeed(termios *termios_p, speed_t speed);
int cfsetispeed(termios *termios_p, speed_t speed);
int cfsetspeed(termios *termios_p, speed_t speed);


int tcgetattr(int fd, termios *termios_p);
int tcsetattr(int fd, int optional_actions, termios *termios_p);


void cfmakeraw(termios *termios_p);
int tcsendbreak(int fd, int duration);
int tcdrain(int fd);
int tcflush(int fd, int queue_selector);
int tcflow(int fd, int action);
__pid_t tcgetsid(int fd);
