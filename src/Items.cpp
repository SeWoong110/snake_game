#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctime>

#include <string>
#include <vector>

#include "Position.h"

using namespace std;

extern int map[5][40][60];
extern WINDOW *win1;

vector<position> vGrow_item ;
vector<position> vPoison_item;
vector<position> vSpeed_item;
position growth_position(0,0);
position poison_position(0,0);
position speed_position(0,0);


void updateGrowth(int stage_num){
    growth_position = vGrow_item.back();
    map[stage_num][growth_position.y][growth_position.x] = 3;
}
void updatePosion(int stage_num){
    poison_position = vPoison_item.back();
    map[stage_num][poison_position.y][poison_position.x] = 6;
}
void updateSpeed(int stage_num){
    speed_position = vSpeed_item.back();
    map[stage_num][speed_position.y][speed_position.x] = 5;
}


void appearPosion(int stage_num, WINDOW *win1){ 
    nodelay(win1, true);
    while(1){
        if(map[stage_num][poison_position.randomPosition().y][poison_position.randomPosition().x] != 0){
            poison_position.randomPosition();
        }
        else break;
    }
    vPoison_item.push_back(poison_position);
    updatePosion(stage_num);
}

void appearGrowth(int stage_num, WINDOW *win1){
    nodelay(win1, true);    
    while(1){
        if(map[stage_num][growth_position.randomPosition().y][growth_position.randomPosition().x] != 0){
            growth_position.randomPosition();
        }
        else break;
    }
    vGrow_item.push_back(growth_position);
    updateGrowth(stage_num);
}

void appearSpeed(int stage_num, WINDOW *win1){
    nodelay(win1, true);    
    while(1){
        if(map[stage_num][speed_position.randomPosition().y][speed_position.randomPosition().x] != 0){
            speed_position.randomPosition();
        }
        else break;
    }
    vSpeed_item.push_back(speed_position);
    updateSpeed(stage_num);
}


void disappearPoison(int stage_num, WINDOW *win1){
    nodelay(win1, true);
    if(vPoison_item.empty() == 0 ){
        poison_position = vPoison_item.back();
        map[stage_num][poison_position.y][poison_position.x] = 0;
        vPoison_item.pop_back();
    }
}

void disappearGrowth(int stage_num, WINDOW *win1){
    nodelay(win1, true);
    if(vGrow_item.empty() == 0 ){
        growth_position = vGrow_item.back();
        map[stage_num][growth_position.y][growth_position.x] = 0;
        vGrow_item.pop_back();
    }
}

void disappearSpeed(int stage_num, WINDOW *win1){
    nodelay(win1, true);
    if(vSpeed_item.empty() == 0 ){
        speed_position = vSpeed_item.back();
        map[stage_num][speed_position.y][speed_position.x] = 0;
        vSpeed_item.pop_back();
    }
}
