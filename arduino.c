/* 
This code primarily comes from 
http://www.prasannatech.net/2008/07/socket-programming-tutorial.html
and
http://www.binarii.com/files/papers/c_sockets.txt
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <pthread.h>

#define BUFFER 100
#define SECONDSINHOUR 3600

char buf[BUFFER];
char write_buf[BUFFER];
char ready_string[1000];

double past1hour[3600];
int past1hourIndex=0;
int hour=0;
double average;
double current;
double high = -100;
double low = 500;
char fc = '0';
char prev_fc = '0';
char changelight ='0';
char HOTCOLD='0';
int COLD = 23;
int HOT=26;
int count=0;
int arduinoDown =0;
int standBy=0;

pthread_mutex_t lock_fd;


void calculateAverage(){
    int i=0;
    int max=SECONDSINHOUR;
    if(hour==0){
        max=past1hourIndex;
    }
    double temp=0;
    for(;i<max;i++){
        temp += past1hour[i];
    }
    average=temp/max;
}
void calculateHighLow(double value){
  if(value>high) high = value;
  if(value<low) low = value;

}


void storeTemperature(){
    sscanf(write_buf, "%lf", &current);
    //printf("Current1: %lf",current);
    double current1 =current;
    if(current1>60) {
      current1 = (current1-32.0+0.5)*5.0/9.0;
    }
    
      if(current1==0) return;
      //printf("Current: %lf\n",current1);
      past1hour[past1hourIndex]=current1;
      //printf("storeTemp: current:%lf, past1hourIndex: %d, past1hour[index]=%lf\n",current,past1hourIndex,past1hour[past1hourIndex]);
      if(past1hourIndex!=SECONDSINHOUR-1){
          past1hourIndex++;
      }
      else {
          past1hourIndex=0;
          hour=1;
      }
      calculateAverage();
      calculateHighLow(current1);
}




int start_server(int PORT_NUMBER)
{

      // structs to represent the server and client
      struct sockaddr_in server_addr,client_addr;    
      
      int sock; // socket descriptor

      // 1. socket: creates a socket descriptor that you later use to make other system calls
      if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("Socket");
    exit(1);
      }
      int temp;
      if (setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&temp,sizeof(int)) == -1) {
    perror("Setsockopt");
    exit(1);
      }

      // configure the server
      server_addr.sin_port = htons(PORT_NUMBER); // specify port number
      server_addr.sin_family = AF_INET;         
      server_addr.sin_addr.s_addr = INADDR_ANY; 
      bzero(&(server_addr.sin_zero),8); 
      
      // 2. bind: use the socket and associate it with the port number
      if (bind(sock, (struct sockaddr *)&server_addr, sizeof(struct sockaddr)) == -1) {
    perror("Unable to bind");
    exit(1);
      }

      // 3. listen: indicates that we want to listn to the port to which we bound; second arg is number of allowed connections
      if (listen(sock, 5) == -1) {
    perror("Listen");
    exit(1);
      }
          
      // once you get here, the server is set up and about to start listening
      printf("\nServer configured to listen on port %d\n", PORT_NUMBER);
      fflush(stdout);
     
      while(1){
      // 4. accept: wait here until we get a connection on that port
      int sin_size = sizeof(struct sockaddr_in);
      int fds = accept(sock, (struct sockaddr *)&client_addr,(socklen_t *)&sin_size);
      printf("Server got a connection from (%s, %d)\n", inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
      
      // buffer to read data into
      char request[1024];
      
      // 5. recv: read incoming message into buffer
      int bytes_received = recv(fds,request,1024,0);

      // null-terminate the string
      request[bytes_received] = '\0';
      printf("Here comes the message:\n");
      printf("%s\n", request);

      if(arduinoDown==1) {
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "Arduino disconnected!");
        strcat(reply, "\"\n}\n");
        //printf("writeBuf:%s\n", write_buf);
        printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
        close(fds);
         continue;
      }

      if (standBy==0 && request[5] == 'b'){// display temperature
        // write(fd,"1",2);
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, write_buf);
        strcat(reply, "\"\n}\n");
        printf("writeBuf:%s\n", write_buf);
        printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
      }
      else if(standBy==0 && request[5] == 'a'){
        if (fc == '0'){
          fc = '1';
        }
        else{
          fc = '0';
        }
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "Degree changed!");
        strcat(reply, "\"\n}\n");
        //printf("writeBuf:%s\n", write_buf);
        //printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);


      }
      else if(request[5]== 'c') {
        // down to page2: PM2.5
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "Display PM 2.5 info. / Next Page");
        strcat(reply, "\"\n}\n");
        //printf("writeBuf:%s\n", write_buf);
        //printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
       
      }
      else if(request[5]=='d') {
        //do nothing
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "Can't do anything.");
        strcat(reply, "\"\n}\n");
        //printf("writeBuf:%s\n", write_buf);
        //printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
      }
      else if(request[5]=='e') {
        //display PM2.5
        //TODO
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "PM2.5!");
        strcat(reply, "\"\n}\n");
        //printf("writeBuf:%s\n", write_buf);
        //printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
        
      }
      else if(request[5]=='f') {
        //down to page3: change light
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "Flash green or purple light");
        strcat(reply, "\"\n}\n");
        //printf("writeBuf:%s\n", write_buf);
        //printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
      }
      else if(standBy==0 && request[5]=='g') {
        // change to green light
        printf("@@@@@@change light to green!!\n");
        changelight='g';
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "Flash green light.");
        strcat(reply, "\"\n}\n");
        //printf("writeBuf:%s\n", write_buf);
        //printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
      }
      else if(standBy==0 && request[5]=='h') {
        // change to purple light
        printf("@@@@@@change light to purple!!\n");
        changelight='p';
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "Flash purple light.");
        strcat(reply, "\"\n}\n");
        //printf("writeBuf:%s\n", write_buf);
        //printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
      }
      else if(request[5]=='i') {
        //down to page4: change threshold 
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "Change threshold value.");
        strcat(reply, "\"\n}\n");
        //printf("writeBuf:%s\n", write_buf);
        //printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
      }
      else if(standBy==0 && request[5]=='j') {
        //threadhold value up
        HOTCOLD='u';//up
        HOT++;COLD++;
        printf("HOT:%d, COLD:%d",HOT,COLD);
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "H: ");
        char temp[10];
        sprintf(temp,"%d", HOT);
        strcat(reply, temp);
        strcat(reply," C: ");
        sprintf(temp,"%d", COLD);
        strcat(reply, temp);
        strcat(reply, "\"\n}\n");
        printf("writeBuf:%s\n", write_buf);
        printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
      }
      else if(standBy==0 && request[5]=='k') {
        //threadhold value down
        HOTCOLD='d';//down
        HOT--;COLD--;
        printf("HOT:%d, COLD:%d",HOT,COLD);
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "TH ");
        strcat(reply, "H: ");
        char temp[10];
        sprintf(temp,"%d", HOT);
        strcat(reply, temp);
        strcat(reply," C: ");
        sprintf(temp,"%d", COLD);
        strcat(reply, temp);
        strcat(reply, "\"\n}\n");
        printf("writeBuf:%s\n", write_buf);
        printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
      }
      else if(request[5]=='l'){
        //change to avg, high, low
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "Display high/low and avg temp.");
        strcat(reply, "\"\n}\n");
        //printf("writeBuf:%s\n", write_buf);
        //printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
      }
       else if(standBy==0 && request[5]=='m') {
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "Avg Temp:");
        char temp[10];
 
        sprintf(temp,"%.3f", average);
        strcat(reply, temp);
        strcat(reply, "\"\n}\n");
        //printf("writeBuf:%s\n", write_buf);
        //printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
        //display average
      }
      else if(standBy==0 && request[5]=='n') {
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "H: ");
        char temp[10];
        sprintf(temp,"%.3f", high);
        strcat(reply, temp);
        strcat(reply," L: ");
        sprintf(temp,"%.3f", low);
        strcat(reply, temp);
        strcat(reply, "\"\n}\n");
        //printf("writeBuf:%s\n", write_buf);
        //printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
        //display high and low
      }
      else if(request[5]=='o') {
          //enter standby page
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "standby mode page.");
        strcat(reply, "\"\n}\n");
        //printf("writeBuf:%s\n", write_buf);
        //printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
      } 
      else if(request[5]=='p') {
        //start standby
        standBy=1;
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "Enter standby.");
        strcat(reply, "\"\n}\n");
        //printf("writeBuf:%s\n", write_buf);
        //printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
      }
      else if(request[5]=='q') {
        //quit standby
        standBy=0;
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "Quit standby.");
        strcat(reply, "\"\n}\n");
        //printf("writeBuf:%s\n", write_buf);
        //printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
      }
      else if(request[5]=='r') {
        //change to page1
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "Display Temperature");
        strcat(reply, "\"\n}\n");
        //printf("writeBuf:%s\n", write_buf);
        //printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
      }
      else if(request[5]=='s') {
        //change to page1
        char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "End of App. Press down for page 1");
        strcat(reply, "\"\n}\n");
        //printf("writeBuf:%s\n", write_buf);
        //printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
      }

      else {
         char *reply = malloc(200 * sizeof(char));
        strcat(reply, "{\n\"name\": \"");
        strcat(reply, "In standby mode...");
        strcat(reply, "\"\n}\n");
        printf("writeBuf:%s\n", write_buf);
        printf("reply:%s\n", reply);
        send(fds, reply, strlen(reply), 0);
      }

      

    


      
      // this is the message that we'll send back
      /* it actually looks like this:
        {
           "name": "cit595"
        }
      */
    // char *reply = malloc(200 * sizeof(char));
    // strcat(reply, "{\n\"name\": \"");
    // strcat(reply, write_buf);
    // strcat(reply, "\"\n}\n");
    // printf("writeBuf:%s\n", write_buf);
    // printf("reply:%s\n", reply);


    // char *reply = "{\n\"name\": \"cit595\"\n}\n";


      
      // 6. send: send the message over the socket
      // note that the second argument is a char*, and the third is the number of chars
      // send(fds, reply, strlen(reply), 0);
      //printf("Server sent message: %s\n", reply);

      // 7. close: close the socket connection
      close(fds);
  }
      close(sock);
      printf("Server closed connection\n");
  
      return 0;
} 


