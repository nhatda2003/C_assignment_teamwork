#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <unistd.h>
#include <time.h>
#define ROWS 16
#define COLS 10
#define SIMULATION_TIME_INTERVAL 20

int counter;

typedef struct Vehicle{
    char name;
    int x;
    int y;
    int destination[2];
    int wait;
    int speed;
    char road;
    int arrive;
    int exit;
} Vehicle;


int randomInRange(int a, int b) {
   srand(time(NULL)); // initialize random seed

   // generate a random integer within the range [a, b]
   int randomNum = rand() % (b - a + 1) + a;

   return randomNum;
}


/*db link-----------------------------------------------------------------------*/
struct node {
    Vehicle * data;
    struct node* next;
};


struct node* head = NULL;
void insert(Vehicle *value) {
    struct node* new_node = (struct node*)malloc(sizeof(struct node));
    new_node->data = value;
    new_node->next = head;
    head = new_node;
}


void delete(int name) {
    struct node* current = head;
    struct node* prev = NULL;

    while (current != NULL && current->data->name != name) {
        prev = current;
        current = current->next;
    }

    if (current == NULL) {
        return; // value not found
    }

    if (prev == NULL) {
        head = current->next;
    } else {
        prev->next = current->next;
    }

    free(current);
}


/*db link-----------------------------------------------------------------------*/
void move(Vehicle* vehicle, char** map){


    int* x0 = &(vehicle->x);
    int* y0 = &(vehicle->y);

    if (vehicle->arrive==1){
            vehicle->exit=1;
            map[*x0-1][*y0-1] = ' ';
            return;
        }

    int x1 = vehicle->destination[0], y1 = vehicle->destination[1];

    int d0 = pow((*x0-x1),2) + pow((*y0-y1),2);
    int newx = -1;
    int newy = -1;
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
        if (map[newx-1][newy-1] == ' '){
        map[*x0-1][*y0-1] = ' ';
        *x0 = newx;
        *y0 = newy;
        map[*x0-1][*y0-1] = '*';
    }   if (newx==vehicle->destination[0] && newy==vehicle->destination[1] && vehicle->exit == 0){
        vehicle->arrive = 1;
        counter ++;
    }
}
}


void map_print(char** map){
    for (int i = 0; i<ROWS;i++){
        for(int j = 0; j<COLS;j++){
            printf("%c ", map[j][i]);
        }
        printf("\n");
    }
};


/*Maybe use file I/O to set map*/


int main(){
    printf("Running\n");
    char** map;
    char filename[] = "map16x10.txt";
    char array[COLS][ROWS];
    char temp;
    int time = 0;


    int number_of_lanes_on_big_road = 6; // in each direction
    int number_of_lanes_on_small_road = 3; // in each direction

    int spawn_set_on_big_road_1; // + (1, y)
    int spawn_set_on_big_road_2; // + (COLS, y)
    int spawn_set_on_small_road_1; // + (x, ROWS)
    int spawn_set_on_small_road_2; // + (x, 1)

    int destination_set_on_big_road_1; // - (1, y)
    int destination_set_on_big_road_2; // - (COLS, y)
    int destination_set_on_small_road_1; // - (x, ROWS)
    int destination_set_on_small_road_2; // - (x, 1)

    FILE *file = fopen(filename, "r");


    /*Declare map object*/
    map = (char**) malloc(COLS*(sizeof(char*)));
    if (map != NULL){
        for (int i = 0; i<COLS; i++){
            map[i] = (char*) malloc(ROWS*sizeof(char*));
        }
    }


    /*Read map from file I/O*/
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        exit(1);
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS+1; j++) {
             fscanf(file, "%c", &temp);
                if(i<ROWS && j<COLS){
                    array[j][i] = temp;
                }
    }
        }
    fclose(file);


    // Convert array to map
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {

            if (array[j][i] == '.'){

                map[j][i] = ' ';
            }
            else if(array[j][i] == 'X'){

                map[j][i] = 'X';
            }
            else if(array[j][i] == '+'){
                 map[j][i] = ' ';
                if (j==1-1){
                    spawn_set_on_big_road_1 = i;
                }
                if (j==COLS-1){
                    spawn_set_on_big_road_2 = i;
                }
                if (i==ROWS-1){
                    spawn_set_on_small_road_1 = j;
                }
                if (i==1-1){
                    spawn_set_on_small_road_2 = j;
                }
            }
            else if(array[j][i] == '-'){
                 map[j][i] = ' ';
                if (j==1-1){
                    destination_set_on_big_road_1 = i;
                }
                if (j==COLS-1){
                    destination_set_on_big_road_2 = i;
                }
                if (i==ROWS-1){
                    destination_set_on_small_road_1 = j;
                }
                if (i==1-1){
                    destination_set_on_small_road_2 = j;
                }

            }
    }
    }


    /*Thay bang linked list va random -> generate car*/
    Vehicle car1 = {1,8, 10, {1,3}, 1,'s', 0, 0};
    Vehicle car2 = {2,1, 8, {10,8}, 1,'b', 0, 0};
    Vehicle car3 = {3,8, 1, {3,10}, 1,'s', 0, 0};
    Vehicle car4 = {4,10, 3, {1,2}, 1,'b', 0, 0};
    Vehicle car5 = {5,7, 1, {8,10}, 1,'s', 0, 0};
    insert(&car1);
    insert(&car2);
    insert(&car3);
    insert(&car4);
    insert(&car5);


    sleep(1);
    system("cls");


    /*Drive code*/
    struct node* current;
    struct node* place_holder;
    while (time < SIMULATION_TIME_INTERVAL){
        /* 
        - Can chinh lai thu tu cua cai linked list 
        - Can co ham random() de spawn() xe
        - spawn coi nhu la mot cai move vao vi tri xuat phat
        - Can co 1 tap cac vi tri spawn duoc doc vao tu map
        
        - random_spawn_big_road -> den xanh o big road thi moi spawn:
            + nhan vao map de biet o vi tri can spawn dang co xe nao khong)
            + return ra pointer vehicle
            + su dung pointer vehicle do de insert()
        random_spawn_small_road -> den xanh o big road thi moi spawn (tuong tu ...big_road)
        */
        
        current = head;

        /*Refresh map*/
        map_print(map); 
        printf("\nTime: %d", time);
        printf("\nPassed: %d", counter);
        sleep(1);
        system("cls");

        while (current != NULL) {
            place_holder = current;
            current = (current->next); 
            if(place_holder->data->exit ==1){
                delete(place_holder->data->name);
            }else{
                move(place_holder->data, map);
            }

        } 
        time ++;
    }
    return 0;
}

