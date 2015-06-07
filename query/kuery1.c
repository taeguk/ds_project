
double dot_product (DataManager* dataManager, WordIdx wordIdx1, Wordidx wordIdx2)
{
    int i;
    double result = 0;
    for(i = 1; i <= dataManager->axisNum; i++) {
        result += dataManager->data[wordIdx1].wordVec[i] * dataManager->data[wordIdx2].wordVec[i];
    }
    return result;
}

void distance_between_words_menu(DataManager* dataManager)
{
    char inputBuffer1[MAX_WORD_SIZE], inputBuffer2[MAX_WORD_SIZE];
    WordIdx wordIdx1, wordIdx2;

    printf("\n"
           "\t\tDistance between two words.\n"
           "\tInput word1 : ");
    scanf("%s", inputBuffer1);
    flush_stdin();
    printf("\tInput word2 : ");
    scanf("%s", inputBuffer2);
    flush_stdin();

    wordIdx1 = get_wordIdx(dataManager, inputBuffer1);
    wordIdx2 = get_wordIdx(dataManager, inputBuffer2);

    //if(wordIdx1 == 0 || wordIdx2 == 0) {
    //  return;
    //}
    printf("distance : %.6lf\n", dot_product(dataManager, wordIdx1, wordIdx2));
}

void get_close_words_menu(DataManager* dataManager)
{
    int printNum = 10;
    int i, j, k;
    Data* *pHighlyRelatedWords;
    double *highlyRelatedVal;
    double nowVal;
    WordIdx wordIdx;
    char inputBuffer[MAX_WORD_SIZE];

    printf("\n"
           "\t\tPrint highly related words list.\n"
           "\tInput word : ");
    scanf("%s", inputBuffer);
    flush_stdin();
    //printf("\tInput number of list : ");
    //scanf("%d", &printNum);

    pHighlyRelatedWords = (Data **)malloc(sizeof(Data *) * printNum);
    highlyRelatedVal = (double *)malloc(sizeof(double) * printNum);

    for(i = 0; i < printNum; i++) {
        pHighlyRelatedWords = 0;
        highlyRelatedVal = 0;
    }

    for(i = 1; i <= dataManager->dataNum; i++) {
        if(i == wordIdx) continue;
        nowVal = dot_product(dataManager, wordIdx, i);
        for(j = 0; j < printNum; j++) {
            if(highlyRelatedVal[j] < nowVal) break;
        }
        if(j == printNum) continue;
        for(k = printNum - 2; k >= j; k--) {
            pHighlyRelatedWords[k + 1] = pHighlyRelatedWords[k];
            highlyRelatedVal[k + 1] = highlyRelatedVal[k];
        }
        pHighlyRelatedWords[j] = dataManager->data[i];
        highlyRelatedVal[j] = nowVal;
    }

    printf("        word        | cos of angular distance\n");
    for(i = 0; i < printNum; i++) {
        printf("%-20s |        %.6lf\n", pHighlyRelatedWords[i]->wordStr, highlyRelatedVal[i]);
    }

    free(pHighlyRelatedWords);
    free(highlyRelatedVal);
}
