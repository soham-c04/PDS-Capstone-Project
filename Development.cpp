#include <bits/stdc++.h>
#include <graphics.h>
using namespace std;

const double pi=2*acos(0.0);

double rad(double deg){
	return deg*pi/180;
}

int X=GetSystemMetrics(SM_CXSCREEN),Y=GetSystemMetrics(SM_CYSCREEN);
int ground=3*Y/4;
int boundary_color=BLACK;
int radius_ball=50,radius_coin=20;
int x=200,y=ground-radius_ball-2;
int vy=0;
double vx=0;
int bk_speed=2;
int mountain_width=300;
const int number=6; //number=X/mountain_width+2;
const int cloud_number=3,coin_number=10;
int mountain_height[number];
int mountain[number]={0,303,603,903,1203,1503};

pair<int,int> coin[coin_number];
pair<int,int> cloudx[cloud_number]={{500,700},{X,X+200},{-1,-1}};
pair<int,int> cloudy[cloud_number]={{100,200},{150,250},{-1,-1}};

double angle=0.0;
int page=0;
int tme=0;
int score=0;
int it=0;

int rand_num(int a, int b){
	srand(time(0)+round(100*cos(it)));
    return a+rand()%(b-a+1);
}

void print(){
	setvisualpage(1-page);
	setactivepage(page);
	cleardevice();
	
	setlinestyle(0,0,1);
	rectangle(0,ground,X,Y);
	line(0,ground,X,ground);
	setfillstyle(SOLID_FILL,7);
	floodfill(X-10,Y-10,boundary_color);
	
	// Printing mountains;
	setcolor(2);
	for(int i=0;i<number;i++){
		line(mountain[i],ground,mountain[i]+mountain_width,ground);
		line(mountain[i],ground,mountain[i]+mountain_width/2,ground-mountain_height[i]);
		line(mountain[i]+mountain_width,ground,mountain[i]+mountain_width/2,ground-mountain_height[i]);
		setfillstyle(SOLID_FILL,2);
		floodfill(mountain[i]+5,ground-1,2);
		floodfill(mountain[i]+mountain_width-5,ground-1,2);
	}
	setcolor(boundary_color);
	circle(x,y,radius_ball);
	setfillstyle(SOLID_FILL,12);
	floodfill(x,y,boundary_color);
	line(x+radius_ball*cos(rad(angle)),y-radius_ball*sin(rad(angle)),x+radius_ball*cos(rad(180+angle)),y-radius_ball*sin(rad(180+angle))); // Rotating lines on ball
	line(x+radius_ball*cos(rad(90+angle)),y-radius_ball*sin(rad(90+angle)),x+radius_ball*cos(rad(270+angle)),y-radius_ball*sin(rad(270+angle))); // Rotating lines on ball
	
	setcolor(14);
	setlinestyle(0,0,5);
	
	//sun
	circle(200,100,50); 
	setfillstyle(SOLID_FILL,14);
	floodfill(200,100,14);
	//coins
	for(int i=0;i<coin_number;i++){
		circle(coin[i].first,coin[i].second,radius_coin);
		floodfill(coin[i].first,coin[i].second,14);
	}
	
	// Rotating ball
	arc(x,y,angle-2.5,angle+2.5,radius_ball);
	arc(x,y,angle+87.5,angle+92.5,radius_ball);
	arc(x,y,angle+177.5,angle+182.5,radius_ball);
	arc(x,y,angle+267.5,angle+272.5,radius_ball);
	
	//clouds
	setcolor(7);
	for(int i=0;i<cloud_number;i++) rectangle(cloudx[i].first,cloudy[i].first,cloudx[i].second,cloudy[i].second);
	setfillstyle(SOLID_FILL,WHITE);
	for(int i=0;i<cloud_number;i++){
		floodfill(cloudx[i].first+3,(cloudy[i].first+cloudy[i].second)/2,7);
		floodfill(cloudx[i].second-3,(cloudy[i].first+cloudy[i].second)/2,7);
	}
	
	setcolor(boundary_color);
	char c[4];
	sprintf(c,"%d",tme);
	outtextxy(X/2,50,c);
	
	char sc[7]="Score:";
	outtextxy(X/2+200,50,sc);
	
	char sco[4];
	sprintf(sco,"%d",score);
	outtextxy(X/2+320,50,sco);
	angle-=(vx>=0)? 5.0:-5.0;
	page=1-page;
}
void move_bk(){
	for(int i=0;i<cloud_number;i++){ // Move clouds
		cloudx[i].first-=bk_speed;
		cloudx[i].second-=bk_speed;
	}
	for(int i=0;i<cloud_number;i++){ // Restore lost clouds
		if(cloudx[i].second<=0){
			it+=5;
			cloudx[i].second=rand_num(cloudx[i].first+50,cloudx[i].first+250);
			cloudy[i].first=rand_num(10,100);
			it+=10;
			cloudy[i].second=rand_num(cloudy[i].first+50,cloudy[i].first+250);
		}
	}
	
	for(int i=0;i<number;i++) mountain[i]-=bk_speed; // Move mountains
	for(int i=0;i<number;i++){ // Restore lost mountains
		if(mountain[i]<=-mountain_width-3){
			mountain[i]=*max_element(mountain,mountain+number)+mountain_width+3;
			mountain_height[i]=rand_num(180,350);
		}
	}
	
	for(int i=0;i<coin_number;i++) coin[i].first-=bk_speed; // Move coins;
	for(int i=0;i<coin_number;i++){
		if(coin[i].first<=-radius_coin){ // restore coins
			do{
				coin[i].first=X+100*rand_num(0,X/100);
				it+=5;
			}
			while(count(coin,coin+coin_number,coin[i])>1);
			coin[i].first=X+100*rand_num(0,X/100);
			it+=5;
			coin[i].second=250+100*rand_num(0,2);
			it+=10;
		}
	}
	
}
void eatcoin(){
	for(int i=0;i<coin_number;i++){
		if(pow(coin[i].first-x,2)+pow(coin[i].second-y,2)<=(radius_coin+radius_ball)*(radius_coin+radius_ball)){
			Beep(700,100);
			score++;
			if(GetAsyncKeyState(VK_SPACE)) cout<<coin[i].first<<" "<<radius_coin<<"\n";
			do{
				coin[i].first=X+100*rand_num(0,X/100);
				it+=5;
			}
			while(count(coin,coin+coin_number,coin[i])>1);
			coin[i].second=250+100*rand_num(0,0);
			it+=10;
		}
	}
}
void moveball(){
	y-=vy;
	x+=vx;
	if(y!=ground-radius_ball-2) vy-=1;
	x=max(radius_ball,x);
	x=min(X-radius_ball,x);
	y=min(y,ground-radius_ball-2);
	y=max(radius_ball,y);
	if(vy<0 and y==ground*1.0-radius_ball-2) vy/=-2;
}

