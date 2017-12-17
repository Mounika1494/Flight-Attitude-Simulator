#include "../../inc/communication.h"

int fd1,fd2;
struct termios *configure1,*configure2;
//mutex lock so that the i2c functions are nor pre- empted
static pthread_mutex_t i2c_mutex = PTHREAD_MUTEX_INITIALIZER;

//Configure UART
int8_t tty_config(struct termios *con, int descriptor){
    
    pthread_mutex_lock(&i2c_mutex);
    tcgetattr(descriptor, con);
	con->c_iflag &= ~(IGNBRK | BRKINT | ICRNL | INLCR | PARMRK | INPCK | ISTRIP | IXON);
	con->c_oflag = 0;
	con->c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | ISIG);
	con->c_cc[VMIN] = 1;
	con->c_cc[VTIME] = 0;
	
	if(cfsetispeed(con, B115200)||cfsetospeed(con, B115200)){
	    
		perror("ERROR in baud set\n");
		return -1;
	}
	
	
	if(tcsetattr(descriptor, TCSAFLUSH, con) < 0){
	    
		perror("ERROR in set attr\n");
		return -1;
	}
	
	pthread_mutex_unlock(&i2c_mutex);
    pthread_mutex_destroy(&i2c_mutex);
 	
	return 0;
    
}


//read one byte 
char read_one_byte(int fd){
    
    int i = 0;
    char c;
    while(1)
	{
		if(read(fd, &c, 1) > 0)
		{
			
			fflush(stdout);
			++i;
			
		}
		if(i>0)
			break;
		
	}
	return c;
}

// read multiple bytes till newline
int8_t read_bytes(int fd, char *str){
	
	pthread_mutex_lock(&i2c_mutex);
    
    int i = 0;
    while(1){
    
        *(str+i) = read_one_byte(fd);
        //printf("%c\n", *(str+i));
        if(*(str+i) == '\n'){
        	*(str+i) = '\0';
        	break;
        }
        	
        ++i;
    }
    pthread_mutex_unlock(&i2c_mutex);
    pthread_mutex_destroy(&i2c_mutex);
    
    return 0;
    
}


//send multiple bytes 
int8_t transfer_bytes(int fd, char *str){
	
	pthread_mutex_lock(&i2c_mutex);
	
	write(fd, str, strlen(str)+1);
	
	pthread_mutex_unlock(&i2c_mutex);
    pthread_mutex_destroy(&i2c_mutex);
    
	return 0;
}

int main(){
	
	char str[100];
	 char *device1 = "/dev/ttyO1";
	  char *device2 = "/dev/ttyO2";
	fd1 = open(device1, O_RDWR | O_NOCTTY | O_NONBLOCK);
       if(fd1 == -1){
       
              perror("ERROR opening file descriptor\n");
       }
       
       //COnfigure UART
       configure1 = (struct termios*)malloc(sizeof(struct termios));
       tty_config(configure1, fd1);
       
       fd2 = open(device2, O_RDWR | O_NOCTTY | O_NONBLOCK);
       if(fd2 == -1){
       
              perror("ERROR opening file descriptor\n");
       }
       
       //COnfigure UART
       configure2 = (struct termios*)malloc(sizeof(struct termios));
       tty_config(configure2, fd2);
       
       
       
       //Loop back test. Connect 21-26 and 22-24
       transfer_bytes(fd2,"hello_world\n");
       
       read_bytes(fd1, str);
       printf("%s\n", str);
       
        transfer_bytes(fd1,"hello_world\n");
	
	
	
	
}