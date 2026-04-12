#include <stdio.h>
#include <stdlib.h>

struct Task
{
    int id;
    char task[100];
};

void reset(){
    FILE *fp = fopen("task.bin", "wb");

    if(!fp){
        printf("Reset failed!\n");
        return;
    }

    fclose(fp);

    printf("All tasks deleted successfully!\n");
}

void add(struct Task arg){
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
        reset();
        return;
    }

    struct Task temp;

    printf("\n--- TASK LIST ---\n");

    while(fread(&temp, sizeof(struct Task), 1, fp)){
        printf("ID: %d | Task: %s\n", temp.id, temp.task);
    }

    fclose(fp);

    printf("\nPress Enter to continue...");
    getchar();
    getchar();
}

int main(){
    int opt;

    while(1){
        printf("+----------------------+\n");
        printf("|         MENU         |\n");
        printf("+----------------------+\n");
        printf("| 1. ADD TASK          |\n");
        printf("| 2. REMOVE TASK       |\n");
        printf("| 3. LIST TASK         |\n");
        printf("| 4. EXIT              |\n");
        printf("| 5. RESET             |\n");
        printf("+----------------------+\n");

        scanf("%d",&opt);

        if(opt==1){
            struct Task temp;
            printf("enter ID :");
            scanf("%d",&temp.id);
            printf("enter Task :");
            scanf(" %[^\n]", temp.task);
            add(temp);
        }
        else if(opt==2){
            int ID;
            printf("enter ID :");
            scanf("%d",&ID);
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
            reset();
        }

        system("cls");
    }

    return 0;
}
