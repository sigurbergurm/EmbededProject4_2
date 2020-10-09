#include<fcntl.h>
#include<unistd.h>
#include<string.h>


int main()
{
    
    char in_buffer[20];
    char out_buffer[20];
    size_t count;
    ssize_t ret;
    ssize_t BUFFER_LENGTH = sizeof(in_buffer);
    strcpy(out_buffer, "String to send/n");
    count = strlen(out_buffer);

    for (int i = 0; i < 10; i++)
    {
        int fd = open("/dev/hello2", O_RDWR);
        ret = read(fd, in_buffer, BUFFER_LENGTH);
        ret = write(fd, out_buffer, count);
        ret = close(fd);
        sleep(1);
    }
    

    return 0;
}