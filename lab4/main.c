/* Обращение к полям структуры
 struct person {
     int age
     char* name
 }
 p->age = 10

 Короч по структурам лучше прочитать метанит

 Обращать внимание на копирование структуры, т.к. при копировании указателя и дальнейшем его
 занулении может привести к указателю на null в предыдущей структуре, от которой мы создаем копию

 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

typedef struct FileInfo {
    char name[256];
    time_t creation_time;
    ino_t inode;
} FileInfo;

typedef struct Node {
    FileInfo data;
    struct Node *next;
} Node;

typedef struct LinkedList {
    struct Node *head;
    int length;
} LinkedList;

LinkedList *createLinkedList(Node *head) {
    LinkedList *list = (LinkedList *) malloc(sizeof(LinkedList));
    if (list == NULL) {
        printf("Error: cannot allocate memory for a new LinkedList\n");
        exit(1);
    }

    // set new head, which can be null
    list->head = head;

    list->length = 0;
    Node *current = list->head;
    while (current != NULL) {
        list->length++;
        current = current->next;
    }

    printf("New LinkedList created\n");
    return list;
}

void calculateLength(LinkedList* list) {
    int length = 0;
    Node *current = list->head;
    while (current != NULL) {
        length++;
        current = current->next;
    }

    list->length = length;
}

Node *createNode(const char *name, time_t creation_time, ino_t inode) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    if (newNode == NULL) {
        printf("Error: cannot allocate memory for a new node\n");
        exit(1);
    }

    // copy name
    strncpy(newNode->data.name, name, sizeof(newNode->data.name) - 1);
    newNode->data.name[sizeof(newNode->data.name) - 1] = '\0';

    // copy creation time
    newNode->data.creation_time = creation_time;

    // copy inode
    newNode->data.inode = inode;

    newNode->next = nullptr;
    return newNode;
}

void insertNode(LinkedList *list, Node* newNode) {
    newNode->next = list->head;
    list->head = newNode;
    list->length++;
}

void freeList(LinkedList *list) {
    while (list->head != NULL) {
        Node *temp = list->head;
        list->head = list->head->next;
        free(temp);
    }
    list->length = 0;
}

void scanDirectory(LinkedList *list, const char *path) {
    freeList(list);

    struct dirent *entry;
    struct stat fileStat;
    DIR *dir = opendir(path);
    if (dir == NULL) {
        perror("Error while trying to open directory\n");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        char fullPath[512];
        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, entry->d_name);

        if (stat(fullPath, &fileStat) == 0 && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            Node *newNode = createNode(entry->d_name, fileStat.st_ctime, fileStat.st_ino);
            insertNode(list, newNode);
        }
    }

    closedir(dir);
}

Node *search(LinkedList *list, const char *name) {
    Node *current = list->head;
    while (current) {
        if (strcmp(current->data.name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    printf("Object \"'%s'\" not found", name);
    return nullptr;
}

void deleteNode(LinkedList *list, const char *name) {
    Node *current = list->head;
    Node *prev = nullptr;

    while (current) {
        if (strcmp(current->data.name, name) == 0) {
            if (prev) {
                prev->next = current->next;
            } else {
                list->head = current->next;
            }
            free(current);
            list->length--;
            printf("Object \"'%s'\" deleted\n", name);
            return;
        }
        prev = current;
        current = current->next;
    }
    printf("Object \"'%s'\" not found\n", name);
}

void printList(LinkedList *list) {
    printf("LinkedList:\n");

    Node *current = list->head;
    if (current == NULL) {
        printf("empty.\n");
        return;
    }

    while (current != NULL) {
        printf("%s\n", current->data.name);
        current = current->next;
    }
}

void printFileInfo(LinkedList *list, const char *name) {
    Node *found = search(list, name);
    if (found == NULL) {
        return;
    }

    printf("Имя: %s\n", found->data.name);
    printf("Время создания: %ld\n", found->data.creation_time);
    printf("Inode: %lu\n", (unsigned long)found->data.inode);
}

int main() {
    LinkedList *list = createLinkedList(NULL);
    char path[512];
    char input[256];
    int choice;

    printf("Введите путь к каталогу: ");
    scanf("%s", path);

    while (1) {
        printf("\nВыберите операцию:\n");
        printf("1. Просканировать каталог\n");
        printf("2. Вывести список объектов\n");
        printf("3. Вывести информацию об объекте\n");
        printf("4. Выход\n");
        printf("Ваш выбор: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                scanDirectory(list, path);
                printf("Каталог '%s' просканирован\n", path);
                break;
            case 2:
                printList(list);
                break;
            case 3:
                printf("Введите имя объекта: ");
                scanf("%s", input);
                printFileInfo(list, input);
                break;
            case 4:
                freeList(list);
                free(list);
                printf("Выход из программы\n");
                return 0;
            default:
                printf("Неверный выбор, попробуйте снова\n");
        }
    }
}



