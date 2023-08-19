#include <graphics.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

const double pi=2*acos(0.0);
double rad(int degree){
	double ans=pi*degree/180.0;
	return ans;
}
double deg(double radi){
	double ans=180.0*radi/pi;
	return ans;
}

#define min(x,y) ((x)>(y))? (y):(x)
#define max(x,y) ((x)>(y))? (x):(y)
#define ms 1000
#define Xratio X_full/1280 
#define Yratio Y_full/720  
#define ratio sqrt(Xratio*Yratio) 

int X=GetSystemMetrics(SM_CXSCREEN); 
int Y=GetSystemMetrics(SM_CYSCREEN); 
int X_full=X,Y_full=Y; 
int X0=100*Xratio,Y0=50*Yratio;
int speed=7*ratio,speed_ball=10*ratio;
int radius=50*ratio,radius_ball=20*ratio; 
double angle1=0.0,angle=0.0,angle2=0.0;
int goal_height=100*Yratio,goal_width=90*Xratio; 
int x1=X0+radius+10*Xratio,y1=Y/2; 
int x=X/2,y=Y/2;
int x2=X-goal_height-radius-10*Xratio,y2=Y/2; 
int goal1=0,goal2=0,goal=0,tme=0,page=0;
int striker_color1,striker_color2;