void*  getContent(void* p) {
    int i, j;
    
    int fd = open("/dev/cu.usbmodem1421", O_RDWR);
 //   void* r = malloc(sizeof(void*)); //return value of the second thread
    if(fd == -1) {
        fprintf(stderr, "can't open the device!\n");
        return 0;
    }
    
    //2. configure the connection
    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, 9600);
    cfsetospeed(&options, 9600);
    tcsetattr(fd, TCSANOW, &options);
    
    j = 0;
    while(1) {
        

        if(standBy==1) {
          write(fd, "s", 2);
          while(standBy==1){
            //do nothing
            
          }

          write(fd,"t",2);
        }
//        sleep(1);
        for(i = 0; i < BUFFER; i++) {
            buf[i] = 0;
        }
//        pthread_mutex_lock(&lock_fd);
        int bytes_read = read(fd, buf, BUFFER);
        //printf("bytes_read: %d\n",bytes_read);
        if(bytes_read==-1) {
          arduinoDown=1;
        }
        for(i = 0; i < BUFFER; i++, j++) {
            if(buf[i] == '\0') {
                break;
            }
            write_buf[j] = buf[i];
            if(write_buf[j] == '\n') {
                write_buf[j] = '\0';
                //if() {
                  //printf("before storeTemperature\n");
                  storeTemperature();
                //}
                j=-1;
                
            }
        }
        //count++;
        if(prev_fc != fc){
          if (fc == '0'){
            write(fd, "0", 2);
            prev_fc = '0';
          }
          else{
            write(fd, "1", 2);  
            prev_fc = '1';
          }          
        }
        if(changelight!='0'){
          if(changelight=='g'){
            printf("write green light to Uno\n");
            write(fd,"g",2);
          }
          if(changelight=='p'){
            printf("write purple light to Uno\n");
            write(fd,"p",2);
          }
          changelight='0';
        }
        if(HOTCOLD!='0'){
          if(HOTCOLD=='u'){
            printf("write threadhold UP to Uno\n");
            write(fd,"u",2);
          }
          if(HOTCOLD=='d'){
            printf("write threadhold DOWN to Uno\n");
            write(fd,"d",2);
          }
          HOTCOLD='0';
        }


    }
    
    
//    free(r);
    close(fd);
    exit(0);
}

// void* listening() {
//     while(1){

//   }
// }




int main(int argc, char *argv[])
{
    pthread_t t1;
    pthread_mutex_init(&lock_fd, NULL);
    pthread_create(&t1, NULL, &getContent, NULL);
    //pthread_create(&t2, NULL, &listening, NULL);

  // check the number of arguments
  if (argc != 2)
    {
      printf("\nUsage: server [port_number]\n");
      exit(0);
    }

  int PORT_NUMBER = atoi(argv[1]);
  start_server(PORT_NUMBER);
}












