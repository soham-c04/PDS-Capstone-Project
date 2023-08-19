#include <graphics.h>
#include <math.h>
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
#define number 5

int X; 
int Y; 
int X_full,Y_full; 
int X0,Y0;
int goal_height,goal_width;
int goal1=0,goal2=0,goal=0,tme=0,page=0;
int radius,radius_ball;
double max_speed;
double angle[2*number];
double striker_speed[2*number];
int x[2*number],y[2*number];
double ball_angle=0.0; // This is angle of ball
double speed_ball=0.0;
double arc_angle=360.0;
double movement=0;
int moves=0;
int go=0;
int ballx,bally;
double Mb=1.0,Ms=7.0;//radius*radius/(radius_ball*radius_ball);
double e=1; //Coefficient of restitution
int turn=-1;
int collisions=0;
time_t ti,tf;
int i;
POINT mouse;

void Time(){
	time(&tf);
	if(ti!=tf){
		ti=tf;
		tme++;
	}
}
void move_all();
void _print(int e){
	setactivepage(page);
	setvisualpage(1-page);
	cleardevice();
	setlinestyle(0,0,3);
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
	setcolor(1);
	line(X0-goal_width,(Y+Y0)/2-goal_height,X0,(Y+Y0)/2-goal_height); 
	line(X0-goal_width,(Y+Y0)/2+goal_height,X0,(Y+Y0)/2+goal_height); 
	line(X0-goal_width,(Y+Y0)/2-goal_height,X0-goal_width,(Y+Y0)/2+goal_height); 
	setcolor(4);
	line(X+goal_width,(Y+Y0)/2-goal_height,X,(Y+Y0)/2-goal_height);
	line(X+goal_width,(Y+Y0)/2+goal_height,X,(Y+Y0)/2+goal_height);
	line(X+goal_width,(Y+Y0)/2-goal_height,X+goal_width,(Y+Y0)/2+goal_height);
	setlinestyle(0,0,1);
	setcolor(1);
	line(X0-1,(Y+Y0)/2-goal_height,X0,(Y+Y0)/2+goal_height);
	setfillstyle(SOLID_FILL,9);
	floodfill(X0-3,(Y+Y0)/2,1);
	setcolor(4);
	line(X+1,(Y+Y0)/2-goal_height,X,(Y+Y0)/2+goal_height);
	setfillstyle(SOLID_FILL,12);
	floodfill(X+3,(Y+Y0)/2,4);
	// Making D
	setcolor(WHITE);
		line(X0,(Y+Y0)/2-goal_height,X0,(Y+Y0)/2+goal_height);
	line(X,(Y+Y0)/2-goal_height,X,(Y+Y0)/2+goal_height);
	line(X0,(Y+Y0)/2-goal_height,X0+50+2*radius,(Y+Y0)/2-goal_height);
	line(X0,(Y+Y0)/2+goal_height,X0+50+2*radius,(Y+Y0)/2+goal_height);
	line(X,(Y+Y0)/2-goal_height,X-50-2*radius,(Y+Y0)/2-goal_height);
	line(X,(Y+Y0)/2+goal_height,X-50-2*radius,(Y+Y0)/2+goal_height);
	line(X0+50+2*radius,(Y+Y0)/2-goal_height,X0+50+2*radius,(Y+Y0)/2+goal_height);
	line(X-50-2*radius,(Y+Y0)/2-goal_height,X-50-2*radius,(Y+Y0)/2+goal_height);
	arc(X0,(Y+Y0)/2,-90,90,3*goal_height/4);
	arc(X,(Y+Y0)/2,90,-90,3*goal_height/4);
	setfillstyle(SOLID_FILL,WHITE);
	floodfill(X0+5,(Y+Y0)/2,WHITE);
	floodfill(X-5,(Y+Y0)/2,WHITE);
	setcolor(10);
	setfillstyle(SOLID_FILL,10);
	for(i=0;i<number;i++){
		circle(x[i],y[i],radius);
		floodfill(x[i],y[i],10);
	}
	setcolor(11);
	setfillstyle(SOLID_FILL,11);
	for(i=number;i<2*number;i++){
		circle(x[i],y[i],radius);
		floodfill(x[i],y[i],11);
	}
	setlinestyle(0,0,3);
	setcolor(YELLOW);
	circle(ballx,bally,radius_ball);
	setfillstyle(SOLID_FILL,WHITE);
	floodfill(ballx,bally,YELLOW);
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
    if(movement==0){
    	setlinestyle(0,0,5);
    	setcolor(13);
    	if(turn==0) for(i=0;i<number;i++) arc(x[i],y[i],0,arc_angle,radius+4); 
		else if(turn==1) for(i=number;i<2*number;i++) arc(x[i],y[i],0,arc_angle,radius+4); 
		arc_angle-=1.0/3;
	}
    if(e!=-1){
		GetCursorPos(&mouse);
		int releasepos_x=mouse.x,releasepos_y=mouse.y;
		double anglee=deg(atan((releasepos_y-y[e])*1.0/(x[e]-releasepos_x)));
		if(releasepos_y==y[e]) anglee=(x[e]<releasepos_x)*(180.0);
		else if(releasepos_x==x[e]) anglee=90.0+(releasepos_y<y[e])*(180.0);
		else if(releasepos_x>x[e]) anglee+=180.0;
		double distance=round(sqrt(pow(releasepos_x-x[e],2)+pow(releasepos_y-y[e],2)));
		distance=min(distance,max_speed*20.0);
		distance/=2;
		setlinestyle(0,0,5);
		anglee=rad(anglee);
		if(distance>radius){
			setcolor(YELLOW);
			line(x[e]+round(radius*cos(anglee)),y[e]-round(radius*sin(anglee)),x[e]+round(distance*cos(anglee)),y[e]-round(distance*sin(anglee)));
			line(x[e]+round(distance*cos(anglee))-round(distance*cos(anglee+rad(30.0))/4),y[e]-round(distance*sin(anglee))+round(distance*sin(anglee+rad(30.0))/4),x[e]+round(distance*cos(anglee)),y[e]-round(distance*sin(anglee)));
			line(x[e]+round(distance*cos(anglee))-round(distance*cos(anglee-rad(30.0))/4),y[e]-round(distance*sin(anglee))+round(distance*sin(anglee-rad(30.0))/4),x[e]+round(distance*cos(anglee)),y[e]-round(distance*sin(anglee)));
			setlinestyle(1,0,2);
			setcolor(WHITE);
			line(x[e]+round(4*distance*cos(anglee)/3),y[e]-round(4*distance*sin(anglee)/3),x[e]+round(distance*cos(anglee)),y[e]-round(distance*sin(anglee)));
			setcolor(BLACK);
			setlinestyle(1,0,3);
			line(x[e]-round(radius*cos(anglee)),y[e]+round(radius*sin(anglee)),x[e]-round((radius+distance/3)*cos(anglee)),y[e]+round((distance/3+radius)*sin(anglee)));
		}
		else{
			setcolor(RED);
			line(x[e],y[e],x[e]+round(distance*cos(anglee)),y[e]-round(distance*sin(anglee)));
		}
	}
    page=1-page;
}
void boundary(){
	if(ballx<=(radius_ball+X0) || ballx+radius_ball>=X ) ball_angle=fmod(fmod(180.0-ball_angle,360.0)+360.0,360.0);
	if(bally<=(radius_ball+Y0) || bally+radius_ball>=Y) ball_angle=fmod(fmod(360.0-ball_angle,360.0)+360.0,360.0);
	for(i=0;i<2*number;i++){
		if(x[i]<=(radius+X0) || x[i]+radius>=X){
			x[i]=min(x[i],X-radius);
			x[i]=max(x[i],radius+X0);
			angle[i]=fmod(fmod(180.0-angle[i],360.0)+360.0,360.0);
		}
		if(y[i]<=(radius+Y0) || y[i]+radius>=Y){
			y[i]=min(y[i],Y-radius);
			y[i]=max(y[i],radius+Y0);
			angle[i]=fmod(fmod(360.0-angle[i],360.0)+360.0,360.0);
		}
	}
}
int check_collision(){
	int ans=0;
	for(i=0;i<2*number;i++){
		int incxb=round(speed_ball*cos(rad(ball_angle))),incyb=round(speed_ball*sin(rad(ball_angle)));
		int incxs=round(striker_speed[i]*cos(rad(angle[i]))),incys=round(striker_speed[i]*sin(rad(angle[i])));
		if((pow((x[i]+incxs)-(ballx+incxb),2)+pow((y[i]-incys)-(bally-incyb),2))<=(radius+radius_ball)*(radius+radius_ball)){
			ans=1;
			double theta=deg(atan((bally-y[i])*1.0/(x[i]-ballx)));
			if(bally==y[i]) theta=(x[i]>ballx)*(180.0);
			else if(ballx==x[i]) theta=90.0+(bally>y[i])*(180.0);
			else if(ballx>x[i]) theta+=180.0;
			double vbi_parallel=speed_ball*cos(rad(ball_angle-theta)),vb_perp=speed_ball*sin(rad(ball_angle-theta));
			double vsi_parallel=striker_speed[i]*cos(rad(angle[i]-theta)),vs_perp=striker_speed[i]*sin(rad(angle[i]-theta));
			double vbf_parallel=((1+e)*Ms*vsi_parallel+vbi_parallel*(Mb-e*Ms))/(Mb+Ms);
			double vsf_parallel=((1+e)*Mb*vbi_parallel+vsi_parallel*(Ms-e*Mb))/(Mb+Ms);
			ball_angle=theta+deg(atan(vb_perp/vbf_parallel));
			angle[i]=theta+deg(atan(vs_perp/vsf_parallel));
			speed_ball=sqrt(pow(vbf_parallel,2)+pow(vb_perp,2));
			striker_speed[i]=sqrt(pow(vsf_parallel,2)+pow(vs_perp,2));
			if(vbf_parallel<0) ball_angle+=180.0;
			if(vsf_parallel<0) angle[i]+=180.0;
		}
		int j;
		for(j=i+1;j<2*number;j++){
			int incxi=round(striker_speed[i]*cos(rad(angle[i]))),incyi=round(striker_speed[i]*sin(rad(angle[i])));
			int incxj=round(striker_speed[j]*cos(rad(angle[j]))),incyj=round(striker_speed[j]*sin(rad(angle[j])));
			if(pow((x[i]+incxi)-(x[j]+incxj),2)+pow((y[i]-incyi)-(y[j]-incyj),2)<=4*radius*radius){
				ans=1;
				double theta=deg(atan((y[j]-y[i])*1.0/(x[i]-x[j])));
				if(y[j]==y[i]) theta=(x[i]>x[j])*(180.0);
				else if(x[j]==x[i]) theta=90.0+(y[j]>y[i])*(180.0);
				else if(x[j]>x[i]) theta+=180.0;
				
				double vji_parallel=striker_speed[j]*cos(rad(angle[j]-theta)),vj_perp=striker_speed[j]*sin(rad(angle[j]-theta));
				double vii_parallel=striker_speed[i]*cos(rad(angle[i]-theta)),vi_perp=striker_speed[i]*sin(rad(angle[i]-theta));
				double vjf_parallel=((1+e)*vii_parallel+vji_parallel*(1-e))/2;
				double vif_parallel=((1+e)*vji_parallel+vii_parallel*(1-e))/2;
				angle[j]=theta+deg(atan(vj_perp/vjf_parallel));
				angle[i]=theta+deg(atan(vi_perp/vif_parallel));
				striker_speed[j]=sqrt(pow(vjf_parallel,2)+pow(vj_perp,2));
				striker_speed[i]=sqrt(pow(vif_parallel,2)+pow(vi_perp,2));
				if(vjf_parallel<0) angle[j]+=180.0;
				if(vif_parallel<0) angle[i]+=180.0;
			}
		}
	}
	return ans;
}
void move_all(){
	int incx=round(speed_ball*cos(rad(ball_angle))),incy=round(speed_ball*sin(rad(ball_angle)));
	ballx+=incx;
	bally-=incy;
	if(goal==0 ) ballx=max(ballx,X0+radius_ball);
	if(goal==0 ) ballx=min(ballx,X-radius_ball);
	if(goal==0) bally=max(bally,Y0+radius_ball);
	if(goal==0) bally=min(bally,Y-radius_ball);
	for(i=0;i<2*number;i++){
		incx=round(striker_speed[i]*cos(rad(angle[i])));
		incy=round(striker_speed[i]*sin(rad(angle[i])));
		x[i]+=incx;
		y[i]-=incy;
		if(goal==1){
			x[i]=min(x[i],X-radius);
			x[i]=max(x[i],X0+radius);
			y[i]=min(y[i],Y-radius);
			y[i]=max(y[i],Y0+radius);
		}
	}
}
void control_striker(){
	moves++;
	arc_angle=360.0;
	int incx=0,incy=0;
	int flag=-1;
	_print(-1);
	while(1){
		if(arc_angle<=0) break;
		while(!GetAsyncKeyState(VK_LBUTTON)){
			if(arc_angle<=0) break;
			Time();
			_print(-1);
			usleep(10*ms);
		}
		GetCursorPos(&mouse);
		if(turn==0){
			for(i=0;i<number;i++){
				if((x[i]-mouse.x)*(x[i]-mouse.x)+(y[i]-mouse.y)*(y[i]-mouse.y)<=radius*radius){
					flag=i;
					break;
				}
			}
		}
		else if(turn==1){
			for(i=number;i<2*number;i++){
				if((x[i]-mouse.x)*(x[i]-mouse.x)+(y[i]-mouse.y)*(y[i]-mouse.y)<=(radius+radius_ball)*(radius+radius_ball)){
					flag=i;
					break;
				}
			}
		}
		if(flag!=-1) break;
		else{
			while(GetAsyncKeyState(VK_LBUTTON)){
				if(arc_angle<=0) break;
				Time();
				_print(-1);
				delay(10);
			}
		}
		usleep(10*ms);
	}
	while(GetAsyncKeyState(VK_LBUTTON)){
		if(arc_angle<=0) break;
		Time();
		_print(flag);
		delay(10);
	}
	i=flag;
	GetCursorPos(&mouse);
	int releasepos_x=mouse.x,releasepos_y=mouse.y;
	double anglee=deg(atan((releasepos_y-y[i])*1.0/(x[i]-releasepos_x)));
	if(releasepos_y==y[i]) anglee=(x[i]<releasepos_x)*(180.0);
	else if(releasepos_x==x[i]) anglee=90.0+(releasepos_y<y[i])*(180.0);
	else if(releasepos_x>x[i]) anglee+=180.0;
	int distance=(arc_angle>0)? (round(sqrt(pow(releasepos_x-x[i],2)+pow(releasepos_y-y[i],2)))):0;
	if(i!=-1) angle[i]=anglee;
	striker_speed[i]=distance/10.0;
	striker_speed[i]=min(striker_speed[i],max_speed);
	if(striker_speed[i]) movement=1;
	go=0;
	movement=1;
	turn=1-turn;
}
int main(){
	X=GetSystemMetrics(SM_CXSCREEN); 
	Y=GetSystemMetrics(SM_CYSCREEN); 
	X_full=X;
	Y_full=Y; 
	X0=100*Xratio;Y0=50*Yratio;
	goal_height=100*Yratio;
	goal_width=90*Xratio;
	radius=40*ratio;radius_ball=20*ratio;
	max_speed=15.0*ratio;
	ballx=X/2;bally=Y/2;
	initwindow( X , Y , "",-3,-3);
	X-=100*Xratio;
	Y-=50*Yratio;
	// Formation:-
	
	x[0]=X0+radius+10*Xratio;
	y[0]=y[0+number]=(Y+Y0)/2;
	x[1]=x[2]=X0+(X-X0)/5;
	y[1]=y[1+number]=Y0+(Y-Y0)/4;
	y[2]=y[2+number]=Y0+(Y-Y0)*3/4;
	x[3]=x[4]=X0+2*(X-X0)/5;
	y[3]=y[3+number]=(Y+Y0)/2-2*goal_height/3;
	y[4]=y[4+number]=(Y+Y0)/2+2*goal_height/3;
	
	x[0+number]=X-radius-10*Xratio;
	x[1+number]=x[2+number]=X-(X-X0)/5;
	x[3+number]=x[4+number]=X-2*(X-X0)/5;
	
	settextstyle(0,0,2);
	srand(time(0));
	turn=0;
	time(&ti);
	while(1){
		if(check_collision()){
			collisions++;
			continue;
		}
		time(&tf);
		if(ti!=tf){
			ti=tf;
			tme++;
			collisions=0;
		}
		_print(-1);
		move_all();
		boundary();
		if(movement==0) control_striker();
		if(GetAsyncKeyState(VK_ESCAPE)) break;
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
