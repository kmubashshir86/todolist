#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <string.h>
#include <windows.h>

int exists(int ID);
struct Task
{
    int id;
    char task[100];
    char date[20];
    bool status;
};

void clear(){
    #ifdef _WIN32
        system("cls");
    #elif (__linux__ or __APPLE__)
        system("clear");
    #endif
}

void reset(){
    FILE *fp = fopen("task.bin", "wb");

    if(!fp){
        printf("Reset failed!\n");
        return;
    }

    fclose(fp);

    printf("All tasks deleted successfully!\n");
    #ifdef _WIN32
    Sleep(1000);
    #else
        sleep(1);
    #endif
}

void clearBuffer(){
    while(getchar()!='\n');
}

void add(struct Task arg){

    if(exists(arg.id)){
        printf("ID already exists!\n");
        return;
    }

    // 🔥 current date auto fill
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    sprintf(arg.date, "%02d-%02d-%d",
            tm.tm_mday, tm.tm_mon+1, tm.tm_year+1900);

    FILE *fp = fopen("task.bin","ab");
    if(!fp){
        printf("File error!\n");
        return;
    }

    fwrite(&arg, sizeof(struct Task), 1, fp);
    fclose(fp);
}

void rem(int ID){
    FILE *fp = fopen("task.bin","rb");
    FILE *tempf = fopen("temp.bin","wb");

    if(!fp || !tempf){
        printf("File error!\n");
        return;
    }

    struct Task temp;

    while(fread(&temp, sizeof(struct Task), 1, fp)){
        if(temp.id != ID){
            fwrite(&temp, sizeof(struct Task), 1, tempf);
        }
    }

    fclose(fp);
    fclose(tempf);

    remove("task.bin");
    rename("temp.bin","task.bin");

    printf("Task removed (if existed)\n");
}

void show(){
    FILE *fp = fopen("task.bin","rb");

    if(!fp){
        printf("No file found. Creating new...\n");
        return;
    }

    struct Task temp;

    printf("\n--- TASK LIST ---\n");

    while(fread(&temp, sizeof(struct Task), 1, fp)){
        printf("ID: %d | Task: %s | Date: %s | %s\n",temp.id, temp.task, temp.date,temp.status ? "DONE" : "PENDING");
    }

    fclose(fp);

    printf("\nPress Enter to continue...");
    getchar();
}

void search(int ID){
    FILE *fp = fopen("task.bin","rb");

    if(!fp){
        printf("File not found!\n");
        return;
    }

    struct Task temp;
    int found = 0;

    while(fread(&temp, sizeof(struct Task), 1, fp)){
        if(temp.id == ID){
            printf("\nTask Found!\n");
            printf("ID: %d | Task: %s | Date: %s\n",temp.id, temp.task, temp.date);
            found = 1;
            break;
        }
    }

    if(!found){
        printf("Task not found!\n");
    }

    fclose(fp);

    printf("\nPress Enter to continue...");
    getchar();
}
void update(int ID){
    FILE *fp = fopen("task.bin","rb+");
    struct Task temp;

    if(!fp){
        printf("File not found!\n");
        return;
    }

    while(fread(&temp, sizeof(temp), 1, fp)){
        if(temp.id == ID){
            printf("Enter new task: ");
            fgets(temp.task, sizeof(temp.task), stdin);
            temp.task[strcspn(temp.task, "\n")] = 0;

            fseek(fp, -sizeof(temp), SEEK_CUR);
            fwrite(&temp, sizeof(temp), 1, fp);

            printf("Task updated!\n");
            fclose(fp);
            return;
        }
    }

    printf("Task not found!\n");
    fclose(fp);
}

void count(){
    FILE *fp = fopen("task.bin","rb");
    struct Task t;
    int total=0;

    if(!fp){
        printf("No tasks yet!\n");
        return;
    }

    while(fread(&t,sizeof(t),1,fp)){
        total++;
    }

    printf("Total Tasks: %d\n", total);
    fclose(fp);
    printf("press enter to continue\n");
    getchar();
    
}

