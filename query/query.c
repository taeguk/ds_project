#include "query.h"

int main(int argc, char *argv[])
{
	DataManager* dataManager;

	if(argc != 2) {
		fprintf(stderr, "[Usage] %s <INPUT_FILE> \n", argv[0]);
		return 1;
	}

	fprintf(stdout, "[*] import data...\n");
	dataManager = import_data(argv[1]);

	menu(dataManager);

	free_dataManager(dataManager);

	return 0;
}

void menu(DataManager* dataManager)
{
    char ch;
    int endFlag = 0;
    do {
		system("clear");
		printf("\t************** ds_project **************\n"
			   "\t\t                    - kwon & koo\n"
				);
        printf("\n"
               //"\t Menu\n\n"
               "\t1. Print information of input data file.\n"
               "\t2. Print information of axis words.\n"
               "\t3. View information of specific word.\n"
               "\n"
               "\tpress q to exit. ");
        ch = getch();
        if(ch == '1') {
			system("clear");
            print_info_menu(dataManager);
        }
        if(ch == '2') {
			system("clear");
            print_axis_menu(dataManager);
        }
        if(ch == '3') {
			system("clear");
            view_specific_word_menu(dataManager);
        }
        if(ch == 'q' || ch == 'Q') {
            endFlag = 1;
        }
    } while(!endFlag);
	puts("");
	system("clear");
}

void print_info_menu(DataManager* dataManager)
{
    printf("\n"
           "\t\tInformation of input data file\n\n"
           "\tNumber of different words : %d\n"
           "\tNumber of axis words : %d\n",
           dataManager->dataNum, dataManager->axisNum);
	printf("\n\t[*] Print information of input data file finish! ");
	getch();
	puts("");
}

void print_axis_menu(DataManager* dataManager)
{
	struct winsize w;
	char ch;
	bool flag = true;
    int i,j;
    printf("\n"
           "\t\tInformation of axis words\n");
    for(i = 1,j = 1; i <= dataManager->axisNum; i++) {
        printf("\n"
               "\tNo. %d\n"
               "\tWord : %s\n"
               "\tCount : %u\n"
			   "\tVector to itself : %.10f\n",
               i, dataManager->ptrAxis[i]->wordStr, dataManager->ptrAxis[i]->wordCnt,
			   dataManager->ptrAxis[i]->wordVec[i]);
		if(i >= j) {
			ch = getch();
			if(ch == ' ') {
				system("clear");
				ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);	
				j = i + w.ws_row / 5;
			} else if(ch == 'q') {
				flag = false;
				break;
			}
		}
    }
	if(flag)
		printf("\n\t[*] Print information of axis words finish! ");
	else
		printf("\n\t[!] Print information of axis words stoped! ");
	getch();
	puts("");
}

void view_specific_word_menu(DataManager* dataManager)
{
	struct winsize w;
    int i, j, maxPrint;
    char inputBuffer[MAX_WORD_SIZE];
    WordIdx wordIdx;
    printf("\n"
           "\t\tInformation of specific word\n\n"
           "\tInput word : ");
    scanf("%s", inputBuffer);
	flush_stdin();
    wordIdx = get_wordIdx(dataManager, inputBuffer);
	if(wordIdx == 0) {
		printf("\n\t[!] %s does not exist! ", inputBuffer);
		getch();
		return;
	}
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);	
    maxPrint = w.ws_col / (10 + 3) - 1;
	if(maxPrint < 1) maxPrint = 1;
    printf("\n"
           "\twordIdx. %u\n"
           "\tWord : %s\n"
           "\tCount : %u\n"
           "\tVector components : \n",
           wordIdx, dataManager->data[wordIdx].wordStr, dataManager->data[wordIdx].wordCnt);
	printf("%8c",' ');
	for(i = 0; i < maxPrint; i++)
		printf("%12d ", i);
	puts("");
    for(i = 1, j = 0; i <= dataManager->axisNum; i++,j=(j+1)%maxPrint) {
		if(j == 0) {
			printf("\n%5d   ", i);
		}
        printf("%.10lf ", dataManager->data[wordIdx].wordVec[i]);
    }
    printf("\n\n");
	printf("\t[*] Print information of specific word finish! ");
	getch();
	puts("");
}

