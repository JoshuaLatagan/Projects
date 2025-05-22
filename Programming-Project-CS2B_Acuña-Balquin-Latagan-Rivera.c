#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define MAX_RANGE 10000000
#define MAX_ALGOS 6
#define MAX_RUNS 5
#define FILE_NAME "sorted_output.txt"

void displayTitle();
void displaySortingMenu();
int promptSizeInput();
void generateRandomIntegers(int arr[], int size);
void generateIncreasingSequence(int arr[], int size, int start);
char sortArrayAndDisplay(int arr[], int size, char *dataType, int *originalArr, int *pTotalSortCount);
void writeToFile(int arr[], int size, char *dataType, char *sortingMethod, double execTime, int *originalArr, int sortNumber);
void clearScreen();
void mergeSort(int arr[], int left, int right);
void quickSort(int arr[], int low, int high);
void heapSort(int arr[], int size);
void selectionSort(int arr[], int size);
void bubbleSort(int arr[], int size);
void insertionSort(int arr[], int size);
void merge(int arr[], int left, int middle, int right);
void swap(int *a, int *b);
int partition(int arr[], int low, int high);
void heapify(int arr[], int n, int i);
void copyArray(int source[], int destination[], int size);

typedef struct {
    char name[30];
    double times[MAX_RUNS];
} SortResult;

void displayTitle() {
    clearScreen();

    const char *titleLines[] = {
        " .d8888.  .d88b.  d8888b. d888888b d888888b d8b   db  d888b        .d8b.  db       d888b   .d88b.  d8888b. d888888b d888888b db   db .88b  d88. .d8888. ",
        " 88'  YP .8P  Y8. 88  `8D `~~88~~'   `88'   888o  88 88' Y8b      d8' `8b 88      88' Y8b .8P  Y8. 88  `8D   `88'   `~~88~~' 88   88 88'YbdP`88 88'  YP ",
        " `8bo.   88    88 88oobY'    88       88    88V8o 88 88           88ooo88 88      88      88    88 88oobY'    88       88    88ooo88 88  88  88 `8bo.   ",
        "   `Y8b. 88    88 88`8b      88       88    88 V8o88 88  ooo      88~~~88 88      88  ooo 88    88 88`8b      88       88    88~~~88 88  88  88   `Y8b. ",
        " db   8D `8b  d8' 88 `88.    88      .88.   88  V888 88. ~8~      88   88 88booo. 88. ~8~ `8b  d8' 88 `88.   .88.      88    88   88 88  88  88 db   8D ",
        " `8888Y'  `Y88P'  88   YD    YP    Y888888P VP   V8P  Y888P       YP   YP Y88888P  Y888P   `Y88P'  88   YD Y888888P    YP    YP   YP YP  YP  YP `8888Y' "
    };

    int numTitleLines = sizeof(titleLines) / sizeof(titleLines[0]);
    int titleLineWidth = 0;
    if (numTitleLines > 0) {
      titleLineWidth = strlen(titleLines[0]);
    }
    int consoleWidth = 210;
    int titlePadding = (consoleWidth - titleLineWidth) / 2;
    if (titlePadding < 0) {
        titlePadding = 0;
    }

    printf("\n\n\n\n\n");
    for (int i = 0; i < numTitleLines; i++) {
        for (int j = 0; j < titlePadding; j++) {
            printf(" ");
        }
        printf("%s\n", titleLines[i]);
    }

    printf("\n");

    const char *subTitle = "(C Programming Language)";
    int subTitleLen = strlen(subTitle);
    int subTitlePadding = (consoleWidth - subTitleLen) / 2;
    if (subTitlePadding < 0) {
        subTitlePadding = 0;
    }

    for (int i = 0; i < subTitlePadding; i++) {
        printf(" ");
    }
    printf("%s\n\n\n\n\n", subTitle);

    printf("\t\t\t\t\t\t\t\t\t\t\t       Elyzel Jade Rivera\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t       Angelo James Acuna\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t       Joshua Rene Latagan\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t       John Jacob Balquin\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t          (BSCS - 2B)\n\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t       Arlene A. Satuito\n");
    printf("\t\t\t\t\t\t\t\t\t\t\t        (DAA Professor)\n\n");
    getchar();
}

