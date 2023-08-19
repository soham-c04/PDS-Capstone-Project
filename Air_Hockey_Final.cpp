#include <graphics.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

const double pi=2*acos(0.0);

int width=0,height=0;
int X_Full=0,Y_Full=0;
int X=10,Y=100;
int goal_width=90,goal_height=125;
int X1=0,Y1=0;//coordinates of top left corner of playable field
int X2=0,Y2=0;//coordinates of bottom right corner of playable field

#define min(x,y) ((x)>(y))? (y):(x)
#define max(x,y) ((x)>(y))? (x):(y)
#define Xratio X_Full/1280 
#define Yratio Y_Full/720  

int radius_striker=50,radius_ball=25;//radius of striker and ball are set
int x1=0,y1=0;//coordinates of first striker
int x2=0,y2=0;//coordinates of second striker
int xb=0,yb=0;//coordinates of ball

int speed_striker=10,speed_ball=20;//fixed speed of striker and ball
double angle=0.0;

int color_1=1,color_2=4;//color of first and second striker respectively
int goal_1=0,goal_2=0;//goal counter for first and second player respectively
int Time=0;
int goal=0;//checks goal condition
int page=0;//for double buffering

double rad(int degree)//converts degree to radian
{
	return pi*degree/180.0;
}
double deg(double radi)//converts radian to degree
{
	return 180.0*radi/pi;
}
void print_game();
void move_ball();
void goaling(int who)//checks goal condition and updates value if goal is scored
{
	goal=1;
	if(who==1)//gone into first players goal
	{
		goal_2++;//player 2 scores
		int ss=speed_ball;
		while(xb>X1-3*goal_width/4&&(yb>=(Y2+Y1)/2-goal_height+radius_ball)&&(yb<=(Y2+Y1)/2+goal_height-radius_ball))
		{
			print_game();
			speed_ball=3;
			move_ball();
			delay(10);
		}
		speed_ball=ss;
		x1=X1+radius_striker+10*Xratio;//reset striker 1 X coordinate
		y1=(Y2+Y1)/2;//reset striker 1 Y coordinate
		xb=(X2+X1)/2;//reset ball X coordinate to middle of playable field
		yb=(Y2+Y1)/2;//reset ball Y coordinate to middle of playable field
		x2=X2-radius_striker-10*Xratio;//reset striker 2 X coordinate
		y2=(Y2+Y1)/2;//reset striker 2 Y coordinate
		angle=90.0;//moves ball vertically next round so both players get fair chance to score
	}
	else
	{
		goal_1++;//player 1 scores
		int ss=speed_ball;
		while(xb<X2+3*goal_width/4 &&(yb>=(Y2+Y1)/2-goal_height+radius_ball)&&(yb<=(Y2+Y1)/2+goal_height-radius_ball))
		{
			print_game();
			speed_ball=3;
			move_ball();
			delay(10);
		}
		speed_ball=ss;
		x1=X1+radius_striker+10*Xratio;//reset striker 1 X coordinate
		y1=(Y2+Y1)/2;//reset striker 1 Y coordinate
		xb=(X2+X1)/2;//reset ball X coordinate to middle of playable field
		yb=(Y2+Y1)/2;//reset ball Y coordinate to middle of playable field
		x2=X2-radius_striker-10*Xratio;//reset striker 2 X coordinate
		y2=(Y2+Y1)/2;//reset striker 2 Y coordinate
		angle=90.0;//moves ball vertically next round so both players get fair chance to score
	}
	goal=0;//resets goal condition
	setvisualpage(1-page);
	readimagefile("resources/goal.jpg",(X1+X2)/2-150,(Y1+Y2)/2-75,(X1+X2)/2+150,(Y1+Y2)/2+75);
	settextstyle(4,HORIZ_DIR,3);//set text style to previous one
	sleep(2);
}
void boundary()
{
	if(xb<=(radius_ball+X1)&&(yb>=(Y2+Y1)/2-goal_height+radius_ball)&&(yb<=(Y2+Y1)/2+goal_height-radius_ball))
	{
		goaling(1);
		return;
	}
	else if((xb+radius_ball)>=X2&&(yb>=(Y2+Y1)/2-goal_height+radius_ball)&&(yb<=(Y2+Y1)/2+goal_height-radius_ball))
	{
		goaling(2);
		return;
	}
	if(xb<=(radius_ball+X1)||xb+radius_ball>=X2) 
	{
		angle=fmod(fmod(180.0-angle,360.0)+360.0,360.0);
	}
	if(yb<=(radius_ball+Y1) || yb+radius_ball>=Y2) 
	{	
		angle=fmod(fmod(360.0-angle,360.0)+360.0,360.0);
	}
}
int check_collision()//function to check collision of ball and strikers
{
	int ans=0;
	if(pow(x1-xb,2)+pow(y1-yb,2)<pow(radius_striker+radius_ball,2))//collision of ball with first striker
		ans++;
	if(pow(x2-xb,2)+pow(y2-yb,2)<pow(radius_striker+radius_ball,2))//collision of ball with second striker
		ans+=2;
	if(pow(x1-x2,2)+pow(y1-y2,2)<=pow(2*radius_striker,2))
		ans+=3;
	return ans;
}
void collision()
{
	if(pow(x1-xb,2)+pow(y1-yb,2)<=pow(radius_striker+radius_ball,2))
	{
		double theta=deg(atan((y1-yb)*1.0/(xb-x1)));
		if(yb==y1)
		{ 
			theta=(x1>xb)*(180);
		}
		else if(xb==x1)
		{ 
			theta=90+(yb>y1)*(180);
		}
		else if(x1>xb)
		{
			theta+=180.0;
		}
		double anglee=angle;
		angle=fmod(fmod(180.0+2*theta-angle,360.0)+360.0,360.0);
		int incx=round(speed_ball*cos(rad(anglee))),
		incy=round(speed_ball*sin(rad(anglee)));
		if(abs(anglee-angle)>10.0)
		{
			xb-=incx;
			yb+=incy;
		}
		else
		{ 
			move_ball(); //Trying to debug tangential collisions with striker moving towards them
		}
	}
	else if(pow(x2-xb,2)+pow(y2-yb,2)<=pow(radius_striker+radius_ball,2))
	{
		double theta=deg(atan((y2-yb)*1.0/(xb-x2)));
		if(yb==y2)
		{ 
			theta=(x2>xb)*(180.0);
		}
		else if(xb==x2)
		{ 
			theta=90.0+(yb>y2)*(180.0);
		}
		else if(x2>xb)
		{
			theta+=180.0;
		}
		double anglee=angle;
		angle=fmod(fmod(180+2*theta-angle,360.0)+360.0,360.0);
		int incx=round(speed_ball*cos(rad(anglee))),
		incy=round(speed_ball*sin(rad(anglee)));
		if(abs(anglee-angle)>10.0)
		{
			xb-=incx;
			yb+=incy;
		}
		else
		{ 
			move_ball(); //Trying to debug tangential collisions with striker moving towards them
		}
	}
}
void move_ball()
{
	int incx=round(speed_ball*cos(rad(angle)));//X component of speed of ball
	int incy=round(speed_ball*sin(rad(angle)));//Y component of speed of ball
	xb+=incx;//increment in X coordinate of ball
	yb-=incy;//increment in Y coordinate of ball
	if(goal==0 && (yb<((Y2+Y1)/2-goal_height+radius_ball)||yb>((Y2+Y1)/2+goal_height-radius_ball)))
	xb=max(xb,X1+radius_ball);
	if(goal==0 && (yb<((Y2+Y1)/2-goal_height+radius_ball)||yb>((Y2+Y1)/2+goal_height-radius_ball))) 
	xb=min(xb,X2-radius_ball);
	if(goal==0) 
	yb=max(yb,Y1+radius_ball);
	if(goal==0) 
	yb=min(yb,Y2-radius_ball);
}
void control_striker()
{
	int incx=0,incy=0;
	if(GetAsyncKeyState('W')) 
	{
		incy=speed_striker;
	}
	if(GetAsyncKeyState('A')) 
	{
		incx=-speed_striker;
	}
	if(GetAsyncKeyState('S')) 
	{	
		incy=-speed_striker;
	}
	if(GetAsyncKeyState('D'))
	{ 
		incx=speed_striker;
	}
	x1+=incx;
	y1-=incy;
	y1=max(y1,Y1+radius_striker);
	y1=min(y1,Y2-radius_striker);
	x1=max(x1,X1+radius_striker);
	x1=min(x1,(X2+X1)/2-radius_striker);
	if(check_collision()>=1)
	{
		x1-=incx;
		y1+=incy;
	}
	incx=incy=0;
	if(GetAsyncKeyState(VK_RIGHT)) 
	{
		incx=speed_striker;
	}
	if(GetAsyncKeyState(VK_LEFT)) 
	{
		incx=-speed_striker;
	}
	if(GetAsyncKeyState(VK_UP)) 
	{
		incy=speed_striker;
	}	
	if(GetAsyncKeyState(VK_DOWN)) 
	{
		incy=-speed_striker;
	}
	x2+=incx;
	y2-=incy;
	y2=max(y2,Y1+radius_striker);
	y2=min(y2,Y2-radius_striker);
	x2=max(x2,(X2+X1)/2+radius_striker);
	x2=min(x2,X2-radius_striker);
	if(check_collision()>=2)
	{
		x2-=incx;
		y2+=incy;
	}
}
void settings()
{
	cleardevice();
	setcolor(1);
	setlinestyle(0,0,5);
	rectangle(50,50,width-50,height-50);
	setfillstyle(SOLID_FILL,8);
	floodfill(width/2,height/2,1);
	char c1[]="Choose color for PLAYER 1:";
	settextstyle(3,HORIZ_DIR,7);
	outtextxy(width/4-100,100,c1);//prints text for player 1 to choose color
	char c2[]="Press 1 to choose";
	settextstyle(3,HORIZ_DIR,4);
	outtextxy(width/4-75,height-150,c2);
	char c3[]="Press 2 to choose";
	outtextxy(3*width/4-225,height-150,c3);
	setcolor(WHITE);
	setfillstyle(SOLID_FILL,1);//color choice 1
	circle(width/4+75,height/2+25,radius_striker*2.5);//prints first circle
	floodfill(width/4+75,height/2+25,WHITE);//fills circle 1 with first color choice
	circle(3*width/4-75,height/2+25,radius_striker*2.5);//prints second circle
	setfillstyle(SOLID_FILL,3);//color choice 2
	floodfill(3*width/4-75,height/2+25,WHITE);//fills circle 2 with second color choice
	while(1){
		if(GetAsyncKeyState('1'))
		{
			color_1=1;//assigns color choice 1 to striker 1
			break;
		}
		else if(GetAsyncKeyState('2'))
		{
			color_1=3;//assigns color choice 2 to striker 1
			break;
		}
		delay(10);
	}
	while(GetAsyncKeyState('1') || GetAsyncKeyState('2'))//to avoid misckicking of keys
	{
		delay(10);
	}
	cleardevice();
	setcolor(1);
	setlinestyle(0,0,5);
	rectangle(50,50,width-50,height-50);//outer border
	setfillstyle(SOLID_FILL,8);
	floodfill(width/2,height/2,1);
	char c4[]="Choose color for PLAYER 2:";//prints text for player 2 to chose color
	settextstyle(3,HORIZ_DIR,7);
	outtextxy(width/4-100,100,c4);
	char c5[]="Press 1 to choose";
	settextstyle(3,HORIZ_DIR,4);
	outtextxy(width/4-75,height-150,c5);
	char c6[]="Press 2 to choose";
	outtextxy(3*width/4-225,height-150,c6);
	setcolor(WHITE);
	setfillstyle(SOLID_FILL,4);//color choice 1
	circle(width/4+75,height/2+25,radius_striker*2.5);//prints first circle
	floodfill(width/4+75,height/2+25,WHITE);//fills circle 1 with first color choice
	circle(3*width/4-75,height/2+25,radius_striker*2.5);//prints second cirlce
	setfillstyle(SOLID_FILL,5);//color choice 2
	floodfill(3*width/4-75,height/2+25,WHITE);//fills circle 2 with second color choice
	while(1)
	{
		if(GetAsyncKeyState('1'))
		{
			color_2=4;//assigns color choice 1 to striker 2
			break;
		}
		else if(GetAsyncKeyState('2'))
		{
			color_2=5;//assigns color choice 2 to striker 2
			break;
		}
		delay(10);
	}
	settextstyle(4,HORIZ_DIR,3);
}
void text()//prints texts while game is running
{
	char g1[3]="00";
	char g2[3]="00";
	char c1[]="TIME",c2[]="PLAYER 1",c3[]="PLAYER 2",c4[]="Press Escape",c5[]="for Menu",c6[]="Press Escape",c7[]="for Menu";
		g1[1]=(char)(goal_1%10+'0');
		g1[0]=(char)(goal_1/10+'0');
		g2[1]=(char)(goal_2%10+'0');
		g2[0]=(char)(goal_2/10+'0');
	outtextxy((X1+X2)/2-108,30,g1);
	outtextxy((X1+X2)/2+70,30,g2);
	char t[4];
		sprintf(t,"%d",Time);
		int tme=Time,sz=0;
	    while(tme>0){
			tme/=10;
			sz++;
		}
		sz=max(sz,1);
		outtextxy((X1+X2)/2-42,5,c1);
		outtextxy((X1+X2)/2-10*sz,38,t);
		outtextxy((X1+X2)/2-304,28,c2);
		outtextxy((X1+X2)/2+135,28,c3);
	outtextxy(X,18,c4);
	outtextxy(X,42,c5);
	outtextxy(width-230,18,c6);
	outtextxy(width-150,42,c7);
}
void print_movable()
{
	setcolor(15);
		circle(x1,y1,radius_striker);
		circle(x2,y2,radius_striker);
		circle(xb,yb,radius_ball);
	setfillstyle(SOLID_FILL,color_1);
		floodfill(x1,y1,15);
	setfillstyle(SOLID_FILL,color_2);
		floodfill(x2,y2,15);
	setfillstyle(SOLID_FILL,14);
		floodfill(xb,yb,15);
}
void color()
{
	setfillstyle(SOLID_FILL,2);
		floodfill(X1+2,Y1+2,BLACK);
		floodfill(X2-2,Y2-2,BLACK);
	setfillstyle(SOLID_FILL,9);
		floodfill(X1-10,(Y+height)/2,BLACK);
		floodfill((X1+X2)/2-10,(Y+height)/2,BLACK);
		floodfill((X1+X2)/2-100,40,BLACK);
	setfillstyle(SOLID_FILL,12);
		floodfill(X2+10,(Y+height)/2,BLACK);
		floodfill((X1+X2)/2+10,(Y+height)/2,BLACK);
		floodfill((X1+X2)/2+100,40,BLACK);
	setfillstyle(4,BLACK); // bleachers
		floodfill(X1-2,Y1+2,BLACK);
		floodfill(X1-2,Y2-2,BLACK);
		floodfill(X2+2,Y1+2,BLACK);
		floodfill(X2+2,Y2-2,BLACK);
}
void print_field()
{
	cleardevice();
	setcolor(BLACK);
	setlinestyle(0,0,3);
	//making stadium
	{
		rectangle(X,Y,width,height);
		rectangle(X,(Y+height)/2-goal_height,X+goal_width,(Y+height)/2+goal_height);//making left goal
		rectangle(width-goal_width,(Y+height)/2-goal_height,width,(Y+height)/2+goal_height);//making right goal
		rectangle(X1,Y1,X2,Y2);//making playable field
	}
	line((X1+X2)/2,Y1,(X1+X2)/2,Y2);//central line
	circle((X1+X2)/2,(Y1+Y2)/2,100);//making central circle
		rectangle((X1+X2)/2-50,0,(X1+X2)/2+50,80);//time square
		rectangle(((X1+X2)/2-50)-80,10,(X1+X2)/2-50,70);//score square 1
		rectangle(((X1+X2)/2+50)+80,10,(X1+X2)/2+50,70);//score square 2
		rectangle(((X1+X2)/2-50)-265,20,(X1+X2)/2-130,60);//name square 1
		rectangle(((X1+X2)/2+50)+265,20,(X1+X2)/2+130,60);//name square 2
}
void print_game()
{
	setactivepage(page);
	setvisualpage(1-page);
	print_field();
	color();
	text();
	print_movable();
	page=1-page;
}
void pause_screen(int option)
{
	setactivepage(page);
	setvisualpage(1-page);
	switch(option)
	{
		case 1:
			readimagefile("resources/pause_screen_resume.jpg",(X1+X2)/2-200,(Y1+Y2)/2-200,(X1+X2)/2+200,(Y1+Y2)/2+200);
			break;
		case 2:
			readimagefile("resources/pause_screen_reset.jpg",(X1+X2)/2-200,(Y1+Y2)/2-200,(X1+X2)/2+200,(Y1+Y2)/2+200);
			break;
		case 3:
			readimagefile("resources/pause_screen_settings.jpg",(X1+X2)/2-200,(Y1+Y2)/2-200,(X1+X2)/2+200,(Y1+Y2)/2+200);
			break;
		case 4:
			readimagefile("resources/pause_screen_quit.jpg",(X1+X2)/2-200,(Y1+Y2)/2-200,(X1+X2)/2+200,(Y1+Y2)/2+200);
			break;
		default:
			readimagefile("resources/pause_screen_resume.jpg",(X1+X2)/2-200,(Y1+Y2)/2-200,(X1+X2)/2+200,(Y1+Y2)/2+200);
	}
	page=1-page;
}
int pause()
{
	int option=1;
	while(1)
	{
		if(GetAsyncKeyState(VK_UP))
		{
			option--;
			if(option<1)
				option=4;
		}
		else if(GetAsyncKeyState(VK_DOWN))
		{
			option++;
			if(option>4)
				option=1;
		}
		else if(GetAsyncKeyState(VK_RETURN))
		{
			switch(option)
			{
				case 1:
					return 1;
					break;
				case 2:
					goal_1=0,goal_2=0,Time=0;
					angle=0.0;
					x1=X1+radius_striker+10,y1=(Y1+Y2)/2;
					x2=X2-radius_striker-10,y2=(Y1+Y2)/2;
					xb=(X1+X2)/2,yb=(Y1+Y2)/2;
					return 1;
					break;
				case 3:
					setvisualpage(1-page);
					settings();
					return 1;
					break;
				case 4:
					return 0;
					break;
				default:
					return 1;
			}
		}
		else if(GetAsyncKeyState(VK_ESCAPE))
		{	
			while(GetAsyncKeyState(VK_ESCAPE));
			return 1;
		}
		pause_screen(option);
		delay(100);
	}
}
int game()
{
	time_t ti,tf;
	time(&ti);
	while(1)
	{
		time(&tf);
		if(ti!=tf)
		{//incrementing time
			ti=tf;
			Time++;
		}
		{//printing game screen
			print_game();
		}
		{//physics	
			move_ball();
			boundary();
			collision();
			control_striker();
		}
		if(GetAsyncKeyState(VK_ESCAPE))
		{
			while(GetAsyncKeyState(VK_ESCAPE)) delay(10);
			int run_game=pause();
			if(run_game==0)
			{
				break;
			}
		}
		delay(10);
	}
	getch();
	closegraph();
}
void start()
{
	setbkcolor(7);
	cleardevice();
	setcolor(BLUE);
	setlinestyle(0,0,7);
	rectangle(50,50,width-40,height-40);
	setlinestyle(0,0,3);
	rectangle(60,60,width-50,height-50);
	settextstyle(3,HORIZ_DIR,10);
	char c1[]="AIR HOCKEY";
	outtextxy(width/2-300,height/2-150,c1);
	setcolor(9);
	settextstyle(3,HORIZ_DIR,5);
	char c2[]="Press ENTER to play!";
	outtextxy(width/2-225,height/2,c2);
	settextstyle(3,HORIZ_DIR,2);
	char c3[]="DEVELOPED BY:";
	outtextxy(width/4-225,3*height/4-46,c3);
	char c4[]="Aditya Dhananjay Singh";
	outtextxy(width/4-225,3*height/4-23,c4);
	char c5[]="Atharva Atul Penkar";
	outtextxy(width/4-225,3*height/4,c5);
	char c6[]="Harshil Singh";
	outtextxy(width/4-225,3*height/4+23,c6);
	char c7[]="Kumar Snehal";
	outtextxy(width/4-225,3*height/4+46,c7);
	char c8[]="Soham Chakraborty";
	outtextxy(width/4-225,3*height/4+69,c8);
	while(1)
	{
		if(GetAsyncKeyState(VK_RETURN))
		{
			settings();
			break;
		}
		else if(GetAsyncKeyState(VK_ESCAPE))
			return;
		delay(10);
	}
	game();
}
int main()
{
	width=GetSystemMetrics(SM_CXSCREEN),height=GetSystemMetrics(SM_CYSCREEN);
	X_Full=width,Y_Full=height;
	X1=X+goal_width,Y1=Y;
	X2=width-goal_width-10,Y2=height-20;
	x1=X1+radius_striker+10,y1=(Y1+Y2)/2;
	x2=X2-radius_striker-10,y2=(Y1+Y2)/2;
	xb=(X1+X2)/2,yb=(Y1+Y2)/2;
	
	initwindow(width,height,"",-3,-3);
	cleardevice();
	readimagefile("resources/opening_screen.jpg",0,0,width,height);
	height-=20;
	width-=10;
	sleep(5);
	cleardevice();
	setbkcolor(7);
	start();
	return 0;
}
