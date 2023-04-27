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
    printf("\n???? %d %d %d\n", destination_x, destination_y, destination);


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
    printf("\n???? %d %d %d\n", destination_x, destination_y, destination);


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
    printf("\n???? %d %d %d\n", destination_x, destination_y, destination);


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
    






    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
                printf("%c", array[j][i]);
            }
            printf("\n");
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
    
    /*Thay bang linked list va random -> generate car*/
    /*
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
    */


    //sleep(2);
    //system("cls");
    printf("Spawn : %d %d %d %d\n", spawn_set_on_big_road_1+1, spawn_set_on_big_road_2+1, spawn_set_on_small_road_1+1, spawn_set_on_small_road_2+1 );
    printf("Desti : %d %d %d %d", destination_set_on_big_road_1+1, destination_set_on_big_road_2+1, destination_set_on_small_road_1+1, destination_set_on_small_road_2+1);

    //printf("\nHi->%d",1 + destination_set_on_small_road_1 + randomInRange(0, number_of_lanes_on_small_road_1-1));
    //sleep(1000);
    sleep(1);
    Vehicle* new_vehicle;
    new_vehicle = random_spawn_small_road_1(map, spawn_set_on_small_road_1, number_of_lanes_on_small_road_1);
        if (new_vehicle!=NULL){
            insert(new_vehicle);
        }
    
    
    
    printf("\ndestination: %d %d", new_vehicle->destination[0], new_vehicle->destination[1]);
    printf("\nstart: %d %d",new_vehicle->x, new_vehicle->y );
    sleep(3);
       
    
        
    /*Drive code*/
    struct node* current;
    struct node* place_holder;
    
    

    while (time < SIMULATION_TIME_INTERVAL){
       
        
      
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

  /*
        - Doc size map luon tu file I/O - >su dung het malloc

        - Can chinh lai thu tu cua cai linked list
        - Can co ham random() de spawn() xe
        - spawn coi nhu la mot cai move vao vi tri xuat phat
        - Can co 1 tap cac vi tri spawn duoc doc vao tu map

        - random_spawn_big_road -> den xanh o big road thi moi spawn:
            + nhan vao map de biet o vi tri can spawn dang co xe nao khong
            + Can chuan bi array cac vi tri spawn, destination duoc duoc tu map
                spawn_big_1_y + big_lanes
                spawn_big_2_y + big_lanes
                spawn_small_1_x + small_lanes
                spawn_small_2_x + small_lanes

                destination_big_1_y
                destination_big_2_y
                destination_small_1_x
                destination_small_2_x




                    int spawn_set_on_big_road_1; // + (1, y)
                    int spawn_set_on_big_road_2; // + (COLS, y)
                    int spawn_set_on_small_road_1; // + (x, ROWS)
                    int spawn_set_on_small_road_2; // + (x, 1)




                    int destination_set_on_big_road_1; // - (1, y) p
                    int destination_set_on_big_road_2; // - (COLS, y) p
                    int destination_set_on_small_road_1; // - (x, ROWS) p
                    int destination_set_on_small_road_2; // - (x, 1) 



            + return ra pointer vehicle
            + su dung pointer vehicle do de insert()
        random_spawn_small_road -> den xanh o big road thi moi spawn (tuong tu ...big_road)
        */
