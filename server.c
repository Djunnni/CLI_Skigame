// 컴파일은 gcc server.c -o server -lmysqlclient -L/usr/lib64/mysql 로 하셔야 됩니다.
// skigame project 입니다.
#include "/usr/include/mysql/mysql.h" 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define BUFSIZE 64 
#define PORT 9000

int DB_INDEX=1;
char* Complete="Complete",*Failed="Failed";

void finish_with_error(MYSQL *conn){
	fprintf(stderr,"%s\n",mysql_error(conn));
	mysql_close(conn);
	exit(1);
}
void error_handling(char *message);


int main(int argc, char **argv)

{
	        int serv_sock;
	        int clnt_sock;
		int nSockOpt;
	        char message[BUFSIZE];
		char* data[2],*ptr,query[64];
		memset(query,0,sizeof(query));

		int str_len;
		struct sockaddr_in serv_addr;
                struct sockaddr_in clnt_addr;

                int clnt_addr_size;
		int i=0;
	
		printf("***********************************\n");
		printf("*Welcome to Skigame ScoreDB SERVER*\n");
		printf("***********************************\n");
		MYSQL *conn = mysql_init(NULL);
		if(conn==NULL){
			fprintf(stderr,"%s\n",mysql_error(conn));
			exit(1);
		}
		if(mysql_real_connect(conn,"localhost","*****","****",NULL,0,NULL,0)==NULL){
			finish_with_error(conn);
		}
		if(mysql_query(conn,"*******")){
			finish_with_error(conn);
		}
			
	
             serv_sock = socket(PF_INET, SOCK_STREAM, 0);   
             if(serv_sock == -1)
                     error_handling("socket() error");
           
	        memset(&serv_addr, 0, sizeof(serv_addr));
               	serv_addr.sin_family = AF_INET;
	        serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
     	        serv_addr.sin_port = htons(9000);

															       
		nSockOpt=1;
setsockopt(serv_sock, SOL_SOCKET,SO_REUSEADDR,&nSockOpt,sizeof(nSockOpt));

 /* 소켓에 주소 할당 */
 if( bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr) )==-1)
 error_handling("bind() error");

															       
 if(listen(serv_sock, 5) == -1)  /* 연결 요청 대기 상태로 진입 */
 error_handling("listen() error");

																       
 clnt_addr_size = sizeof(clnt_addr);

while(1){
      /* 연결 요청 수락 */
       clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
       if(clnt_sock == -1)
                   error_handling("accept() error");

																           

/* 데이터 수신 및 전송
 *   *
 *     *           * 클라이언트가 EOF를 보낼 때까지 데이터를 받아서 서버 콘솔에 한번 출력해 주고
 *       *
 *         *                     * 클라이언트로 재전송해 주고 있다. */
while( (str_len = read(clnt_sock, message, BUFSIZE)) != 0) {

		ptr=strtok(message," ");
		while(ptr!=NULL){
			data[i]=ptr;
			ptr=strtok(NULL," ");
			i=i+1;
		}
		snprintf(query,64,"insert into score_tb values(%d,%d,'%s')",DB_INDEX++,atoi(data[0]),data[1]);
		printf("-----------------------------------------\n");
		printf("%s\n",query);
		//sql_query는 정상일 경우 0 아닐 경우엔 !=0
		if(mysql_query(conn,query)){
		i=0;
		memset(query,0,sizeof(query));
		send(clnt_sock,Failed,sizeof(Failed)+1,0);
		printf("Index Number : %d is Failed\n",DB_INDEX-1);
		DB_INDEX--;
		printf("-----------------------------------------\n");
		continue;
			}
		i=0;
		 send(clnt_sock,Complete,sizeof(Complete)+1,0);
		 printf("Index Number : %d is Complete\n",DB_INDEX-1);
		 printf("-----------------------------------------\n");
		 memset(query,0,sizeof(query));
	  }
		
		close(clnt_sock);
	 }
		 mysql_close(conn);
		  return 0;
		
 }
		
							                      
		void error_handling(char *message) {
		fputs(message, stderr);
		putc('\n', stderr);
		 exit(1);
		}