void settings();
void start(){
	setbkcolor(7);
	cleardevice();
	setcolor(BLUE);
	settextstyle(3,HORIZ_DIR,10);
	char c1[]="AIR HOCKEY";
	outtextxy(X/2-300,Y/2-150,c1);
	setcolor(9);
	settextstyle(3,HORIZ_DIR,5);
	char c2[]="Press ENTER to play!";
	outtextxy(X/2-240,Y/2,c2);
	while(!GetAsyncKeyState(VK_RETURN)) delay(10);
	settings();
}
void settings(){
	cleardevice();
	setcolor(1);
	setlinestyle(0,0,5);
	rectangle(50,50,X-50,Y-50);
	setfillstyle(SOLID_FILL,8);
	floodfill(X/2,Y/2,1);
	char c1[]="Choose color for PLAYER 1:";
	settextstyle(3,HORIZ_DIR,7);
	outtextxy(X/4-100,100,c1);
	char c2[]="Press 1 to choose";
	settextstyle(3,HORIZ_DIR,4);
	outtextxy(X/4-75,Y-150,c2);
	char c3[]="Press 2 to choose";
	outtextxy(3*X/4-225,Y-150,c3);
	setcolor(WHITE);
	setfillstyle(SOLID_FILL,1);
	circle(X/4+75,Y/2+25,radius*2.5);
	floodfill(X/4+75,Y/2+25,WHITE);
	circle(3*X/4-75,Y/2+25,radius*2.5);
	setfillstyle(SOLID_FILL,3);
	floodfill(3*X/4-75,Y/2+25,WHITE);
	while(1){
		if(GetAsyncKeyState('1')){
			striker_color1=1;
			break;
		}
		else if(GetAsyncKeyState('2')){
			striker_color1=3;
			break;
		}
		usleep(10*ms);
	}
	while(GetAsyncKeyState('1') || GetAsyncKeyState('2')) delay(10);
	cleardevice();
	setcolor(1);
	setlinestyle(0,0,5);
	rectangle(50,50,X-50,Y-50);
	setfillstyle(SOLID_FILL,8);
	floodfill(X/2,Y/2,1);
	char c4[]="Choose color for PLAYER 2:";
	settextstyle(3,HORIZ_DIR,7);
	outtextxy(X/4-100,100,c4);
	char c5[]="Press 1 to choose";
	settextstyle(3,HORIZ_DIR,4);
	outtextxy(X/4-75,Y-150,c5);
	char c6[]="Press 2 to choose";
	outtextxy(3*X/4-225,Y-150,c6);
	setcolor(WHITE);
	setfillstyle(SOLID_FILL,4);
	circle(X/4+75,Y/2+25,radius*2.5);
	floodfill(X/4+75,Y/2+25,WHITE);
	circle(3*X/4-75,Y/2+25,radius*2.5);
	setfillstyle(SOLID_FILL,5);
	floodfill(3*X/4-75,Y/2+25,WHITE);
	while(1){
		if(GetAsyncKeyState('1')){
			striker_color2=4;
			break;
		}
		else if(GetAsyncKeyState('2')){
			striker_color2=5;
			break;
		}
		usleep(10*ms);
	}
}
void pause(){
	sleep(1);
	while(!(GetAsyncKeyState(VK_ESCAPE) || GetAsyncKeyState(VK_SPACE))) usleep(10*ms);
	while(GetAsyncKeyState(VK_ESCAPE) || GetAsyncKeyState(VK_SPACE)) usleep(10*ms);
	tme--;
	usleep(500*ms);
}
void move_ball();
void _print(){
	setactivepage(page);
	setvisualpage(1-page);
	cleardevice();
	setcolor(YELLOW);
	circle(x,y,radius_ball);
	setfillstyle(SOLID_FILL,WHITE);
	floodfill(x,y,YELLOW);
	setcolor(WHITE);
	setlinestyle(0,0,1); 
	line((X+X0)/2,Y0,(X+X0)/2,Y);
	setlinestyle(0,0,3); 
	line(X0,Y0,X0,(Y+Y0)/2-goal_height); 
	line(X0,(Y+Y0)/2+goal_height,X0,Y); 
	line(X,Y0,X,(Y+Y0)/2-goal_height); 
	line(X,(Y0+Y)/2+goal_height,X,Y); 
	line(X0,Y0,X,Y0); 
	line(X0,Y,X,Y); 
	line(X0-goal_width,(Y+Y0)/2-goal_height,X0,(Y+Y0)/2-goal_height); 
	line(X0-goal_width,(Y+Y0)/2+goal_height,X0,(Y+Y0)/2+goal_height); 
	line(X0-goal_width,(Y+Y0)/2-goal_height,X0-goal_width,(Y+Y0)/2+goal_height); 
	line(X+goal_width,(Y+Y0)/2-goal_height,X,(Y+Y0)/2-goal_height);
	line(X+goal_width,(Y+Y0)/2+goal_height,X,(Y+Y0)/2+goal_height);
	line(X+goal_width,(Y+Y0)/2-goal_height,X+goal_width,(Y+Y0)/2+goal_height);
	setlinestyle(2,0,1);
	line(X0,(Y+Y0)/2-goal_height,X0,(Y+Y0)/2+goal_height);
	line(X,(Y+Y0)/2-goal_height,X,(Y+Y0)/2+goal_height);
	setlinestyle(0,0,3);
	setcolor(WHITE);
	circle(x1,y1,radius);
	setfillstyle(SOLID_FILL,striker_color1);
	floodfill(x1,y1,WHITE);
	circle(x2,y2,radius);
	setfillstyle(SOLID_FILL,striker_color2);
	floodfill(x2,y2,WHITE);
	// Number of Goals:
	char c[]="Goals 1: ";
    outtextxy(10*Xratio,10*Yratio,c);
    char c1[4];
    sprintf(c1, "%d", goal1);
    outtextxy(125*Xratio,10*Yratio,c1);
    char ttime[4];
    char tt[]="Time:";
    outtextxy(X/2-40*Xratio,10*Yratio,tt);
    char ttm[4];
    sprintf(ttm,"%d",tme);
    outtextxy(X/2+40*Xratio,10*Yratio,ttm);
    int xt=tme,sz=0;
    while(xt>0){
		xt/=10;
		sz++;
	}
    char ss[]="s";
    outtextxy(X/2+40*Xratio+sz*15*Xratio,10*Yratio,ss);
    char s2[]="s";
    int xc=goal2,sze=0;
    while(xc>0){
    	xc/=10;
    	sze++;
	}
	char ce[]="Goals 2: ";
	sze=max(1,sze);
    outtextxy(100*Xratio+X-130*Xratio-15*sze*Xratio,10*Yratio,ce);
	char c2[4];
    sprintf(c2, "%d", goal2);
    outtextxy(100*Xratio+X-15*Xratio-15*sze*Xratio,10*Yratio,c2);
    page=1-page;
}
void gola(int who){
	goal=1;
	if(who==1){
		goal2++;
		int ss=speed_ball;
		while(x>X0-3*goal_width/4 && (y>=(Y+Y0)/2-goal_height+radius_ball) && (y<=(Y+Y0)/2+goal_height-radius_ball)){
			_print();
			speed_ball=3;
			move_ball();
			usleep(10*ms);
		}
		speed_ball=ss;
		x1=X0+radius+10*Xratio;
		y1=(Y+Y0)/2;
		x=(X+X0)/2;
		y=(Y+Y0)/2;
		x2=X-radius-10*Xratio;
		y2=(Y+Y0)/2;
		angle=90.0;
	}
	else{
		goal1++;
		int ss=speed_ball;
		while(x<X+3*goal_width/4 && (y>=(Y+Y0)/2-goal_height+radius_ball) && (y<=(Y+Y0)/2+goal_height-radius_ball)){
			_print();
			speed_ball=3;
			move_ball();
			usleep(10*ms);
		}
		speed_ball=ss;
		x1=X0+radius+10*Xratio;
		y1=(Y+Y0)/2;
		x=(X+X0)/2;
		y=(Y+Y0)/2;
		x2=X-radius-10*Xratio;
		y2=(Y+Y0)/2;
		angle=90.0;
	}
	goal=0;
	setvisualpage(1-page);
	setcolor(13);
	settextstyle(0,0,5);
	char c[]="GOAL !!";
	outtextxy((X0+X)/2-120*Xratio,(Y0+Y)/2-50*Yratio,c);
	settextstyle(0,0,2);
	sleep(2);
}
void boundary(){
	if(x<=(radius_ball+X0) && (y>=(Y+Y0)/2-goal_height+radius_ball) && (y<=(Y+Y0)/2+goal_height-radius_ball)){
		gola(1);
		return;
	}
	else if((x+radius_ball)>=X && (y>=(Y+Y0)/2-goal_height+radius_ball) && (y<=(Y+Y0)/2+goal_height-radius_ball)){
		gola(2);
		return;
	}
	if(x<=(radius_ball+X0) || x+radius_ball>=X ) angle=fmod(fmod(180.0-angle,360.0)+360.0,360.0);
	if(y<=(radius_ball+Y0) || y+radius_ball>=Y) angle=fmod(fmod(360.0-angle,360.0)+360.0,360.0);
}
int check_collision(){
	int ans=0;
	if((x1-x)*(x1-x)+(y1-y)*(y1-y)<(radius+radius_ball)*(radius+radius_ball)) ans++;
	if((x2-x)*(x2-x)+(y2-y)*(y2-y)<(radius+radius_ball)*(radius+radius_ball)) ans+=2;
	if((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)<=4*radius*radius) ans+=3;
	return ans;
}
void collision(){
	if((x1-x)*(x1-x)+(y1-y)*(y1-y)<=(radius+radius_ball)*(radius+radius_ball)){
		double theta=deg(atan((y1-y)*1.0/(x-x1))); //update
		if(y==y1) theta=(x1>x)*(180);
		else if(x==x1) theta=90+(y>y1)*(180);
		else if(x1>x) theta+=180.0; //update
		double anglee=angle;
		angle=fmod(fmod(180.0+2*theta-angle,360.0)+360.0,360.0);
		int incx=round(speed_ball*cos(rad(anglee))),incy=round(speed_ball*sin(rad(anglee)));
		if(abs(anglee-angle)>10.0){
			x-=incx;
			y+=incy;
		}
		else move_ball(); //Trying to debug tangential collisions with striker moving towards them
	}
	else if((x2-x)*(x2-x)+(y2-y)*(y2-y)<=(radius+radius_ball)*(radius+radius_ball)){
		double theta=deg(atan((y2-y)*1.0/(x-x2))); // update
		if(y==y2) theta=(x2>x)*(180.0);
		else if(x==x2) theta=90.0+(y>y2)*(180.0);
		else if(x2>x) theta+=180.0; //update
		double anglee=angle;
		angle=fmod(fmod(180+2*theta-angle,360.0)+360.0,360.0);
		int incx=round(speed_ball*cos(rad(anglee))),incy=round(speed_ball*sin(rad(anglee)));
		if(abs(anglee-angle)>10.0){
			x-=incx;
			y+=incy;
		}
		else move_ball(); //Trying to debug tangential collisions with striker moving towards them
	}
}
void move_ball(){
	int incx=round(speed_ball*cos(rad(angle))),incy=round(speed_ball*sin(rad(angle)));
	x+=incx;
	y-=incy;
	if(goal==0 && (y<((Y+Y0)/2-goal_height+radius) || y>((Y+Y0)/2+goal_height-radius))) x=max(x,X0+radius_ball);
	if(goal==0 && (y<((Y+Y0)/2-goal_height+radius) || y>((Y+Y0)/2+goal_height-radius))) x=min(x,X-radius_ball);
	if(goal==0) y=max(y,Y0+radius_ball);
	if(goal==0) y=min(y,Y-radius_ball);
}
void control_striker(){
	int incx=0,incy=0;
	if(GetAsyncKeyState('W')) incy=speed;
	if(GetAsyncKeyState('A')) incx=-speed;
	if(GetAsyncKeyState('S')) incy=-speed;
	if(GetAsyncKeyState('D')) incx=speed;
	x1+=incx;
	y1-=incy;
	y1=max(y1,Y0+radius);
	y1=min(y1,Y-radius);
	x1=max(x1,X0+radius);
	x1=min(x1,(X+X0)/2-radius);
	if(check_collision()>=1){
		x1-=incx;
		y1+=incy;
	}
	if(incx==0) angle1=(incy>0)*90;
	else if(incy==0) angle1=(incx<0)*180;
	else angle1=fmod(deg(atan(incy/incx))+360.0,360.0);
	incx=incy=0;
	if(GetAsyncKeyState(VK_RIGHT)) incx=speed;
	if(GetAsyncKeyState(VK_LEFT)) incx=-speed;
	if(GetAsyncKeyState(VK_UP)) incy=speed;
	if(GetAsyncKeyState(VK_DOWN)) incy=-speed;
	x2+=incx;
	y2-=incy;
	y2=max(y2,Y0+radius);
	y2=min(y2,Y-radius);
	x2=max(x2,(X+X0)/2+radius);
	x2=min(x2,X-radius);
	if(check_collision()>=2){
		x2-=incx;
		y2+=incy;
	}
	if(incx==0) angle2=(incy>0)*90;
	else if(incy==0) angle2=(incx<0)*180;
	else angle2=fmod(deg(atan(incy/incx))+360.0,360.0);
}
int main(){
	initwindow( X , Y , "",-3,-3);
	start();
	X-=100*Xratio;
	Y-=50*Yratio;
	settextstyle(0,0,2);
	int flag=0;
	time_t t0,t1;
	time( &t0 );
	setbkcolor(2);
	while(1){
		if(GetAsyncKeyState(VK_ESCAPE) || GetAsyncKeyState(VK_SPACE)) pause();
		time(&t1);
		if(t0!=t1){
			t0=t1;
			tme++;
		}
		_print();
		move_ball();
		boundary();
		collision();
		control_striker();
		if(GetAsyncKeyState('X')) break;
		usleep(10*ms);
	}
	setvisualpage(1-page);
	setcolor(WHITE);
	char c[]="BYE !! ";
    settextstyle(EUROPEAN_FONT,HORIZ_DIR,2);
    outtextxy(600*Xratio,300*Yratio,c);
    char c1[]="BYE!!";
    settextstyle(SANS_SERIF_FONT,HORIZ_DIR,4);
    outtextxy(600*Xratio,400*Yratio,c1);
    sleep(2);
    closegraph();
	return 0;
}
