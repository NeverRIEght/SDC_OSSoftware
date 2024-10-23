#include <stdio.h>

int main() {
    char first_name[20];
    char last_name[20];
    char interests[200];
    printf("Enter your first name: ");
    scanf("%s", first_name);
    printf("Enter your last name: ");
    scanf("%s", last_name);
    printf("Enter your interests: ");
    scanf("%s", interests);

    printf("Your name is %s %s and you are interested in %s\n", first_name, last_name, interests);


    return 0;
}