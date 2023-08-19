#include <graphics.h>
#include <math.h>
#include <time.h>
#include <unistd.h>

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
#define Xratio X_full/1280  // For Adjusting to size of objects in X direction according to resolution of Screen
#define Yratio Y_full/720   // For Adjusting to size of objects in Y direction according to resolution of Screen
#define ratio sqrt(Xratio*Yratio) // For Adjusting to size of objects in circular direction according to resolution of Screen
/*
Since this game was build in my PC. Hence I adjusted the X and Y resolution of Screen According to my PC so that the
relative sizes of all objects in any resolution are the same.
*/
int X; // Gets the X resolution of Screen
int Y; // Gets the Y resolution of Screen;
int X_full,Y_full; // Stores the X and Y orignial resolution of screen used in Xratio and Yratio
int X0,Y0; // Defines the starting corrdinates of playable field
int speed,speed_ball; // speed means speed of striker and spped_ball is the speed of ball.
int radius,radius_ball; // Radius of radius of striker and radius_ball is the radius of ball
double angle; // Angle is the angle at which the ball is currently travelling in.
// Angle is measured in anticlockwise diredction from the +x x-axis. Assuming the bottom left of our laptop to be origin.
// This is not the same as graph origin which is at the upper_left corner of laptop.
int goal_height,goal_width; // Defines the size of goal
int x1,y1; // Coordinates of left striker
int x,y; // Coordinates of Ball
int x2,y2; // Coordinates of right striker
int goal1=0,goal2=0,goal=0,tme=0,page=0;
/*
goal1 and goal 2 and goals scored by left and right respectively.
goal is variable which is set to 1 when a goal occurs. This is done so as to prevent boundary collisions in x
when goal occurs. As a result ball can travel inside the goal
tme keeps track of  current time after the start of game
page refers to the visual and active pages used in double buffering
*/
void move_ball(); // used for movement of ball \. predefined as it is called in gola() before definition of move_all(); 
void _print(){ // Used for printing the whole field and screen
	setactivepage(page); // double buffering
	setvisualpage(1-page); // It is used so as to remove flickerings when printing a new field or a new striker. 
	// Observed during floodfilling if not done.
	cleardevice();
	// Creating the ball
	setcolor(YELLOW);
	circle(x,y,radius_ball);
	setfillstyle(SOLID_FILL,WHITE);
	floodfill(x,y,YELLOW);
	setcolor(WHITE);
	setlinestyle(0,0,1); // Set the line width to a 1 pixel line
	line((X+X0)/2,Y0,(X+X0)/2,Y); // Draw central boundary line of strikers
	setlinestyle(0,0,3); // Set the line width to a 3 pixel line
	line(X0,Y0,X0,(Y+Y0)/2-goal_height); // Creating left X boundary above left goal
	line(X0,(Y+Y0)/2+goal_height,X0,Y); // Creating left X boundary below left goal
	line(X,Y0,X,(Y+Y0)/2-goal_height); // Creating right X boundary above right goal
	line(X,(Y0+Y)/2+goal_height,X,Y); // Creating right X boundary below right goal
	line(X0,Y0,X,Y0); // Upper Y-boundary
	line(X0,Y,X,Y); // Lower Y-boundary
	line(X0-goal_width,(Y+Y0)/2-goal_height,X0,(Y+Y0)/2-goal_height); // Upper left goal boundary
	line(X0-goal_width,(Y+Y0)/2+goal_height,X0,(Y+Y0)/2+goal_height); // Lower left goal boundary
	line(X0-goal_width,(Y+Y0)/2-goal_height,X0-goal_width,(Y+Y0)/2+goal_height); // Left boundary of left goal
	line(X+goal_width,(Y+Y0)/2-goal_height,X,(Y+Y0)/2-goal_height); // Upper boundary of right goal
	line(X+goal_width,(Y+Y0)/2+goal_height,X,(Y+Y0)/2+goal_height); // Lower boundary of right goal
	line(X+goal_width,(Y+Y0)/2-goal_height,X+goal_width,(Y+Y0)/2+goal_height); // Right boundary of right goal
	setlinestyle(2,0,1); // Creating 1 pixel dotted line
	line(X0,(Y+Y0)/2-goal_height,X0,(Y+Y0)/2+goal_height); // Left goal entrance
	line(X,(Y+Y0)/2-goal_height,X,(Y+Y0)/2+goal_height); //Right goal entrance
	setlinestyle(0,0,3);
	// Left striker
	setcolor(12);
	circle(x1,y1,radius);
	setfillstyle(SOLID_FILL,10);
	floodfill(x1,y1,12);
	// Right striker
	setcolor(13);
	circle(x2,y2,radius);
	setfillstyle(SOLID_FILL,11);
	floodfill(x2,y2,13);
	// Number of Goals:
	char c[]="Goals 1: ";
    outtextxy(10*Xratio,10*Yratio,c);
    char c1[4];
    sprintf(c1, "%d", goal1); // sprintf convert integer to char[] as outtext only accepts char array
    outtextxy(125*Xratio,10*Yratio,c1);
    char ttime[4];
    char tt[]="Time:";
    outtextxy(X/2-40*Xratio,10*Yratio,tt);
    char ttm[4];
    sprintf(ttm,"%d",tme);
    outtextxy(X/2+40*Xratio,10*Yratio,ttm);
    int xt=tme,sz=0;
    // sz is the number of digits of time spent in seconds (tme) in decimal
    while(xt>0){
		xt/=10;
		sz++;
	}
	sz=max(1,sz);
	// sz is required so as to push to s away from time spent otherwise with number of digits of time increasing time will eventually
	// overlap with s.
	// sz=max(1,sz) is done so that in the case when tme=0 it doesn't overlap with s bcoz sz will be 0 when tme=0
    char ss[]="s";
    outtextxy(X/2+40*Xratio+sz*15*Xratio,10*Yratio,ss);
    char s2[]="s";
    int xc=goal2,sze=0;
    while(xc>0){
    	xc/=10;
    	sze++;
	}
	// Same pushing of number is done here with size of goals scored by 2. This time it is done from the right side and both
	// goals scored and "Goals 2: " is pushing by that factor(15) on each size increase of goal 2
	char ce[]="Goals 2: ";
	sze=max(1,sze);
    outtextxy(100*Xratio+X-130*Xratio-15*sze*Xratio,10*Yratio,ce);
	char c2[4];
    sprintf(c2, "%d", goal2);
    outtextxy(100*Xratio+X-15*Xratio-15*sze*Xratio,10*Yratio,c2);
    page=1-page; // double buffering page change
}
void gola(int who){ // This is initiated whenever a goal happens. Here who defines whoever accepted the goal
	goal=1; // Goal is set to 1 as defined above because the process of occurence of goal has started
	int ss=speed_ball; // initial speed of ball is stored because in the process of goal scoring teh value of speed_ball is changed
	// Thus to restore the initial speed when match start again
	if(who==1){
		goal2++; // If player 1 accepts goal means number of goals of 2 increases by 1.
		while(x>X0-3*goal_width/4 && (y>=(Y+Y0)/2-goal_height+radius_ball) && (y<=(Y+Y0)/2+goal_height-radius_ball)){
			/*
			THe 1st condition of while loop condition defines the amount of distnace ball with travel before stopping inside the goal
			The 2nd and 3rd define that the ball should not cross the upper and lower boundaries of goal while travelling to the 
			required distance inside the goal i.e. 3/4 * width_goal
			*/
			_print(); // print the new position of ball inside goal
			speed_ball=3; // This is to slow down ball movement inside goal
			move_ball(); // To keep moving the ball in the same direction with slower speed_ball when goal happens
			usleep(10*ms);
		}
	}
	else{ // Same process as above but this time when player 2 accepts goal
		goal1++;
		int ss=speed_ball;
		while(x<X+3*goal_width/4 && (y>=(Y+Y0)/2-goal_height+radius_ball) && (y<=(Y+Y0)/2+goal_height-radius_ball)){
			_print();
			speed_ball=3;
			move_ball();
			usleep(10*ms);
		}
	}
	speed_ball=ss; // to restore the initial speed_ball
	// Set the position of ball and strikers again to its original position
	x1=X0+radius+10*Xratio;
	y1=(Y+Y0)/2;
	x=(X+X0)/2;
	y=(Y+Y0)/2;
	x2=X-radius-10*Xratio;
	y2=(Y+Y0)/2;
	angle=90.0;
	goal=0; // End the process of goal i.e movement of ball inside goal
	setvisualpage(1-page); // since in the above while loop in double buffering the visual and active pages were being constantly 
	// swapped. To set visual page back to the newly printed one this is done.
	// GOAL !! is printed for 2 seconds in the middle of the ground for 2 seconds after all process of ball movement inside goal has 
	//occured 
	setcolor(13);
	settextstyle(0,0,5);
	char c[]="GOAL !!";
	outtextxy((X0+X)/2-120*Xratio,(Y0+Y)/2-50*Yratio,c);
	settextstyle(0,0,2);
	sleep(2); // play the goal text for 2 seconds
}
void boundary(){
	if(x<=(radius_ball+X0) && (y>=(Y+Y0)/2-goal_height+radius_ball) && (y<=(Y+Y0)/2+goal_height-radius_ball)){ 
		gola(1); // This if is to detect the left goal when ball goes inside it. Then gola function is initiated with parameter
		return; // who accepted the goal
	}
	else if((x+radius_ball)>=X && (y>=(Y+Y0)/2-goal_height+radius_ball) && (y<=(Y+Y0)/2+goal_height-radius_ball)){
		gola(2); // TO detect right goal when ball goes inside
		return;
	}
	if(x<=(radius_ball+X0) || x+radius_ball>=X ) angle=fmod(fmod(180.0-angle,360.0)+360.0,360.0); // Change the direction when collision with left and right boundaries
	if(y<=(radius_ball+Y0) || y+radius_ball>=Y) angle=fmod(fmod(360.0-angle,360.0)+360.0,360.0); // Change the direction when collision with upper and lower boundaries
}
int check_collision(){ // Check if a collision has occured when striker is moved towards ball.
	int ans=0; // ans non-zero returns if collision has occured. And if collison has occured then who collided with whom is determined
	if((x1-x)*(x1-x)+(y1-y)*(y1-y)<(radius+radius_ball)*(radius+radius_ball)) ans++; // Left striker and ball
	if((x2-x)*(x2-x)+(y2-y)*(y2-y)<(radius+radius_ball)*(radius+radius_ball)) ans+=2; // Right striker and ball
	if((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)<=4*radius*radius) ans+=3; // Left and right striker
	return ans;
}
void collision(){ // Detect collision of ball with striker
	if((x1-x)*(x1-x)+(y1-y)*(y1-y)<=(radius+radius_ball)*(radius+radius_ball)){ // Left striker wtih ball
		double theta; // Theta is the angle of line joining centres of striker and ball with +X axis measured in anticlockwise direction
		// Also the direction of theta points from the centre of striker to ball
		if(y==y1) theta=(x1>x)*(180); // When both on same horizontal line theta is 0 or 180
		else if(x==x1) theta=90+(y>y1)*(180); // When both on same vertical line theta is 90 or 270
		else theta=deg(atan((y1-y)*1.0/(x-x1))); // Calculating slope of line and converting that to degree from tan inverse
		double anglee=angle; // Inital angle of direction of ball movement before collision
		angle=fmod(fmod(180.0+2*theta-angle,360.0)+360.0,360.0); // Using reflection physics new angle=180-2*theta-initial_angle
		// fmod is used for modular division in doubles. Bcoz % can't be usd for modular division in double
		int incx=round(speed_ball*cos(rad(anglee))),incy=round(speed_ball*sin(rad(anglee))); 
		// return the ball to the position just before the collision.
		if(abs(anglee-angle)>10.0){ // Excluding tangential collisions
			x-=incx;// Position of ball before collision and then reflect from that point
			y+=incy;// This has to be done bcoz is some cases the striker tries to move towards ball but the ball is already inside
			// striekr as per the collision condition as a result the ball does some wierd collision with striker and finally
			// the striker attaches to the ball 
		}
		else move_ball(); 
		/*
		Trying to debug tangential collisions with striker moving towards them
		in the previous case the was moved to its previous location in such collisions in the tangential collision case the ball
		is moved forward because if it is moved backward then if striker is moving towards ball, teh ball again gets attached to 
		striker. Thus repeating this process of collision for infintie time until striker is moved backward. Hence, as a attempt
		to solve this case the ball is force moved forward instead of backward in tangential collisions. TO escape vicinity of striker.
		*/
	}
	else if((x2-x)*(x2-x)+(y2-y)*(y2-y)<=(radius+radius_ball)*(radius+radius_ball)){ // Collision of Right striker with ball
		double theta;
		if(y==y2) theta=(x2>x)*(180.0);
		else if(x==x2) theta=90.0+(y>y2)*(180.0);
		else theta=deg(atan((y2-y)*1.0/(x-x2)));
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
	int incx=round(speed_ball*cos(rad(angle))),incy=round(speed_ball*sin(rad(angle))); // Move the ball by taking its vector 
	// components in x and y direction
	x+=incx;
	y-=incy;
	if(goal==0){
		if(y<((Y+Y0)/2-goal_height+radius) || y>((Y+Y0)/2+goal_height-radius)) x=max(x,X0+radius_ball); // Ball touches
		// left boundary above and below goal.
		if(y<((Y+Y0)/2-goal_height+radius) || y>((Y+Y0)/2+goal_height-radius)) x=min(x,X-radius_ball); // Ball touches
		// right boundary above and below goal
		y=max(y,Y0+radius_ball); // Lower Y limits on ball
		y=min(y,Y-radius_ball); // Lower X limits on ball
	}
}
void control_striker(){
	int incx=0,incy=0;
	// Moving the striker
	if(GetAsyncKeyState('W')) incy=speed;
	if(GetAsyncKeyState('A')) incx=-speed;
	if(GetAsyncKeyState('S')) incy=-speed;
	if(GetAsyncKeyState('D')) incx=speed;
	x1+=incx;
	y1-=incy;
	// Collisions with boundary or limits of striker movement
	y1=max(y1,Y0+radius);
	y1=min(y1,Y-radius);
	x1=max(x1,X0+radius);
	x1=min(x1,(X+X0)/2-radius);
	if(check_collision()>=1){
		x1-=incx; // This checks if the striker is colliding with the ball or another striker. If yes then then striker is moved 
		y1+=incy; // back to its original position to prevent any overlapping with ball.
	}
	incx=incy=0;
	// Same for right ball
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
}
int main(){
	X=GetSystemMetrics(SM_CXSCREEN);
	Y=GetSystemMetrics(SM_CYSCREEN);
	X_full=X;Y_full=Y;
	initwindow( X , Y , "",-3,-3);
	X0=100*Xratio;Y0=50*Yratio;
	speed=7*ratio;speed_ball=10*ratio;
	radius=50*ratio;radius_ball=20*ratio;
	angle=0.0;
	goal_height=100*Yratio;goal_width=90*Xratio;
	x1=X0+radius+10*Xratio;y1=Y/2;
	x=X/2;y=Y/2;
	x2=X-goal_height-radius-10*Xratio;y2=Y/2;
	setbkcolor(2);
	X-=100*Xratio;
	Y-=50*Yratio; 
	// Decreasing the size of playable field from the full screen window to a fixed size. Thus it begins with (X0,Y0) and ends at (X,Y)
	settextstyle(0,0,2);
	int flag=0;
	time_t t0,t1;
	time( &t0 ); // gets the current time of system
	while(1){
		time(&t1);
		if(t0!=t1){ // If the current time of system is not equal to previously stored value of time then tme increases by 1s
			t0=t1;  // Then store this current changed value which will tehn further be used to check difference in the next 1s
			tme++;
		}
		_print();
		move_ball();
		boundary();
		collision();
		control_striker();
		if(GetAsyncKeyState(VK_ESCAPE)) break; // Break out of game condition
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
