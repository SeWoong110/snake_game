#include "Snake.h"

using namespace std;

extern int map[5][40][60];
extern string whyDead;
extern void appearPosion(int stage_num, WINDOW *win1);
extern void appearGrowth(int stage_num, WINDOW *win1);
extern void disappearPoison(int stage_num, WINDOW *win1);
extern void disappearGrowth(int stage_num, WINDOW *win1);
extern WINDOW *win1;


Snake::Snake(int r, int c) : row(r), col(c)
	{
		snakeLen=4;
		for(int i=0; i<snakeLen; i++)
			snake_vec.push_back(Vector(col/2, row/2+i));
		setDirection(0);
		end = false;
		speed = 100000;
		map_list = new char[row*col];
		level=1;
		setGateCnt(0);
	}

Snake::~Snake(){ delete [] map_list; }


void Snake::setDirection(int d){
	switch(d)
	{
		case 0 : direction = Vector(0, -1); break;
		case 1 : direction = Vector(1, 0); break;
		case 2 : direction = Vector(0, 1); break;
		case 3 : direction = Vector(-1, 0); break;
	}
}

int Snake::gateDirection(Vector gate, int map[40][60]){
	Vector A = gate + Vector(0,-1) ;
	if(map[A.getY()][A.getX()]==0) return 0;
	Vector B = gate + Vector(1,0) ;
	if(map[B.getY()][B.getX()]==0) return 1;
	Vector C = gate + Vector(0,1) ;
	if(map[C.getY()][C.getX()]==0) return 2;
	Vector D = gate + Vector(-1,0) ;
	if(map[D.getY()][D.getX()]==0) return 3;

	return -1;
}


void Snake::moveSnakeHead(int map[40][60]){
	snake_vec[0] += direction; 
	for(int i=0; i<wall.size(); i++) { 
		if(snake_vec[0] == wall[i]) { 
			if(snake_vec[0] == gate[1]) { 
				snake_vec[0].setX(gate[0].getX()); 
				snake_vec[0].setY(gate[0].getY());
				setDirection(gateDirection(gate[0], map));
				setGateCnt(1);
				break;
			}
			else if(snake_vec[0] == gate[0]) {
				snake_vec[0].setX(gate[1].getX());
				snake_vec[0].setY(gate[1].getY());
				setDirection(gateDirection(gate[1], map));
				setGateCnt(1);
				break;
			}
			else {
				setEnd(true);
				removeGate(map);
				whyDead = "You hit the wall!"; 
			}
		}
	}
}


void Snake::moveSnakeBody(){ 
	for(unsigned int i=snake_vec.size()-1; i>0; --i) snake_vec[i] = snake_vec[i - 1];
}


char* Snake::setMaptoList(int map[40][60]){ 
	memset(map_list, ' ', row*col);
	for(unsigned int i=0; i<40; i++) {
		for(int j=0; j<60; j++){
			switch(map[i][j]) { 
				case 0 : map_list[i*col+j] = '0'; break;
				case 1 : 
					map_list[i*col+j] = '1';
					wall.push_back(Vector(j,i)); 
					break;
				case 2 : map_list[i*col+j] = '2'; break;
				case 3 : map_list[i*col+j] = '3'; break;
				case 4 : map_list[i*col+j] = '4'; break;
				case 5 : map_list[i*col+j] = '5'; break;
				case 6 : map_list[i*col+j] = '6'; break;
				case 98 : map_list[i*col+j] = '8'; break;
				case 99 : map_list[i*col+j] = '9'; break;
			}
		}

	}
	map_list[snake_vec[0].getY()*col+snake_vec[0].getX()] = 'h'; 
	for(unsigned int i=1; i<snake_vec.size(); ++i)
		map_list[snake_vec[i].getY()*col+snake_vec[i].getX()] = 'b'; 
	return map_list;
}


char Snake::getDirection() { 
	if(direction.getX()==1) return 'r'; 
	else if(direction.getX()==-1) return 'l'; 
	else if(direction.getY()==-1) return 'u'; 
	else return 'd';
}


void Snake::setEnd(bool e) {end = e;}
void Snake::setSpeed(float f) { if(f==1.0) speed = 100000; else speed *= f; }
bool Snake::getEnd() {return end;}
int Snake::getSpeed() {return speed;}
int Snake::getRow() {return row;}
int Snake::getCol() {return col;}

int Snake::getLevel() {return level;}
int Snake::getSnakeLen(){return snakeLen;}


void Snake::setGate(int map[40][60]) { 
	int randWall = rand() % wall.size();
	int randWall2 = rand() % wall.size();
	if(randWall == randWall2) setGate(map); 
	gate[0] = wall[randWall]; 
	gate[1] = wall[randWall2];
	map[gate[0].getY()][gate[0].getX()] = 98; 
	map[gate[1].getY()][gate[1].getX()] = 99;
	if(map[20-gateCnt][30+gateCnt]==0) map[20-gateCnt][30+gateCnt] = 1;
	if(map[20-gateCnt][30-gateCnt]==0) map[20-gateCnt][30-gateCnt] = 1;
	if(map[20+gateCnt][30+gateCnt]==0) map[20+gateCnt][30+gateCnt] = 1;
	if(map[20+gateCnt][30-gateCnt]==0) map[20+gateCnt][30-gateCnt] = 1;
}


void Snake::removeGate(int map[40][60])
{
	map[gate[0].getY()][gate[0].getX()] = 1; 
	map[gate[1].getY()][gate[1].getX()] = 1;
	gate[0].setX(0);
	gate[0].setY(0);
	gate[1].setX(0);
	gate[1].setY(0);
}



void Snake::setGateCnt(int i) {
	if (i==0){
		gateCnt = 0;
	}
	else{
		gateCnt += 1;
	}
}
int Snake::getGateCnt() {return gateCnt;}


int Snake::getSize(){return snake_vec.size();}

void Snake::setLevel(int new_level){
	level = new_level;
}


void Snake::minusSnake(WINDOW *win1){
	disappearPoison(level-1,win1);
	snake_vec.pop_back();
	appearPosion(level-1,win1);
}


void Snake::crushItem(WINDOW *win1){
	disappearGrowth(level -1,win1);
	snake_vec.push_back(snake_vec.back());
	appearGrowth(level-1,win1);
}


position Snake::getHead(){
	position head(snake_vec[0].getX(),snake_vec[0].getY());
	return head;
}


void Snake::resize(int new_size){
	snake_vec.resize(new_size);
	snakeLen = new_size;
}

void Snake::changeSnakeLen(){snakeLen = snake_vec.size();}