DataManager* import_data(const char* filename)
{
	FILE * fp = fopen(filename, "rb");
	DataManager *dataManager;
	int i;

	dataManager = (DataManager*) malloc_wrap(sizeof(DataManager));
	fread(&dataManager->dataNum, sizeof(dataManager->dataNum), 1, fp);
	fread(&dataManager->axisNum, sizeof(dataManager->axisNum), 1, fp);
	fread(&dataManager->mws, sizeof(dataManager->mws), 1, fp);

	if(dataManager->mws > MAX_WORD_SIZE) {
		fprintf(stderr, "[Error] MAX_WORD_SIZE(%d) must be greater than %d\n", MAX_WORD_SIZE, dataManager->mws);
		fclose(fp);
		free(dataManager);
		exit(1);
	}

	dataManager->data = (Data*) malloc_wrap((dataManager->dataNum+1) * sizeof(Data));
	dataManager->wordIdxTable = (WordIdx*) calloc_wrap(MAX_HASH_SIZE, sizeof(WordIdx));
	dataManager->axisIdxTable = (WordIdx*) calloc_wrap(dataManager->dataNum+1, sizeof(AxisIdx));
	dataManager->ptrAxis = (Data**) calloc_wrap(dataManager->axisNum+1, sizeof(Data*));
	dataManager->data[0].wordVec = (WordVec*) calloc_wrap(dataManager->axisNum+1, sizeof(WordVec));

	fread(dataManager->axisIdxTable+1, sizeof(AxisIdx), dataManager->dataNum, fp);
/*
	for(i=1; i <= dataManager->dataNum; ++i) {
		if(dataManager->axisIdxTable[i] != 0) {
			fprintf(stdout, "wordIdx %u : axisIdx %u\n", i, dataManager->axisIdxTable[i]);
		}
	}

	exit(1);
*/	
	for(i=1; i <= dataManager->dataNum; ++i) {
		Data *curData = &dataManager->data[i];
		AxisIdx axisIdx = get_axisIdx(dataManager, i);
		curData->wordVec = (WordVec*) calloc_wrap(dataManager->axisNum+1, sizeof(WordVec));
		fread(curData->wordStr, sizeof(char), dataManager->mws, fp);
		fread(&curData->wordCnt, sizeof(WordCnt), 1, fp);
		fread(curData->wordVec+1, sizeof(WordVec), dataManager->axisNum, fp);
		register_data(dataManager, curData->wordStr, i);
		if(axisIdx != 0) {
			//fprintf(stdout, "ptrAxis[%u]'s wordIdx = %d\n", axisIdx, i);
			dataManager->ptrAxis[axisIdx] = curData;
		}
	}

	fclose(fp);

	return dataManager;
}

void register_data(DataManager* dataManager, const char* wordStr, WordIdx wordIdx)
{
	HashIdx hashIdx = hash_word(wordStr);

	while(dataManager->wordIdxTable[hashIdx] != 0) {
		Data* curData = &dataManager->data[dataManager->wordIdxTable[hashIdx]];
		if(!strcmp(curData->wordStr, wordStr)) {
			break;
		}
		hashIdx = collision_hash(hashIdx, wordStr);
	}

	dataManager->wordIdxTable[hashIdx] = wordIdx;
}

HashIdx hash_word(const char* wordStr)
{
	HashIdx hash = 2729;
	const char *str = wordStr;
    int c;
    while(c = *str++)
        hash = (hash * 585) + c;
    return hash % MAX_HASH_SIZE;
}

inline HashIdx collision_hash(HashIdx hashIdx, const char* WordStr)
{
	return (hashIdx + 1) % MAX_HASH_SIZE;
}

inline AxisIdx get_axisIdx(const DataManager *dataManager, WordIdx wordIdx)
{
	return dataManager->axisIdxTable[wordIdx];
}

WordIdx get_wordIdx(const DataManager *dataManager, const char *wordStr)
{
	HashIdx hashIdx = hash_word(wordStr);

	while(dataManager->wordIdxTable[hashIdx] != 0) {
		Data* curWord = &dataManager->data[dataManager->wordIdxTable[hashIdx]];
		if(!strcmp(curWord->wordStr, wordStr)) {
			return dataManager->wordIdxTable[hashIdx];
		}
		hashIdx = collision_hash(hashIdx, wordStr);
	}

	return 0;
}

void free_dataManager(DataManager *dataManager)
{
	int i;
	for(i=0; i<=dataManager->dataNum; ++i) {
		free(dataManager->data[i].wordVec);
	}
	free(dataManager->data);
	free(dataManager->wordIdxTable);
	free(dataManager->axisIdxTable);
	free(dataManager->ptrAxis);
	free(dataManager);
}

void* malloc_wrap(size_t size)
{
	void* ret = malloc(size);
	if(!ret) {
		fprintf(stderr, "[Error] cannot malloc. check memory please.\n");
		exit(1);
	}
	return ret;
}

void* calloc_wrap(size_t num, size_t size)
{
	void* ret = calloc(num, size);
	if(!ret) {
		fprintf(stderr, "[Error] cannot calloc. check memory please.\n");
		exit(1);
	}
	return ret;
}

void* realloc_wrap(void* ptr, size_t size)
{
	void* ret = realloc(ptr, size);
	if(!ret) {
		fprintf(stderr, "[Error] cannot realloc. check memory please.\n");
		exit(1);
	}
	return ret;
}

int getch(void)
{
	struct termios oldt, newt;
	int ch;
	tcgetattr( STDIN_FILENO, &oldt );
	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );
	tcsetattr( STDIN_FILENO, TCSANOW, &newt );
	ch = getchar();
	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );
	return ch;
}

inline void flush_stdin(void)
{
	int c;
	while((c = getchar()) != '\n' && c != EOF);
}
