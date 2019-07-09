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
#define LEFT 68 //좌로 이동    //키보드값들 75
#define RIGHT 67 //우로 이동  77
#define DOWN 80 //아래 이동 
#define p 112 //일시정지 
#define P 80 //일시정지
#define q 113 //게임종료
#define Q 81 //게임종료

#define false 0
#define true 1

#define EMPTY 0         // 블록이 이동할 수 없는 공간은 양수로 표현 
#define WALL 1
#define BARRIER 2
#define CENTER 3
#define CHARCENTER 8
#define CHAR 9



int board[xmax][ymax]={0,}; //전역변수 
int center[20]={0,}; 
int key; //키보드로 입력받은 키값을 저장 

int cx; // 현재 블록의 좌표 저장 
int cy;
int printy=0;
int prevy=ymax-1;

int level; //현재 level 
int level_goal; //다음레벨로 넘어가기 위한 목표점수 
int score; //현재 점수

void check_key(void); //키보드로 키를 입력받음 
void pauser(void);//게임을 일시정지시킴 
void reset_board(void); //메인 게임판 초기화 
void draw_board(void); //게임판을 그림 
void move_block(int dir); //캐릭터를 내림 
void move_barr(void); // 장애물을 움직임 
int check_game_over(void); //게임오버인지 판단하고 게임오버를 진행 
void del_line(void);
void gotoxy(int x, int y);
void level_up();
void Sleep(unsigned int mseconds);
void scoresave(int gamescore); // gamescore 저장하기 위한 함수

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

void draw_map() { // 움직이는 장애물을 따로 출력 
   int i,j; // 지워주는게 필요 함  > del_barr에서 지워줌 
    for(i=printy, j=0; i<ymax; i++, j++) {
      gotoxy(center[i]-3+1,1+j);
      printf("□");
      gotoxy(center[i]+3+1,1+j);
      printf("□");
   }
   for(i=0; i<printy; i++, j++) {
      gotoxy(center[i]-3+1, 1+j);
      printf("□");
      gotoxy(center[i]+3+1, 1+j);
      printf("□");
   }
   gotoxy(0,0);
   printf("Ski Game!!   현재 레벨 : %d \t 현재 점수 : %d \t \n",level,score);
   gotoxy(0,16);
   printf("Pause : P / p \t Exit : Q / q \t \n");
}

void title() {
   printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
   printf("■                                ■\n");
   printf("■                                ■\n");
   printf("■           Ski Game!!           ■\n");
   printf("■                                ■\n");
   printf("■                                ■\n");
   printf("■       Plz Press Any Key!       ■\n");
   printf("■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■\n");
   getchar();
}

void reset(void){

    level=1; //각종변수 초기화 
    score=0;
    cx=12;
    cy=3;
    key=0;

    system("clear"); //화면지움 
    reset_board(); // board를 초기화

}