int main(){
	srand(time(0)+it);
	initwindow(X,Y,"",-3,-3);
	setbkcolor(11);
	for(int i=0;i<number;i++) mountain_height[i]=250;
	for(int i=0;i<coin_number;i++){
		do{
			coin[i].first=X+100*rand_num(0,X/100);
			it+=5;
		}
		while(count(coin,coin+coin_number,coin[i])>1);
		coin[i].second=250+100*rand_num(0,2);
		it+=10;
	}
	time_t ti,tf;
	time(&ti);
	settextstyle(4,0,3);
	
	while(!GetAsyncKeyState(VK_ESCAPE)){
		time(&tf);
		if(ti!=tf){
			tme++;
			ti=tf;
		}
		move_bk();
		if(GetAsyncKeyState(VK_UP));
		if(vy==0 and y==ground-radius_ball-2 and GetAsyncKeyState(VK_UP)) vy=20;
		if(GetAsyncKeyState(VK_RIGHT)) vx=5;
		else if(GetAsyncKeyState(VK_LEFT)) vx=-5;
		else if(vy==0 and y==ground-radius_ball-2) vx=0;
		if(vy || vx || y!=ground-radius_ball-2) moveball();
		eatcoin();
		print();
		it++;
		delay(10);
	}
	closegraph();
	return 0;
}
