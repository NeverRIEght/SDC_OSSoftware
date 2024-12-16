//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <sys/stat.h>
//#include <dirent.h>
//#include <time.h>
//
//typedef struct FileInfo {
//    char name[256];
//    time_t creation_time;
//    ino_t inode;
//} FileInfo;
//
//typedef struct Node {
//    FileInfo data;
//    struct Node *next;
//} Node;
//
//typedef struct LinkedList {
//    struct Node *head;
//    int length;
//} LinkedList;
//
//typedef struct Snapshot {
//    LinkedList *list;
//    struct Snapshot *next;
//    char *creation_time;
//} Snapshot;
//
//typedef struct SnapshotHistory {
//    Snapshot *head;
//    int length;
//    char directoryName[256];
//} SnapshotHistory;
//
//// List related functions
//LinkedList *createLinkedList(Node *head);
//
//void freeList(LinkedList *list);
//
//void printList(LinkedList *list);
//
//Node *createNode(const char *name, time_t creation_time, ino_t inode);
//
//void insertNode(LinkedList *list, Node *newNode);
//
//void deleteNode(LinkedList *list, const char *name);
//
//Node *searchNode(LinkedList *list, const char *name);
//
//// Snapshot related functions
//SnapshotHistory *createSnapshotHistory(char *directoryName);
//
//Snapshot *createSnapshot(SnapshotHistory *history);
//
//void addSnapshotToHistory(SnapshotHistory *history, Snapshot *snapshot);
//
//Snapshot *selectSnapshot(SnapshotHistory *history);
//
//// Other functions
//void checkDirectory(char path[512]);
//
//void scanDirectory(LinkedList *list, const char *path);
//
//void printFileInfo(LinkedList *list, const char *name);
//
//char *getCurrentDateTime();
//
//void cleanUp(SnapshotHistory *history);
//
//
//LinkedList *createLinkedList(Node *head) {
//    LinkedList *list = malloc(sizeof(LinkedList));
//    if (list == NULL) {
//        printf("Error: cannot allocate memory for a new LinkedList\n");
//        exit(1);
//    }
//
//    // set new head, which can be null
//    list->head = head;
//
//    list->length = 0;
//    Node *current = list->head;
//    while (current != NULL) {
//        list->length++;
//        current = current->next;
//    }
//
//    printf("New LinkedList created\n");
//    return list;
//}
//
//void freeList(LinkedList *list) {
//    while (list->head != NULL) {
//        Node *temp = list->head;
//        list->head = list->head->next;
//        free(temp);
//    }
//    list->length = 0;
//}
//
//void printList(LinkedList *list) {
//    printf("LinkedList:\n");
//
//    Node *current = list->head;
//    if (current == NULL) {
//        printf("empty.\n");
//        return;
//    }
//
//    while (current != NULL) {
//        printf("%s\n", current->data.name);
//        current = current->next;
//    }
//}
//
//Node *createNode(const char *name, time_t creation_time, ino_t inode) {
//    Node *newNode = malloc(sizeof(Node));
//    if (newNode == NULL) {
//        printf("Error: cannot allocate memory for a new node\n");
//        exit(1);
//    }
//
//    // copy name
//    strncpy(newNode->data.name, name, sizeof(newNode->data.name) - 1);
//    newNode->data.name[sizeof(newNode->data.name) - 1] = '\0';
//
//    // copy creation time
//    newNode->data.creation_time = creation_time;
//
//    // copy inode
//    newNode->data.inode = inode;
//
//    newNode->next = NULL;
//    return newNode;
//}
//
//void insertNode(LinkedList *list, Node *newNode) {
//    newNode->next = list->head;
//    list->head = newNode;
//    list->length++;
//}
//
//void deleteNode(LinkedList *list, const char *name) {
//    Node *current = list->head;
//    Node *prev = NULL;
//
//    while (current) {
//        if (strcmp(current->data.name, name) == 0) {
//            if (prev) {
//                prev->next = current->next;
//            } else {
//                list->head = current->next;
//            }
//            free(current);
//            list->length--;
//            printf("Object \"'%s'\" deleted\n", name);
//            return;
//        }
//        prev = current;
//        current = current->next;
//    }
//    printf("Object \"'%s'\" not found\n", name);
//}
//
//Node *searchNode(LinkedList *list, const char *name) {
//    Node *current = list->head;
//    while (current) {
//        if (strcmp(current->data.name, name) == 0) {
//            return current;
//        }
//        current = current->next;
//    }
//    printf("Object \"'%s'\" not found", name);
//    return NULL;
//}
//
//SnapshotHistory *createSnapshotHistory(char *directoryName) {
//    SnapshotHistory *history = malloc(sizeof(SnapshotHistory));
//    if (history == NULL) {
//        printf("Error: cannot allocate memory for a new SnapshotHistory\n");
//        exit(1);
//    }
//
//    history->head = NULL;
//    history->length = 0;
//    strncpy(history->directoryName, directoryName, sizeof(history->directoryName) - 1);
//
//    return history;
//}
//
//Snapshot *createSnapshot(SnapshotHistory *history) {
//    Snapshot *snapshot = (Snapshot *) malloc(sizeof(Snapshot));
//    if (snapshot == NULL) {
//        printf("Error: cannot allocate memory for a new snapshot\n");
//        exit(1);
//    }
//
//    LinkedList *snapshotList = createLinkedList(NULL);
//    scanDirectory(snapshotList, history->directoryName);
//    snapshot->list = snapshotList;
//    snapshot->creation_time = getCurrentDateTime();
//
//    printf("Created snapshot for \"%s\" directory (%s)\n", history->directoryName, snapshot->creation_time);
//    return snapshot;
//}
//
//void addSnapshotToHistory(SnapshotHistory *history, Snapshot *snapshot) {
//    // History is limited to 10 snapshots
//    if (history->length < 10) {
//        snapshot->next = history->head;
//        history->head = snapshot;
//        history->length++;
//    } else {
//        // Remove the oldest snapshot
//        Snapshot *current = history->head;
//        while (current->next->next != NULL) {
//            current = current->next;
//        }
//        free(current->next);
//        current->next = NULL;
//        snapshot->next = history->head;
//        history->head = snapshot;
//    }
//}
//
//Snapshot *selectSnapshot(SnapshotHistory *history) {
//    printf("SnapshotHistory:\n");
//    Snapshot *current = history->head;
//    int i = 0;
//    while (current != NULL) {
//        printf("%d. %s\n", i, current->creation_time);
//        current = current->next;
//        i++;
//    }
//
//    int index;
//    printf("Select snapshot index (0 - %d): ", history->length - 1);
//    scanf("%d", &index);
//
//    if (index < 0 || index >= history->length) {
//        printf("Error: invalid index\n");
//        return selectSnapshot(history);
//    }
//
//    current = history->head;
//    for (int i = 0; i < index; i++) {
//        current = current->next;
//    }
//
//    return current;
//}
//
//void checkDirectory(char path[512]) {
//    if (path == NULL) {
//        printf("Error: path is null\n");
//        exit(1);
//    }
//
//    DIR *dir = opendir(path);
//    if (dir) {
//        closedir(dir);
//    } else {
//        printf("Error: directory does not exists\n");
//        exit(1);
//    }
//}
//
//void scanDirectory(LinkedList *list, const char *path) {
//    freeList(list);
//
//    struct dirent *element;
//    struct stat elementStat;
//    DIR *dir = opendir(path);
//    if (dir == NULL) {
//        printf("Error while trying to open directory\n");
//        return;
//    }
//
//    while ((element = readdir(dir)) != NULL) { // for each element in directory
//        if (strcmp(element->d_name, ".") == 0 && strcmp(element->d_name, "..") == 0) { // exclude .. and . elements
//            continue;
//        }
//
//        printf("Scanning: %s\n", element->d_name);
//
//        char fullPath[512];
//        // Concatenate path and element name to get full path
//        snprintf(fullPath, sizeof(fullPath), "%s/%s", path, element->d_name);
//
//        if (stat(fullPath, &elementStat) == 0) { // if file info was successfully retrieved
//            Node *newNode = createNode(element->d_name, // file name
//                                       elementStat.st_ctime, // creation time
//                                       elementStat.st_ino); // inode
//            insertNode(list, newNode);
//        }
//    }
//
//    closedir(dir);
//}
//
//void printFileInfo(LinkedList *list, const char *name) {
//    Node *found = searchNode(list, name);
//    if (found == NULL) {
//        return;
//    }
//
//    printf("Name: %s\n", found->data.name);
//    printf("Creation timestamp: %lld\n", found->data.creation_time);
//    printf("Inode: %lu\n", (unsigned long) found->data.inode);
//}
//
//char *getCurrentDateTime() {
//    time_t rawTime;
//    struct tm *timeInfo;
//
//    time(&rawTime);
//    timeInfo = localtime(&rawTime);
//
//    return asctime(timeInfo);
//}
//
//void cleanUp(SnapshotHistory *history) {
//    Snapshot *current = history->head;
//    while (current != NULL) {
//        Snapshot *temp = current;
//        current = current->next;
//        freeList(temp->list);
//        free(temp);
//    }
//    free(history);
//}
//
//int main() {
//    char path[512];
//
//    printf("Enter path to directory: ");
//    scanf("%s", path);
//
//    checkDirectory(path);
//    SnapshotHistory *history = createSnapshotHistory(path);
//    char object_name[256];
//    int choice;
//
//    while (1) {
//        printf("\nSelect operation:\n");
//        printf("1. Scan directory\n");
//        printf("2. Get objects list\n");
//        printf("3. Get object details\n");
//        printf("4. Exit\n");
//        printf("Your selection: ");
//        scanf("%d", &choice);
//
//        switch (choice) {
//            case 1:
//                addSnapshotToHistory(history, createSnapshot(history));
//                break;
//            case 2:
//                if (history->head == NULL) {
//                    printf("No snapshots available\n");
//                    break;
//                }
//                Snapshot *snap = selectSnapshot(history);
//                printf("Selected snapshot: %s\n", snap->creation_time);
//                printList(snap->list);
//                break;
//            case 3:
//                if (history->head == NULL) {
//                    printf("No snapshots available\n");
//                    break;
//                }
//                snap = selectSnapshot(history);
//                printf("Selected snapshot: %s\n", snap->creation_time);
//                printList(snap->list);
//
//                printf("Enter object name to show info: ");
//                scanf("%s", object_name);
//
//                printFileInfo(snap->list, object_name);
//                break;
//            case 4:
//                printf("Exiting...\n");
//                cleanUp(history);
//                return 0;
//            default:
//                printf("Incorrect input, please try again.\n");
//        }
//    }
//
//    cleanUp(history);
//    return 0;
//}