int exists(int ID){
    FILE *fp = fopen("task.bin","rb");
    struct Task t;

    if(!fp) return 0;

    while(fread(&t,sizeof(t),1,fp)){
        if(t.id == ID){
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void markDone(int ID){
    FILE *fp = fopen("task.bin","rb");
    FILE *tempf = fopen("temp.bin","wb");
    FILE *donef = fopen("done.bin","ab");

    if(!fp || !tempf || !donef){
    printf("File error!\n");
    if(fp) fclose(fp);
    if(tempf) fclose(tempf);
    if(donef) fclose(donef);
    return;
}

    struct Task t;
    int found = 0;

    while(fread(&t, sizeof(t), 1, fp)){
        if(t.id == ID){
            t.status = 1;
            fwrite(&t, sizeof(t), 1, donef); 
            found = 1;
        } else {
            fwrite(&t, sizeof(t), 1, tempf);
        }
    }

    fclose(fp);
    fclose(tempf);
    fclose(donef);

    remove("task.bin");
    rename("temp.bin","task.bin");

    if(found)
        printf("Task marked as DONE!\n");
    else
        printf("Task not found!\n");
}
void showDone(){
    FILE *fp = fopen("done.bin","rb");

    if(!fp){
        printf("No done tasks yet!\n");
        return;
    }

    struct Task t;

    printf("\n--- DONE TASKS ---\n");

    while(fread(&t,sizeof(t),1,fp)){
        printf("ID: %d | Task: %s | Date: %s\n",
                t.id, t.task, t.date);
    }

    fclose(fp);
    printf("\nPress Enter to continue...");
    getchar();
}
void resetDone(){
    FILE *fp = fopen("done.bin", "wb");

    if(!fp){
        printf("Reset failed!\n");
        return;
    }

    fclose(fp);

    printf("All DONE tasks deleted!\n");
}

int safeIntInput(int *num){
    if(scanf("%d", num) != 1){
        clearBuffer();
        return 0;
    }
    return 1;
}

int main(){
    int opt;

    while(1){
        printf("+----------------------+\n");
        printf("|         MENU         |\n");
        printf("+----------------------+\n");
        printf("| 1.  ADD TASK         |\n");
        printf("| 2.  REMOVE TASK      |\n");
        printf("| 3.  LIST TASK        |\n");
        printf("| 4.  EXIT             |\n");
        printf("| 5.  RESET            |\n");
        printf("| 6.  SEARCH BY ID     |\n");
        printf("| 7.  COUNT TASKS      |\n");
        printf("| 8.  UPDATE           |\n");
        printf("| 9.  DONE             |\n");
        printf("| 10. HISTORY          |\n");
        printf("| 11. CLEAR HISTORY    |\n");
        printf("+----------------------+\n");

        scanf("%d",&opt);
        clearBuffer();

        if(opt==1){
            struct Task temp;
            printf("enter ID :");
            if(!safeIntInput(&temp.id)){
                printf("Invalid input!\n");
                continue;
            }
            clearBuffer();
            printf("enter Task :");
            fgets(temp.task, sizeof(temp.task), stdin);
temp.task[strcspn(temp.task, "\n")] = 0;
            clearBuffer();
            temp.status=0;
            add(temp);
        }
        else if(opt==2){
            int ID;
            printf("enter ID :");
            scanf("%d",&ID);
            clearBuffer();
            rem(ID);
        }
        else if(opt==3){
            show();
        }
        else if(opt==4){
            printf("EXITING...\n");
            exit(0);
        }
        else if(opt==5){
            reset();}
        else if(opt==6){
            int n;
            printf("enter number : ");
            scanf("%d",&n);
            clearBuffer();
            search(n);

        }
        else if(opt==7){
            count();
        }
        else if(opt==8){
            int i_d;
            printf("enter id : ");
            scanf("%d",&i_d);
            clearBuffer();
            update(i_d);
        }
        else if(opt==9){
            int id;
            printf("Enter ID to mark DONE: ");
            scanf("%d",&id);
            clearBuffer();
            markDone(id);}
        else if(opt==10){
        showDone();}
        else if(opt==11){
            resetDone();
        }
        else{
        }
        clear();
    }
    

    return 0;
}