int main() {
    int size;
    char dataGenChoice;
    char continueChoice = 'y';
    char changeAlgoChoice = 'n';
    int totalSortCount = 0;

    int *arr = NULL;
    int *originalArr = NULL;
    char dataType[50];

    displayTitle();

    do {
        clearScreen();

        size = promptSizeInput();

        arr = malloc(size * sizeof(int));
        originalArr = malloc(size * sizeof(int));
        if (arr == NULL || originalArr == NULL) {
            printf("Memory allocation failed.\n");
            if (arr) free(arr);
            if (originalArr) free(originalArr);
            return 1;
        }

        printf("\nChoose the data generation method:\n");
        printf("A. Randomly Generated Integers\n");
        printf("B. Increasing Sequence\n");
        printf("Enter your choice: ");
        scanf(" %c", &dataGenChoice);
        while(getchar() != '\n');

        if (dataGenChoice == 'A' || dataGenChoice == 'a') {
            generateRandomIntegers(arr, size);
            strcpy(dataType, "Randomly Generated Numbers");
        } else if (dataGenChoice == 'B' || dataGenChoice == 'b') {
            int startValue;
            printf("Enter the starting value for the increasing sequence: ");
            while (scanf("%d", &startValue) != 1) {
                printf("Invalid input. Please enter an integer: ");
                while(getchar() != '\n');
            }
            while(getchar() != '\n');
            generateIncreasingSequence(arr, size, startValue);
            strcpy(dataType, "Increasing Sequence");
        } else {
            printf("\nInvalid data generation choice! Defaulting to random.\n");
            generateRandomIntegers(arr, size);
            strcpy(dataType, "Randomly Generated Numbers");
        }

        copyArray(arr, originalArr, size);

        do {
            copyArray(originalArr, arr, size);

            sortArrayAndDisplay(arr, size, dataType, originalArr, &totalSortCount);

            printf("\nTry another sorting algorithm on the SAME data? (y/n): ");
            scanf(" %c", &changeAlgoChoice);
            while(getchar() != '\n');

        } while (tolower(changeAlgoChoice) == 'y');

        free(arr);
        arr = NULL;
        free(originalArr);
        originalArr = NULL;

        printf("\nWould you like to continue? (y/n): ");
        scanf(" %c", &continueChoice);
        while(getchar() != '\n');

    } while (tolower(continueChoice) == 'y');

    printf("\nAttempting to open sorted_output.txt...");
    fflush(stdout);

    int ret = -1;
    #ifdef _WIN32
        ret = system("start sorted_output.txt");
    #elif __APPLE__
        ret = system("open sorted_output.txt");
    #elif __linux__
        ret = system("xdg-open sorted_output.txt");
    #else
        printf("\nCannot automatically open file on this OS.");
    #endif

    if (ret != 0 && ret != -1) {
         printf("\nCould not automatically open file (Error code: %d).", ret);
    }

    printf("\nProgram finished. Press Enter to exit.\n");
    getchar();

    return 0;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        printf("\033[H\033[J");
        fflush(stdout);
    #endif
}

int promptSizeInput() {
    int size;
    const int MAX_ALLOWED_SIZE = 1000000;

    while (1) {
        printf("Enter the size of the array (1-%d): ", MAX_ALLOWED_SIZE);

        if (scanf("%d", &size) == 1) {
             while(getchar() != '\n'); 
             
            if (size >= 1 && size <= MAX_ALLOWED_SIZE) {
                return size;
            } else {
                
                 printf("Invalid range. Please enter a number between 1 and %d.\n", MAX_ALLOWED_SIZE);
            }
        } else {
            printf("Invalid input. Please enter an integer.\n");
             while(getchar() != '\n'); 
        }
    }
}

