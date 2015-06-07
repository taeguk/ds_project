#include "kuery.h"

void menu(DataManager* dataManager)
{
    char ch;
    int endFlag = 0;
    do {
        printf("\n"
               "\t\tMenu\n"
               "\t1. Print information of input data file.\n"
               "\t2. Print information of axis words.\n"
               "\t3. View information of specific word.\n"
               "\n"
               "\tpress q to exit.");
        ch = getchar();
        if(ch == '1') {
            print_info_menu(dataManager);
        }
        if(ch == '2') {
            print_axis_menu(dataManager);
        }
        if(ch == '3') {
            view_specific_word_menu(dataManager);
        }
        if(ch == 'q' || ch == 'Q') {
            endFlag = 1;
        }
    } while(!endFlag);
}

void print_info_menu(DataManager* dataManager)
{
    printf("\n"
           "\t\tInformation of input data file.\n\n"
           "\tNumber of different words : %d\n"
           "\tNumber of axis words : %d\n",
           dataManager->dataNum, dataManager->axisNum);
}

void print_axis_menu(DataManager* dataManager)
{
    int i;
    printf("\n"
           "\t\tInformation of axis words.\n");
    for(i = 1; i <= dataManager->axisNum; i++) {
        printf("\n"
               "\tNo. %d\n"
               "\tWord : %s\n"
               "\tCount : %u\n",
               i, dataManager->ptrAxis[i]->wordStr, dataManager->ptrAxis[i]->wordCnt);
    }
}

void view_specific_word_menu(DataManager* dataManager)
{
    int i, j, maxPrint;
    char inputBuffer[MAX_WORD_SIZE];
    WordIdx wordIdx;
    printf("\n"
           "\t\tInformation of specific word\n"
           "\tInput word : ");
    scanf("%s", inputBuffer);
    wordIdx = get_wordIdx(dataManager, inputBuffer);
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    printf("\n"
           "\twordIdx. %u\n"
           "\tWord : %s\n"
           "\tCount : %u\n",
           "\tVector components : \n"
           wordIdx, dataManager->data[wordIdx].wordStr, dataManager->data[wordIdx].wordCnt);
    maxPrint = w.ws_col / (10 + 3);
    for(i = 1, j = maxPrint; i <= dataManager->axisNum; i++) {
        printf("%.10lf ", dataManager->data[wordIdx].wordVec[i]);
        if(!j--) {
            j = maxPrint;
            printf("\n");
        }
    }
    if(j != maxPrint) {
        printf("\n");
    }
}
