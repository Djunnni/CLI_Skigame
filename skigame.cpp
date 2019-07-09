#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <termio.h>
#include <unistd.h>
#include <fcntl.h>

#include<string.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#define BUFSIZE 64
#define PORT 9000

#define xmax 23
#define ymax 15
#define LEFT 68 //�·� �̵�    //Ű���尪�� 75
#define RIGHT 67 //��� �̵�  77
#define DOWN 80 //�Ʒ� �̵� 
#define p 112 //�Ͻ����� 
#define P 80 //�Ͻ�����
#define q 113 //��������
#define Q 81 //��������

#define false 0
#define true 1

#define EMPTY 0         // ����� �̵��� �� ���� ������ ����� ǥ�� 
#define WALL 1
#define BARRIER 2
#define CENTER 3
#define CHARCENTER 8
#define CHAR 9



int board[xmax][ymax]={0,}; //�������� 
int center[20]={0,}; 
int key; //Ű����� �Է¹��� Ű���� ���� 

int cx; // ���� ����� ��ǥ ���� 
int cy;
int printy=0;
int prevy=ymax-1;

int level; //���� level 
int level_goal; //���������� �Ѿ�� ���� ��ǥ���� 
int score; //���� ����

void check_key(void); //Ű����� Ű�� �Է¹��� 
void pauser(void);//������ �Ͻ�������Ŵ 
void reset_board(void); //���� ������ �ʱ�ȭ 
void draw_board(void); //�������� �׸� 
void move_block(int dir); //ĳ���͸� ���� 
void move_barr(void); // ��ֹ��� ������ 
int check_game_over(void); //���ӿ������� �Ǵ��ϰ� ���ӿ����� ���� 
void del_line(void);
void gotoxy(int x, int y);
void level_up();
void Sleep(unsigned int mseconds);
void scoresave(int gamescore); // gamescore �����ϱ� ���� �Լ�

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if(ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}

