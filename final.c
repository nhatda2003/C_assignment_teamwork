#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include <unistd.h>
#include <time.h>
#define SIMULATION_TIME_INTERVAL 100

int counter;
int ROWS=0, COLS=0;
int number_of_vehicles = 0;
int number_of_lanes_on_big_road_1 = 0; // in each direction
int number_of_lanes_on_big_road_2 = 0; // in each direction
int number_of_lanes_on_small_road_1 = 0; // in each direction
int number_of_lanes_on_small_road_2 = 0; // in each direction

int spawn_set_on_big_road_1; // + (1, y)
int spawn_set_on_big_road_2; // + (COLS, y)
int spawn_set_on_small_road_1; // + (x, ROWS)
int spawn_set_on_small_road_2; // + (x, 1)

int destination_set_on_big_road_1; // - (1, y)
int destination_set_on_big_road_2; // - (COLS, y)
int destination_set_on_small_road_1; // - (x, ROWS)
int destination_set_on_small_road_2; // - (x, 1)

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


int randomInRange(int a, int b){
    srand(time(NULL));
    // Generate a random number between 1 and 100
    int num = rand() % b + a;
    // Print the random number
    return num;
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

    int other_way[4];
    int count_xy = 0; 
    int dmin1;
    int dmin1x=-1;
    int dmin1y=-1;
    int dmin2;
    int dmin2x=-1;
    int dmin2y=-1;

    int* x0 = &(vehicle->x);
    int* y0 = &(vehicle->y);
    int*waiting = &(vehicle->wait);

    if (vehicle->arrive==1){
            vehicle->exit=1;
            map[*x0-1][*y0-1] = ' ';
            return;
        }

    int x1 = vehicle->destination[0], y1 = vehicle->destination[1];

    int d0 = pow((*x0-x1),2) + pow((*y0-y1),2);
    dmin1 = d0;
    dmin2 = d0;
    int d;
    int newx = -1;
    int newy = -1;
    for (int i = -1; i<=1; i++){
        for (int j = -1; j<=1; j++){
            if ((pow((*x0-i-x1),2) + pow((*y0-j-y1),2)) < d0 ){
                d = (int)(pow((*x0-i-x1),2) + pow((*y0-j-y1),2));
                if (d < dmin1){
                    dmin1 = d;
                    dmin1x = (int) abs(*x0-i);
                    dmin1y = (int) abs(*y0-j);
                } else if(d<dmin2){
                    dmin2 =d;
                    dmin2x = (int) abs(*x0-i);
                    dmin2y = (int) abs(*y0-j);
                }
                
            }
    }
}
    if (dmin1x!=-1 && dmin1y!=-1){
        if (map[dmin1x-1][dmin1y-1] == ' '){
        map[*x0-1][*y0-1] = ' ';
        *x0 = dmin1x;
        *y0 = dmin1y;
        map[*x0-1][*y0-1] = '*';
    } else if (map[dmin1x-1][dmin1y-1] == 'X'){
        if(y1<dmin1y){
            map[*x0-1][*y0-1] = ' ';
            *y0 -= 1;
            map[*x0-1][*y0-1] = '*';
            
        } else if(y1>dmin1y){
            map[*x0-1][*y0-1] = ' ';
            *y0 +=1;
            map[*x0-1][*y0-1] = '*';
        } else if (x1<dmin1x){
            map[*x0-1][*y0-1] = ' ';
            *x0 -=1;
            map[*x0-1][*y0-1] = '*';
        }else if(x1>dmin1x){
            map[*x0-1][*y0-1] = ' ';
            *x0 += 1;
             map[*x0-1][*y0-1] = '*';
        }
   
    } else if(map[dmin1x-1][dmin1y-1] == '*'){
        if(*waiting<=10){
            *waiting +=1;
        } else{
            *waiting = 0;
            map[*x0-1][*y0-1] = ' ';
            *x0 = dmin1x;
            *y0 = dmin1y;
            map[*x0-1][*y0-1] = '*';
        
        }
    }
    
}
    if (*x0==vehicle->destination[0] && *y0==vehicle->destination[1] && vehicle->exit == 0){
            vehicle->arrive = 1;
            counter ++;
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




Vehicle* random_spawn_big_road_1(char **map, int start_point, int num_lanes){
    
    int spawn_x = 1;
    int spawn_y = start_point + randomInRange(0, num_lanes-1) + 1;


    int destination = randomInRange(1, 3);
    int destination_x;
    int destination_y;
    if (destination==1){
        destination_x = COLS;
        destination_y = 1 + destination_set_on_big_road_2 + randomInRange(0, number_of_lanes_on_big_road_2-1);
    } else if(destination==2){
        destination_x = 1 + destination_set_on_small_road_1 + randomInRange(0, number_of_lanes_on_small_road_1-1);
        destination_y = ROWS;
    } else if (destination==3){
        destination_x = 1 + destination_set_on_small_road_2 + randomInRange(0, number_of_lanes_on_small_road_2-1);
        destination_y = 1; 
    }
    //printf("\n???? %d %d %d\n", destination_x, destination_y, destination);


    if (map[spawn_x-1][spawn_y-1] == ' '){
        map[spawn_x-1][spawn_y-1] = '*';

        Vehicle * new_vehicle = malloc(sizeof(Vehicle));
        
        new_vehicle -> name = ++number_of_vehicles;
        new_vehicle -> x = spawn_x;
        new_vehicle -> y = spawn_y;
        new_vehicle -> road = 'b';
        new_vehicle -> arrive = 0 ;
        new_vehicle -> exit = 0;
        new_vehicle -> wait = 0;
        new_vehicle -> speed = 0;
        new_vehicle -> destination[0] = destination_x;
        new_vehicle -> destination[1] = destination_y;
        return new_vehicle;

    } else{
        return NULL;
    }
   
}

Vehicle* random_spawn_big_road_2(char **map, int start_point, int num_lanes){
    
    int spawn_x = COLS;
    int spawn_y = start_point + randomInRange(0, num_lanes-1) + 1;


    int destination = randomInRange(1, 3);
    int destination_x;
    int destination_y;
    if (destination==1){
        destination_x = 1;
        destination_y = 1 + destination_set_on_big_road_1 + randomInRange(0, number_of_lanes_on_big_road_1-1);
    } else if(destination==2){
        destination_x = 1 + destination_set_on_small_road_1 + randomInRange(0, number_of_lanes_on_small_road_1-1);
        destination_y = ROWS;
    } else if (destination==3){
        destination_x = 1 + destination_set_on_small_road_2 + randomInRange(0, number_of_lanes_on_small_road_2-1);
        destination_y = 1; 
    }
    


    if (map[spawn_x-1][spawn_y-1] == ' '){
        map[spawn_x-1][spawn_y-1] = '*';

        Vehicle * new_vehicle = malloc(sizeof(Vehicle));
        
        new_vehicle -> name = ++number_of_vehicles;
        new_vehicle -> x = spawn_x;
        new_vehicle -> y = spawn_y;
        new_vehicle -> road = 'b';
        new_vehicle -> arrive = 0 ;
        new_vehicle -> exit = 0;
        new_vehicle -> wait = 0;
        new_vehicle -> speed = 0;
        new_vehicle -> destination[0] = destination_x;
        new_vehicle -> destination[1] = destination_y;
        return new_vehicle;

    } else{
        return NULL;
    }
   
}

Vehicle* random_spawn_small_road_1(char **map, int start_point, int num_lanes){
    
    int spawn_x = start_point + randomInRange(0, num_lanes-1) + 1;
    int spawn_y = ROWS;


    int destination = randomInRange(1, 3);
    int destination_x;
    int destination_y;
    if (destination==1){
        destination_x = 1;
        destination_y = 1 + destination_set_on_big_road_1 + randomInRange(0, number_of_lanes_on_big_road_1-1);
    } else if(destination==2){
        destination_x = COLS;
        destination_y = 1 + destination_set_on_big_road_2 + randomInRange(0, number_of_lanes_on_big_road_2-1);
    } else if (destination==3){
        destination_x = 1 + destination_set_on_small_road_2 + randomInRange(0, number_of_lanes_on_small_road_2-1);
        destination_y = 1; 
    }
    


    if (map[spawn_x-1][spawn_y-1] == ' '){
        map[spawn_x-1][spawn_y-1] = '*';

        Vehicle * new_vehicle = malloc(sizeof(Vehicle));
        
        new_vehicle -> name = ++number_of_vehicles;
        new_vehicle -> x = spawn_x;
        new_vehicle -> y = spawn_y;
        new_vehicle -> road = 'b';
        new_vehicle -> arrive = 0 ;
        new_vehicle -> exit = 0;
        new_vehicle -> wait = 0;
        new_vehicle -> speed = 0;
        new_vehicle -> destination[0] = destination_x;
        new_vehicle -> destination[1] = destination_y;
        return new_vehicle;

    } else{
        return NULL;
    }
   
}

Vehicle* random_spawn_small_road_2(char **map, int start_point, int num_lanes){
    
    int spawn_x = start_point + randomInRange(0, num_lanes-1) + 1;
    int spawn_y = 1;


    int destination = randomInRange(1, 3);
    int destination_x;
    int destination_y;
    if (destination==1){
        destination_x = 1;
        destination_y = 1 + destination_set_on_big_road_1 + randomInRange(0, number_of_lanes_on_big_road_1-1);
    } else if(destination==2){
        destination_x = COLS;
        destination_y = 1 + destination_set_on_big_road_2 + randomInRange(0, number_of_lanes_on_big_road_2-1);
    } else if (destination==3){
        destination_x = 1 + destination_set_on_small_road_1 + randomInRange(0, number_of_lanes_on_small_road_1-1);
        destination_y = ROWS; 
    }
    


    if (map[spawn_x-1][spawn_y-1] == ' '){
        map[spawn_x-1][spawn_y-1] = '*';

        Vehicle * new_vehicle = malloc(sizeof(Vehicle));
        
        new_vehicle -> name = ++number_of_vehicles;
        new_vehicle -> x = spawn_x;
        new_vehicle -> y = spawn_y;
        new_vehicle -> road = 'b';
        new_vehicle -> arrive = 0 ;
        new_vehicle -> exit = 0;
        new_vehicle -> wait = 0;
        new_vehicle -> speed = 0;
        new_vehicle -> destination[0] = destination_x;
        new_vehicle -> destination[1] = destination_y;
        return new_vehicle;

    } else{
        return NULL;
    }
   
}



/*Maybe use file I/O to set map*/


int main(){
    printf("Running\n");
    char** map;
    char filename[] = "map16x10.txt";
    char temp;
    int size = 0;
    int time = 0;

    int big_green = 0;
    int big_light_time = 0;
    int big_max_time = 20;

    int small_green = 0;
    int small_light_time = 0;
    int small_max_time = 10;


    

    FILE *file = fopen(filename, "r");

    /*Read map from file I/O*/

    /*read size*/
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
        exit(1);
    }

    while ((temp = fgetc(file)) != EOF){
        if (temp == '\n'){
            ROWS ++;
            size --;
        }
        size ++;
    }
    ROWS++;
    COLS = size/ROWS;
    printf("Found: %d %d\n", COLS, ROWS);

    fclose(file);


    file = fopen(filename, "r");
    char array[COLS][ROWS];



    /*print to test map read*/
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS+1; j++) {
             fscanf(file, "%c", &temp);
                if(i<ROWS && j<COLS){
                    array[j][i] = temp;
                }
            }
        }
    fclose(file);
    

    int flag = -1;
    int order = 0;
    /*Find number of lanes on small road*/
    for (int i = 0; i<COLS-1; i++){
        if (array[i][0] == 'X'){
            if (array[i+1][0]!='X'){
            order ++;
            flag = 1;
            } else{
                flag = -1;
            }
        }
            
           
        else{
            if (flag ==1){
                if (order == 1){
                number_of_lanes_on_small_road_1 ++;
                }
                if (order == 2){
                number_of_lanes_on_small_road_2 ++;
            }
        }
    }
    }
    

    flag = -1;
    order = 0;
    /*Find number of lanes on big road*/
    for (int i = 0; i<ROWS-1; i++){
        if (array[0][i] == 'X'){
            if (array[0][i+1]!='X'){
            order ++;
            flag = 1;
            } else{
                flag = -1;
            }
        }
            
           
        else{
            if (flag ==1){
                if (order == 1){
                number_of_lanes_on_big_road_1 ++;
                }
                if (order == 2){
                number_of_lanes_on_big_road_2 ++;
            }
        }
    }
    }
    


    /*Declare map object*/
    map = (char**) malloc(COLS*(sizeof(char*)));
    if (map != NULL){
        for (int i = 0; i<COLS; i++){
            map[i] = (char*) malloc(ROWS*sizeof(char*));
        }
    }




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
    
    
    destination_set_on_big_road_1 -= (number_of_lanes_on_big_road_1-1);
    destination_set_on_big_road_2 -= (number_of_lanes_on_big_road_2-1);
    destination_set_on_small_road_1 -= (number_of_lanes_on_small_road_1-1);
    destination_set_on_small_road_2 -= (number_of_lanes_on_small_road_2-1);
    spawn_set_on_big_road_1 -= (number_of_lanes_on_big_road_1-1);
    spawn_set_on_big_road_2 -= (number_of_lanes_on_big_road_2-1);
    spawn_set_on_small_road_1 -= (number_of_lanes_on_small_road_1-1);
    spawn_set_on_small_road_2 -= (number_of_lanes_on_small_road_2-1);
    

    
       
    
        
    /*Drive code*/
    struct node* current;
    struct node* place_holder;
    Vehicle* new_vehicle;
    
    
    big_green =1;
    while (time < SIMULATION_TIME_INTERVAL){
        big_green = 1;
        small_green = 1;
        if (big_light_time == big_max_time){
           big_green = 0;
           small_green = 1;
        } else if (small_light_time == small_max_time){
            small_green = 0;
            big_green = 1;
        }

        if (big_green ==1){
            big_light_time ++;
            new_vehicle = random_spawn_big_road_1(map, spawn_set_on_big_road_1, number_of_lanes_on_big_road_1);
            if (new_vehicle!=NULL){
                insert(new_vehicle);
            }
            new_vehicle = random_spawn_big_road_2(map, spawn_set_on_big_road_2, number_of_lanes_on_big_road_2);
            if (new_vehicle!=NULL){
                insert(new_vehicle);
            }

        }
        if (small_green ==1){
            small_light_time ++;
            new_vehicle = random_spawn_small_road_1(map, spawn_set_on_small_road_1, number_of_lanes_on_small_road_1);
            if (new_vehicle!=NULL){
                insert(new_vehicle);
            }
            new_vehicle = random_spawn_small_road_2(map, spawn_set_on_small_road_2, number_of_lanes_on_small_road_2);
            if (new_vehicle!=NULL){
                insert(new_vehicle);
            }
        }


         
       
        
      
        current = head;

        /*Refresh map*/
        map_print(map);
        printf("\nTime: %d", time);
        printf("\nPassed: %d", counter);
        if (big_green == 1){
            printf("\nStatus: Green light on horizontal road");
        } else if(small_green == 1){
             printf("\nStatus: Green light on vertical road");
        }
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

  