void generateRandomIntegers(int arr[], int size) {
    srand(time(NULL));
    printf("Generated Random Numbers: ");
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % (MAX_RANGE + 1);
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void generateIncreasingSequence(int arr[], int size, int start) {
    printf("Generated Increasing Sequence: ");
    for (int i = 0; i < size; i++) {
        arr[i] = start + i;
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void displaySortingMenu() {
    printf("\nSorting Menu\n");
    printf("A. Merge Sort\n");
    printf("B. Quick Sort\n");
    printf("C. Heap Sort\n");
    printf("D. Selection Sort\n");
    printf("E. Bubble Sort\n");
    printf("F. Insertion Sort\n");
    printf("Enter your choice: ");
}

void copyArray(int source[], int destination[], int size) {
    for (int i = 0; i < size; i++) {
        destination[i] = source[i];
    }
}

char sortArrayAndDisplay(int arr[], int size, char *dataType, int *originalArr, int *pTotalSortCount) {
    char sortChoice;
    displaySortingMenu();
    scanf(" %c", &sortChoice);
    while(getchar() != '\n');

    char sortingMethod[50] = "Unknown";
    clock_t start, end;
    double execTime = 0.0;

    start = clock();

    switch (sortChoice) {
        case 'A': case 'a':
            printf("\nYou chose MERGE SORT\n");
            mergeSort(arr, 0, size - 1);
            strcpy(sortingMethod, "Merge Sort");
            break;
        case 'B': case 'b':
            printf("\nYou chose QUICK SORT\n");
            quickSort(arr, 0, size - 1);
            strcpy(sortingMethod, "Quick Sort");
            break;
        case 'C': case 'c':
            printf("\nYou chose HEAP SORT\n");
            heapSort(arr, size);
            strcpy(sortingMethod, "Heap Sort");
            break;
        case 'D': case 'd':
            printf("\nYou chose SELECTION SORT\n");
            selectionSort(arr, size);
            strcpy(sortingMethod, "Selection Sort");
            break;
        case 'E': case 'e':
            printf("\nYou chose BUBBLE SORT\n");
            bubbleSort(arr, size);
            strcpy(sortingMethod, "Bubble Sort");
            break;
        case 'F': case 'f':
            printf("\nYou chose INSERTION SORT\n");
            insertionSort(arr, size);
            strcpy(sortingMethod, "Insertion Sort");
            break;
        default:
            printf("\nInvalid sort selection. No sorting performed.\n");
            return 0;
    }

    end = clock();
    execTime = ((double) (end - start)) / CLOCKS_PER_SEC;

    (*pTotalSortCount)++;

    writeToFile(arr, size, dataType, sortingMethod, execTime, originalArr, *pTotalSortCount);

    printf("\nSorted Numbers: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    printf("\nExecution Time: %.6f seconds\n", execTime);
    printf("Output saved to 'sorted_output.txt'!\n");

    return sortChoice;
}

void writeToFile(int arr[], int size, char *dataType, char *sortingMethod, double execTime, int *originalArr, int sortNumber) {

    FILE *readFile = fopen(FILE_NAME, "r");
    FILE *tempFile = fopen("temp.txt", "w");

    if (tempFile == NULL) {
        printf("Error creating temporary file!\n");
        if (readFile) fclose(readFile);
        return;
    }

    char *algorithms[MAX_ALGOS] = {
        "Merge Sort", "Quick Sort", "Heap Sort",
        "Selection Sort", "Bubble Sort", "Insertion Sort"
    };

    int algorithmIndex = -1;
    for (int i = 0; i < MAX_ALGOS; i++) {
        if (strcmp(algorithms[i], sortingMethod) == 0) {
            algorithmIndex = i;
            break;
        }
    }
    if (algorithmIndex == -1) {
        fprintf(stderr, "Error: Unknown sorting method '%s' passed to writeToFile.\n", sortingMethod);
        fclose(tempFile);
        if (readFile) fclose(readFile);
        remove("temp.txt");
        return;
    }

    int foundN = 0;
    int updatedExistingLine[MAX_ALGOS] = {0};
    char buffer[1024];
    int currentN = -1;
    int headerWritten = 0;

    while (readFile && fgets(buffer, sizeof(buffer), readFile)) {

        buffer[strcspn(buffer, "\r\n")] = 0;

        int nValueRead;
        char dataTypeRead[50] = "";
        int headerMatch = sscanf(buffer, "Performance for N = %d (%49[^)])", &nValueRead, dataTypeRead);
        if (headerMatch < 1) {
            headerMatch = sscanf(buffer, "Performance for N = %d", &nValueRead);
        }

        if (headerMatch >= 1) {
            currentN = nValueRead;
            if (currentN == size) {
                foundN = 1;
                for(int k=0; k<MAX_ALGOS; k++) updatedExistingLine[k] = 0;
                headerWritten = 1;
            } else {
                foundN = 0;
            }
            fprintf(tempFile, "%s\n", buffer);
            continue;
        }

        if (strncmp(buffer, "----------------", 16) == 0 || strncmp(buffer, "Algorithm", 9) == 0) {
             fprintf(tempFile, "%s\n", buffer);
             continue;
        }

        if (foundN) {
            int lineAlgorithmIndex = -1;
            for (int k = 0; k < MAX_ALGOS; k++) {
                 size_t algoNameLen = strlen(algorithms[k]);
                 if (strncmp(buffer, algorithms[k], algoNameLen) == 0 &&
                     (isspace((unsigned char)buffer[algoNameLen]) || buffer[algoNameLen] == '\0'))
                 {
                     lineAlgorithmIndex = k;
                     break;
                 }
            }

            if (lineAlgorithmIndex != -1 && lineAlgorithmIndex == algorithmIndex && !updatedExistingLine[algorithmIndex]) {
                double runTimes[MAX_RUNS];
                double avgTime = 0.0;
                int runCount = 0;

                for (int k = 0; k < MAX_RUNS; k++) {
                    runTimes[k] = 0.0;
                }

                sscanf(buffer, "%*s %lf %lf %lf %lf %lf",
                       &runTimes[0], &runTimes[1], &runTimes[2],
                       &runTimes[3], &runTimes[4]);

                while (runCount < MAX_RUNS && fabs(runTimes[runCount]) > 1e-9) {
                    runCount++;
                }

                if (runCount < MAX_RUNS) {
                    runTimes[runCount] = execTime;
                }

                double totalTime = 0;
                int validRuns = 0;
                for (int j = 0; j < MAX_RUNS; j++) {
                    if (fabs(runTimes[j]) > 1e-9) {
                        totalTime += runTimes[j];
                        validRuns++;
                    }
                }
                avgTime = (validRuns > 0) ? (totalTime / validRuns) : 0.0;

                fprintf(tempFile, "%-15s %-10.6f %-10.6f %-10.6f %-10.6f %-10.6f %-10.6f\n",
                        sortingMethod,
                        runTimes[0], runTimes[1], runTimes[2],
                        runTimes[3], runTimes[4], avgTime);

                updatedExistingLine[algorithmIndex] = 1;
                continue;
            }
            else if (lineAlgorithmIndex != -1) {
                 fprintf(tempFile, "%s\n", buffer);
                 if (!updatedExistingLine[lineAlgorithmIndex]) {
                    updatedExistingLine[lineAlgorithmIndex] = 1;
                 }
                 continue;
            }
        }

        fprintf(tempFile, "%s\n", buffer);

    }

    if (!foundN) {
        if (ftell(tempFile) > 0) {
             fprintf(tempFile, "\n");
        }

        fprintf(tempFile, "Performance for N = %d (%s)\n", size, dataType);
        fprintf(tempFile, "--------------------------------------------------------------------------------\n");
        fprintf(tempFile, "%-15s %-10s %-10s %-10s %-10s %-10s %-10s\n",
                "Algorithm", "Run 1", "Run 2", "Run 3", "Run 4", "Run 5", "Avg Time");
        fprintf(tempFile, "--------------------------------------------------------------------------------\n");

        for (int i = 0; i < MAX_ALGOS; i++) {
            if (i == algorithmIndex) {
                fprintf(tempFile, "%-15s %-10.6f %-10.6f %-10.6f %-10.6f %-10.6f %-10.6f\n",
                        algorithms[i], execTime, 0.0, 0.0, 0.0, 0.0, execTime);
            } else {
                fprintf(tempFile, "%-15s %-10.6f %-10.6f %-10.6f %-10.6f %-10.6f %-10.6f\n",
                        algorithms[i], 0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
            }
        }
         fprintf(tempFile, "-------------------------------------------------------------------------------\n");
    }

    if (readFile) fclose(readFile);
    fclose(tempFile);

    if (remove(FILE_NAME) != 0) {
        if (readFile != NULL) {

        }
    }
    if (rename("temp.txt", FILE_NAME) != 0) {
        perror("Error renaming temporary file");
    }
}

void merge(int arr[], int left, int middle, int right) {
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;
    int *L = malloc(n1 * sizeof(int));
    int *R = malloc(n2 * sizeof(int));
    if (L == NULL || R == NULL) { fprintf(stderr, "Malloc failed in merge\n"); if(L) free(L); if(R) free(R); return; }
    for (i = 0; i < n1; i++) L[i] = arr[left + i];
    for (j = 0; j < n2; j++) R[j] = arr[middle + 1 + j];
    i = 0; j = 0; k = left;
    while (i < n1 && j < n2) arr[k++] = (L[i] <= R[j]) ? L[i++] : R[j++];
    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
    free(L); free(R);
}

void mergeSort(int arr[], int left, int right) {
    if (left < right) {
        int middle = left + (right - left) / 2;
        mergeSort(arr, left, middle);
        mergeSort(arr, middle + 1, right);
        merge(arr, left, middle, right);
    }
}

void swap(int *a, int *b) { int temp = *a; *a = *b; *b = temp; }

int partition(int arr[], int low, int high) {
    int pivot = arr[high]; int i = (low - 1);
    for (int j = low; j <= high - 1; j++) if (arr[j] < pivot) swap(&arr[++i], &arr[j]);
    swap(&arr[i + 1], &arr[high]); return (i + 1);
}

void quickSort(int arr[], int low, int high) {
    if (low < high) { int pi = partition(arr, low, high); quickSort(arr, low, pi - 1); quickSort(arr, pi + 1, high); }
}

void heapify(int arr[], int n, int i) {
    int largest = i; int l = 2 * i + 1; int r = 2 * i + 2;
    if (l < n && arr[l] > arr[largest]) largest = l;
    if (r < n && arr[r] > arr[largest]) largest = r;
    if (largest != i) { swap(&arr[i], &arr[largest]); heapify(arr, n, largest); }
}

void heapSort(int arr[], int n) {
    for (int i = n / 2 - 1; i >= 0; i--) heapify(arr, n, i);
    for (int i = n - 1; i > 0; i--) { swap(&arr[0], &arr[i]); heapify(arr, i, 0); }
}

void selectionSort(int arr[], int size) {
    int i, j, min_idx;
    for (i = 0; i < size - 1; i++) {
        min_idx = i; for (j = i + 1; j < size; j++) if (arr[j] < arr[min_idx]) min_idx = j;
        if (min_idx != i) swap(&arr[min_idx], &arr[i]);
    }
}

void bubbleSort(int arr[], int size) {
    int i, j; int swapped;
    for (i = 0; i < size - 1; i++) {
        swapped = 0; for (j = 0; j < size - i - 1; j++) if (arr[j] > arr[j + 1]) { swap(&arr[j], &arr[j + 1]); swapped = 1; }
        if (swapped == 0) break;
    }
}

void insertionSort(int arr[], int size) {
    int i, key, j;
    for (i = 1; i < size; i++) {
        key = arr[i]; j = i - 1; while (j >= 0 && arr[j] > key) { arr[j + 1] = arr[j]; j = j - 1; } arr[j + 1] = key;
    }
}