void reset_board(void){ //게임판을 초기화  
    int i,j;

    for(i=0;i<ymax;i++){ // 게임판을 0으로 초기화  
        for(j=0;j<xmax;j++)
            board[i][j]=0;
    }
    for(i=0;i<ymax;i++){ //좌우 벽을 만듦  
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

    system("setterm -cursor off"); // 커서를 숨기는 시스템콜 
    title(); //메인타이틀 호출 
    reset(); //게임판 리셋 
    while(1){
        for(i=0;i<5;i++){ //블록이 한칸떨어지는동안 5번 키입력받을 수 있음 
      check_key(); //키입력확인
        draw_map(); //화면을 그림
        check_game_over(); //게임오버를 체크
        Sleep();
        }
        del_line();
        del_barr();
        level_up();
        move_block(DOWN); // 블록을 한칸 내림 
        check_game_over(); //게임오버를 체크
    }
    return 0;
}

void del_line(void){
   int i;
   for(i=1; i<xmax-1; i++)
      board[i][printy]=0;
}

void check_key(void){
    key=0; //키값 초기화  

    if(kbhit()){ //키입력이 있는 경우  
        key=getch(); //키값을 받음
        if(key==91){ //방향키인경우  do{key=getch();} while(key==224);//방향키지시값을 버림 
            key=getch();
            switch(key){ //do{key=getch();} while(key==224);//방향키지시값을 버림 
                case LEFT: //왼쪽키 눌렀을때   // 68
                    move_block(LEFT); 
                    break;                            
                case RIGHT: //오른쪽 방향키 눌렀을때- 위와 동일하게 처리됨 67
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
       } //방향키가 아닌경우 
   }
}

void draw_board(void){ //게임판 그리는 함수 
    int i, j;
    system("clear");
    for(i=1; i<=ymax; i++) {
       for(j=0; j<=xmax+1; j=j+xmax+1) {
          gotoxy(j,i);
          printf("▩");
      }
   }
}

void move_block(int dir){ //블록을 이동시킴 
    int i,j;
   gotoxy(cx,cy);
   printf("  ");
   board[cx][cy]=0;
   gotoxy(cx+1,3+1);
   printf("  ");
    switch(dir){
        case LEFT: //왼쪽방향
           if(board[cx-1][cy]==0)
              board[cx-1][cy]=CHAR;
           else if(board[cx-1][cy]==3)
              board[cx-1][cy]=CHARCENTER;
           else
              check_game_over();
           cx--;
           break;

        case RIGHT:    //오른쪽 방향. 왼쪽방향이랑 같은 원리로 동작 
           if(board[cx+1][cy]==0)
              board[cx+1][cy]=CHAR;
           else if(board[cx+1][cy]==3)
              board[cx+1][cy]=CHARCENTER;
           else
              check_game_over();
           cx++;
           break;

        case DOWN:    //아래쪽 방향
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
    gotoxy(cx+1,3+1); // 지우는거 필요  >> 이 함수 위쪽에서 지 움 
    printf("★");
}

void move_barr(void){
   int i,max,adrr,dir;
   max=prevy-1;
   if(max==-1)   max=ymax-1;
   for(i=0; i<xmax; i++) {
      if((board[i][max]==CENTER)||(board[i][max]==CHARCENTER)) { // 맨밑 바로 위의 장애물 위치를 판단 
         adrr=i;
         break;
      }
   }
   srand(time(NULL));
   dir=rand()%3-1; //-1 ~ +1
   if(board[1][max]==BARRIER) { // 튕기는 함수 
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
   else { // 그 외 일반 
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
    if(over) {  //캐릭터 라인에 캐릭터가 없으면 게임오버 
       printf("\n");
       printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤\n"); //게임오버 메세지 
        printf("▤                              ▤\n");
        printf("▤  +-----------------------+   ▤\n");
        printf("▤  |  G A M E  O V E R..   |   ▤\n");
        printf("▤  +-----------------------+   ▤\n");
        printf("▤   YOUR SCORE: %6d         ▤\n",score);
        printf("▤                              ▤\n");
        printf("▤    Press Enter to process..  ▤\n");
        printf("▤                              ▤\n");
        printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤\n");  // 서버저장 하는게 필요
        do{key=getch();} while(key==27);//방향키지시값을 버림
        getchar();
        system("clear");
        gotoxy(10,10); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤\n"); //게임오버 메세지 
        gotoxy(10,11); printf("▤                              ▤\n");
        gotoxy(10,12); printf("▤   YOUR SCORE: %6d         ▤\n",score);
        gotoxy(10,13); printf("▤                              ▤\n");
        gotoxy(10,14); printf("▤     Do you want to save??    ▤\n");
        gotoxy(10,15); printf("▤       If you press Y/y       ▤\n");
        gotoxy(10,16); printf("▤                              ▤\n");
        gotoxy(10,17); printf("▤     if you don't want to     ▤\n");
        gotoxy(10,18); printf("▤  Press any key to process..  ▤\n");
        gotoxy(10,19); printf("▤                              ▤\n");
        gotoxy(10,20); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤\n");  // 서버저장 하는게 필요
        save=getch();
   if((save=='y')||(save=='Y')){
       scoresave(score);
   }
   system("clear"); 
   getchar();
   gotoxy(10,10); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤\n"); //게임오버 메세지 
        gotoxy(10,11); printf("▤                              ▤\n");
        gotoxy(10,12); printf("▤     Do you want to Exit??    ▤\n");
        gotoxy(10,13); printf("▤         Press Y / y          ▤\n");
        gotoxy(10,14); printf("▤                              ▤\n");
        gotoxy(10,15); printf("▤                              ▤\n");
        gotoxy(10,16); printf("▤     if you don't want to     ▤\n");
        gotoxy(10,17); printf("▤  Press any key to restart!!  ▤\n");
        gotoxy(10,18); printf("▤                              ▤\n");
        gotoxy(10,19); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤\n");  // 서버저장 하는게 필요
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
        gotoxy(10,10); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤\n"); //게임오버 메세지 
        gotoxy(10,11); printf("▤                              ▤\n");
        gotoxy(10,12); printf("▤   YOUR SCORE: %6d         ▤\n",score);
        gotoxy(10,13); printf("▤   NICK NAME :                ▤\n");
        gotoxy(10,14); printf("▤                              ▤\n");
        gotoxy(10,15); printf("▤     name size is until 5     ▤\n");
        gotoxy(10,16); printf("▤                              ▤\n");
        gotoxy(10,17); printf("▤                              ▤\n");
        gotoxy(10,18); printf("▤       [ ENTER ] Please       ▤\n");
        gotoxy(10,19); printf("▤                              ▤\n");
        gotoxy(10,20); printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤\n");  // 서버저장 하는게 필요
   gotoxy(26,13); scanf("%s",name);
   
   snprintf(message,BUFSIZE,"%d %s",score,name);
   write(sock,message,BUFSIZE);
   str_len=read(sock,message,BUFSIZE-1);
   message[str_len]=0;
   for(int i=0;i<4;i++){
      gotoxy(23,17); printf("%s\n",message);
      system("sleep 0.10");
      gotoxy(10,17); 
            printf("▤                              ▤\n");
      system("sleep 0.10");
   }
   system("sleep 0.5");
   close(sock);
}

void pauser(void){ //게임 일시정지 함수 

    printf("\n");   //게임 일시정지 메세지 
    printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤\n");
    printf("▤                              ▤\n");
    printf("▤  +-----------------------+   ▤\n");
    printf("▤  |       P A U S E       |   ▤\n");
    printf("▤  +-----------------------+   ▤\n");
    printf("▤  Press any key to resume..   ▤\n");
    printf("▤                              ▤\n");
    printf("▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤▤\n");
   
    getchar(); //키입력시까지 대기 
    system("clear"); //화면 지우고 새로 그림 
    draw_board();

} //끝!

