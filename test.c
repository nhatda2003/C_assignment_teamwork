/*
Simulate:
- Case 1: With traffic light
- Case 2: No traffic light


Tasks undone:
- File I/O to read and create map
- Linked list for vehicle
- Logic to wait for vehicle (or change direction)
- Counter to represent efficiency of 2 cases (with traffict light vs no traffic light)
- Case 2
- Traffic light for case 1
- Change: Big road 6 lanes, small road 3 lanes
- File I/O to save the results after each simulation
*/
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <unistd.h>

int N = 10;

typedef struct Vehicle{
    int x;
    int y;
    int destination[2];
    int speed;
    int change_lane;
} Vehicle;


void move(int* x0, int* y0, int x1, int y1){
    int d0 = pow((*x0-x1),2) + pow((*y0-y1),2);
    int newx = -1, newy = -1;
    for (int i = -1; i<=1; i++){
        for (int j = -1; j<=1; j++){
            if ((pow((*x0-i-x1),2) + pow((*y0-j-y1),2)) < d0 ){
                d0 = (int)(pow((*x0-i-x1),2) + pow((*y0-j-y1),2));
                newx = (int) abs(*x0-i);
                newy = (int) abs(*y0-j);
            }
    }
}
    if (newx!=-1 && newy!=-1){
        *x0 = newx;
        *y0 = newy;
    }
};


char point(char** map, int x, int y){
    return map[x-1][y-1];
};



void map_print(char** map){
    for (int i = 0; i<N;i++){
        for(int j = 0; j<N;j++){
            printf("%c ", map[j][i]);
        }
        printf("\n");
    }
};

/*Maybe use file I/O to set map*/



int main(){
    /*Create map NxN */
    char** map = (char**) malloc(N*(sizeof(int*)));
    if (map != NULL){
        for (int i = 0; i<N; i++){
            map[i] = (char*) malloc(N*sizeof(int));
        }
        for (int i = 0; i<N; i++){
            for(int j = 0; j<N; j++){
                map[i][j]=' ';
            }
        }
    }
    /*set map*/
    map[5-1][1-1] =map[0][0] =map[6-1][1-1] = map[10-1][5-1] =map[10-1][6-1] =map[5-1][10-1] = map[10-1][1-1]= map[1-1][5-1]= map[1-1][6-1] = map[1-1][10-1]= map[5-1][10-10] =map[6-1][10-1] = map[10-1][10-1] = 'X';


    Vehicle car1 = {8, 10, {1,3}, 1,0};
    Vehicle car2 = {1, 8, {10,8}, 1,0};
    Vehicle car3 = {8, 1, {3,10}, 1,0};
    Vehicle car4 = {10, 3, {1,2}, 1,0};
    Vehicle car5 = {8, 1, {8,10}, 1,0};
    //Vehicle car5...


    for (int i = 0; i<=20; i++){
        map[car1.x-1][car1.y-1] = '1';
        map[car2.x-1][car2.y-1] = '2';
        map[car3.x-1][car3.y-1] = '3';
        map[car4.x-1][car4.y-1] = '4';
        map[car5.x-1][car5.y-1] = '5';

        map_print(map);

        map[car1.x-1][car1.y-1] = ' ';
        map[car2.x-1][car2.y-1] = ' ';
        map[car3.x-1][car3.y-1] = ' ';
        map[car4.x-1][car4.y-1] = ' ';
        map[car5.x-1][car5.y-1] = ' ';

        sleep(1);
        system("cls");

        move(&car1.x, &car1.y, car1.destination[0], car1.destination[1]);
        move(&car2.x, &car2.y, car2.destination[0], car2.destination[1]);
        move(&car3.x, &car3.y, car3.destination[0], car3.destination[1]);
        move(&car4.x, &car4.y, car4.destination[0], car4.destination[1]);
        move(&car5.x, &car5.y, car5.destination[0], car5.destination[1]);
    }


    return 0;
}