int getch(void){
    int ch;
    struct termios buf, save;
    tcgetattr(0,&save);
    buf = save;
    buf.c_lflag &= ~(ICANON|ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;
    tcsetattr(0, TCSAFLUSH, &buf);
    ch = getchar();
    tcsetattr(0, TCSAFLUSH, &save);
    return ch;
}

void Sleep() {
    switch(level) {
       case 1:
          system("sleep 0.12");
          break;
       case 2:
          system("sleep 0.115");
          break;
       case 3:
          system("sleep 0.11");
          break;
       case 4:
          system("sleep 0.1");
          break;
       case 5:
          system("sleep 0.095");
          break;
       case 6:
          system("sleep 0.09");
          break;
       case 7:
          system("sleep 0.085");
          break;
       case 8:
          system("sleep 0.08");
          break;
       case 9:
          system("sleep 0.07");
          break;
       case 10:
          system("sleep 0.06");
          break;
       case 11:
          system("sleep 0.055");
          break;
       case 12:
          system("sleep 0.05");
          break;
        case 13:
          system("sleep 0.045");
          break;
        case 14:
          system("sleep 0.04");
          break;
        case 15:
          system("sleep 0.035");
          break;
       case 16:
          system("sleep 0.03");
          break;
        case 17:
          system("sleep 0.025");
          break;
        case 18:
          system("sleep 0.02");
          break;
        case 19:
          system("sleep 0.015");
          break;
        case 20:
          system("sleep 0.01");
          break;
   }
}


void level_up() {
   if(score<20)   level=1;
   else if(score<60) {
      level=2;
   }
   else if(score<120) {
      level=3;
   }
   else if(score<200) {
      level=4;
   }
   else if(score<300) {
      level=5;
   }
   else if(score<420) {
      level=6;
   }
   else if(score<560) {
      level=7;
   }
   else if(score<700) {
      level=8;
   }
   else if(score<860) {
      level=9;
   }
   else if(score<1200) {
      level=10;
   }
   else if(score<1600) {
      level=11;
   }
   else if(score<2000) {
      level=12;
   }
   else if(score<2500) {
      level=13;
   }
   else if(score<3000) {
      level=14;
   }
   else if(score<4000) {
      level=15;
   }
   else if(score<4500) {
      level=16;
   }
   else if(score<5000) {
      level=17;
   }
   else if(score<5800) {
      level=18;
   }
   else if(score<7000) {
      level=19;
   }
   else if(score<8500) {
      level=20;
   }
}

void del_barr() {
   int i,j; 
   for(i=printy, j=0; i<ymax; i++, j++) {
      gotoxy(center[i]-3+1,1+j);
      printf("  ");
      gotoxy(center[i]+3+1,1+j);
      printf("  ");
   }
   for(i=0; i<printy; i++, j++) {
      gotoxy(center[i]-3+1, 1+j);
      printf("  ");
      gotoxy(center[i]+3+1, 1+j);
      printf("  ");
   }
}

void gotoxy(int x, int y) {
    printf("\033[%d;%df", y, x);
    fflush(stdout);
}

void draw_map() { // �����̴� ��ֹ��� ���� ��� 
   int i,j; // �����ִ°� �ʿ� ��  > del_barr���� ������ 
    for(i=printy, j=0; i<ymax; i++, j++) {
      gotoxy(center[i]-3+1,1+j);
      printf("��");
      gotoxy(center[i]+3+1,1+j);
      printf("��");
   }
   for(i=0; i<printy; i++, j++) {
      gotoxy(center[i]-3+1, 1+j);
      printf("��");
      gotoxy(center[i]+3+1, 1+j);
      printf("��");
   }
   gotoxy(0,0);
   printf("Ski Game!!   ���� ���� : %d \t ���� ���� : %d \t \n",level,score);
   gotoxy(0,16);
   printf("Pause : P / p \t Exit : Q / q \t \n");
}

void title() {
   printf("�����������������������������������\n");
   printf("��                                ��\n");
   printf("��                                ��\n");
   printf("��           Ski Game!!           ��\n");
   printf("��                                ��\n");
   printf("��                                ��\n");
   printf("��       Plz Press Any Key!       ��\n");
   printf("�����������������������������������\n");
   getchar();
}

void reset(void){

    level=1; //�������� �ʱ�ȭ 
    score=0;
    cx=12;
    cy=3;
    key=0;

    system("clear"); //ȭ������ 
    reset_board(); // board�� �ʱ�ȭ

}

void reset_board(void){ //�������� �ʱ�ȭ  
    int i,j;

    for(i=0;i<ymax;i++){ // �������� 0���� �ʱ�ȭ  
        for(j=0;j<xmax;j++)
            board[i][j]=0;
    }
    for(i=0;i<ymax;i++){ //�¿� ���� ����  
        board[0][i]=WALL;
        board[xmax-1][i]=WALL;
        board[xmax/2-3][i]=BARRIER;
      board[xmax/2+3][i]=BARRIER; 
    }
    board[xmax/2][ymax-1]=CENTER;
    board[xmax/2+3][ymax-1]=BARRIER;
    board[xmax/2-3][ymax-1]=BARRIER;
    board[xmax/2][cy]=CHAR;
    printy=0;
    for(i=0; i<ymax; i++)
       center[i]=xmax/2;
    draw_board();
}

int main(){
    int i;

    system("setterm -cursor off"); // Ŀ���� ����� �ý����� 
    title(); //����Ÿ��Ʋ ȣ�� 
    reset(); //������ ���� 
    while(1){
        for(i=0;i<5;i++){ //����� ��ĭ�������µ��� 5�� Ű�Է¹��� �� ���� 
      check_key(); //Ű�Է�Ȯ��
        draw_map(); //ȭ���� �׸�
        check_game_over(); //���ӿ����� üũ
        Sleep();
        }
        del_line();
        del_barr();
        level_up();
        move_block(DOWN); // ����� ��ĭ ���� 
        check_game_over(); //���ӿ����� üũ
    }
    return 0;
}

void del_line(void){
   int i;
   for(i=1; i<xmax-1; i++)
      board[i][printy]=0;
}

void check_key(void){
    key=0; //Ű�� �ʱ�ȭ  

    if(kbhit()){ //Ű�Է��� �ִ� ���  
        key=getch(); //Ű���� ����
        if(key==91){ //����Ű�ΰ��  do{key=getch();} while(key==224);//����Ű���ð��� ���� 
            key=getch();
            switch(key){ //do{key=getch();} while(key==224);//����Ű���ð��� ���� 
                case LEFT: //����Ű ��������   // 68
                    move_block(LEFT); 
                    break;                            
                case RIGHT: //������ ����Ű ��������- ���� �����ϰ� ó���� 67
                    move_block(RIGHT);
                    break;
            }
      }
       else if((key==P)||(key==p))
           pauser();
       else if((key==q)||(key==Q)) {
           system("clear");
           system("setterm -cursor on");
           exit(0);
       } //����Ű�� �ƴѰ�� 
   }
}

void draw_board(void){ //������ �׸��� �Լ� 
    int i, j;
    system("clear");
    for(i=1; i<=ymax; i++) {
       for(j=0; j<=xmax+1; j=j+xmax+1) {
          gotoxy(j,i);
          printf("��");
      }
   }
}

void move_block(int dir){ //����� �̵���Ŵ 
    int i,j;
   gotoxy(cx,cy);
   printf("  ");
   board[cx][cy]=0;
   gotoxy(cx+1,3+1);
   printf("  ");
    switch(dir){
        case LEFT: //���ʹ���
           if(board[cx-1][cy]==0)
              board[cx-1][cy]=CHAR;
           else if(board[cx-1][cy]==3)
              board[cx-1][cy]=CHARCENTER;
           else
              check_game_over();
           cx--;
           break;

        case RIGHT:    //������ ����. ���ʹ����̶� ���� ������ ���� 
           if(board[cx+1][cy]==0)
              board[cx+1][cy]=CHAR;
           else if(board[cx+1][cy]==3)
              board[cx+1][cy]=CHARCENTER;
           else
              check_game_over();
           cx++;
           break;

        case DOWN:    //�Ʒ��� ����
           if(cy==ymax-1)   cy=-1;
           cy++;
           if(board[cx][cy]==0)
              board[cx][cy]=CHAR;
           else if(board[cx][cy]==3)
              board[cx][cy]=CHARCENTER;
           else
              check_game_over();
           prevy=printy;
           printy++;
           if(printy==ymax)   printy=0;
           move_barr();
           score=score+level;
           break;
    } 
    gotoxy(cx+1,3+1); // ����°� �ʿ�  >> �� �Լ� ���ʿ��� �� �� 
    printf("��");
}

void move_barr(void){
   int i,max,adrr,dir;
   max=prevy-1;
   if(max==-1)   max=ymax-1;
   for(i=0; i<xmax; i++) {
      if((board[i][max]==CENTER)||(board[i][max]==CHARCENTER)) { // �ǹ� �ٷ� ���� ��ֹ� ��ġ�� �Ǵ� 
         adrr=i;
         break;
      }
   }
   srand(time(NULL));
   dir=rand()%3-1; //-1 ~ +1
   if(board[1][max]==BARRIER) { // ƨ��� �Լ� 
      board[1+4][prevy]=CENTER;
      board[1+4+3][prevy]=BARRIER;
      board[1+4-3][prevy]=BARRIER;
      center[prevy]=(1+4);
   }
   else if(board[xmax-2][max]==BARRIER) {
      board[xmax-2-4][prevy]=CENTER;
      board[xmax-2-4+3][prevy]=BARRIER;
      board[xmax-2-4-3][prevy]=BARRIER;
      center[prevy]=(xmax-2-4);
   }
   else { // �� �� �Ϲ� 
      board[adrr+dir][prevy]=CENTER;
      board[adrr+dir+3][prevy]=BARRIER;
      board[adrr+dir-3][prevy]=BARRIER;
      center[prevy]=(adrr+dir);
   }
}

int check_game_over(void) {
    int i;
    int over=1;
    char save;
    char name[3];

    for(i=1;i<xmax-1;i++){
       if((board[i][cy]==CHAR)||(board[i][cy]==CHARCENTER)) {
          over=0;
          break;
       }
    }
    if(over) {  //ĳ���� ���ο� ĳ���Ͱ� ������ ���ӿ��� 
       printf("\n");
       printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�\n"); //���ӿ��� �޼��� 
        printf("��                              ��\n");
        printf("��  +-----------------------+   ��\n");
        printf("��  |  G A M E  O V E R..   |   ��\n");
        printf("��  +-----------------------+   ��\n");
        printf("��   YOUR SCORE: %6d         ��\n",score);
        printf("��                              ��\n");
        printf("��    Press Enter to process..  ��\n");
        printf("��                              ��\n");
        printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�\n");  // �������� �ϴ°� �ʿ�
        do{key=getch();} while(key==27);//����Ű���ð��� ����
        getchar();
        system("clear");
        gotoxy(10,10); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�\n"); //���ӿ��� �޼��� 
        gotoxy(10,11); printf("��                              ��\n");
        gotoxy(10,12); printf("��   YOUR SCORE: %6d         ��\n",score);
        gotoxy(10,13); printf("��                              ��\n");
        gotoxy(10,14); printf("��     Do you want to save??    ��\n");
        gotoxy(10,15); printf("��       If you press Y/y       ��\n");
        gotoxy(10,16); printf("��                              ��\n");
        gotoxy(10,17); printf("��     if you don't want to     ��\n");
        gotoxy(10,18); printf("��  Press any key to process..  ��\n");
        gotoxy(10,19); printf("��                              ��\n");
        gotoxy(10,20); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�\n");  // �������� �ϴ°� �ʿ�
        save=getch();
   if((save=='y')||(save=='Y')){
       scoresave(score);
   }
   system("clear"); 
   getchar();
   gotoxy(10,10); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�\n"); //���ӿ��� �޼��� 
        gotoxy(10,11); printf("��                              ��\n");
        gotoxy(10,12); printf("��     Do you want to Exit??    ��\n");
        gotoxy(10,13); printf("��         Press Y / y          ��\n");
        gotoxy(10,14); printf("��                              ��\n");
        gotoxy(10,15); printf("��                              ��\n");
        gotoxy(10,16); printf("��     if you don't want to     ��\n");
        gotoxy(10,17); printf("��  Press any key to restart!!  ��\n");
        gotoxy(10,18); printf("��                              ��\n");
        gotoxy(10,19); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�\n");  // �������� �ϴ°� �ʿ�
        save=getch();
   if((save=='y')||(save=='Y')){
       system("clear");
        system("setterm -cursor on");
        exit(0);
   }
   else   reset();
    }
}

void scoresave(int gamescore){
   int sock;
   int score=gamescore;
   char name[5];
   struct sockaddr_in serv_addr;
   char message[BUFSIZE];
   int str_len;
   sock = socket(PF_INET,SOCK_STREAM,0);
   if(sock == -1){
      printf("socket error");
      exit(1);
   }
   memset(message,0,sizeof(message));
   memset(&serv_addr,0,sizeof(serv_addr));
   serv_addr.sin_family=AF_INET;
   serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
   serv_addr.sin_port=htons(PORT);
      
   if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1){
      printf("connect error");
      exit(1);
   }
   system("clear");
        gotoxy(10,10); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�\n"); //���ӿ��� �޼��� 
        gotoxy(10,11); printf("��                              ��\n");
        gotoxy(10,12); printf("��   YOUR SCORE: %6d         ��\n",score);
        gotoxy(10,13); printf("��   NICK NAME :                ��\n");
        gotoxy(10,14); printf("��                              ��\n");
        gotoxy(10,15); printf("��     name size is until 5     ��\n");
        gotoxy(10,16); printf("��                              ��\n");
        gotoxy(10,17); printf("��                              ��\n");
        gotoxy(10,18); printf("��       [ ENTER ] Please       ��\n");
        gotoxy(10,19); printf("��                              ��\n");
        gotoxy(10,20); printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�\n");  // �������� �ϴ°� �ʿ�
   gotoxy(26,13); scanf("%s",name);
   
   snprintf(message,BUFSIZE,"%d %s",score,name);
   write(sock,message,BUFSIZE);
   str_len=read(sock,message,BUFSIZE-1);
   message[str_len]=0;
   for(int i=0;i<4;i++){
      gotoxy(23,17); printf("%s\n",message);
      system("sleep 0.10");
      gotoxy(10,17); 
            printf("��                              ��\n");
      system("sleep 0.10");
   }
   system("sleep 0.5");
   close(sock);
}

void pauser(void){ //���� �Ͻ����� �Լ� 

    printf("\n");   //���� �Ͻ����� �޼��� 
    printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�\n");
    printf("��                              ��\n");
    printf("��  +-----------------------+   ��\n");
    printf("��  |       P A U S E       |   ��\n");
    printf("��  +-----------------------+   ��\n");
    printf("��  Press any key to resume..   ��\n");
    printf("��                              ��\n");
    printf("�ǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢǢ�\n");
   
    getchar(); //Ű�Է½ñ��� ��� 
    system("clear"); //ȭ�� ����� ���� �׸� 
    draw_board();

} //��!

