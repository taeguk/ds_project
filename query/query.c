#include "query.h"

DataManager* import_data(const char* filename)
{
	FILE * fp = fopen(filename, "rb");
	DataManager *dataManager;
	int i;

	dataManager = (DataManager*) malloc_wrap_wrap(sizeof(DataManager));
	fread(&dataManager->dataNum, sizeof(dataManager->dataNum), 1, fp);
	fread(&dataManager->axisNum, sizeof(dataManager->axisNum), 1, fp);
	fread(&dataManager->mws, sizeof(dataManager->mws), 1, fp);

	dataManager->data = (Data*) malloc_wrap((dataManager->dataNum+1) * sizeof(Data));
	dataManager->wordIdxTable = (WordIdx*) calloc_wrap(MAX_HASH_SIZE, sizeof(WordIdx));
	dataManager->axisIdxTable = (WordIdx*) calloc_wrap(dataManager->dataNum+1, sizeof(AxisIdx));
	dataManager->ptrAxis = (Data**) calloc_wrap(dataManager->axisNum+1, sizeof(Data*));
	dataManager->data[0].wordVec = (WordVec*) calloc_wrap(dataManager->axisNum+1, sizeof(WordVec));

	fread(dataManager->axisIdxTable+1, sizeof(AxisIdx), dataManager->axisNum, fp);
	
	for(i=1; i <= dataManager->dataNum; ++i) {
		Data *curData = &dataManager->data[i];
		AxisIdx axisIdx = get_axisIdx(dataManager, i);
		curData->wordVec = (WordVec*) calloc_wrap(dataManager->axisNum+1, sizeof(WordVec));
		fread(curData->wordStr, sizeof(char), dataManager->mws, fp);
		fread(&curData->wordCnt, sizeof(WordCnt), 1, fp);
		fread(curData->wordVec+1, sizeof(WordVec), dataManager->axisNum, fp);
		register_data(dataManager, curData->wordStr, i);
		if(axisIdx != 0) {
			dataManager->ptrAxis[axisIdx] = curData;
		}
	}

	fclose(fp);
}

void register_data(DataManager* dataManager, const char* wordStr, WordIdx wordIdx)
{
	HashIdx hashIdx = hash_word(wordStr);

	while(dataManager->wordIdxTable[hashIdx] != 0) {
		Word* curData = &dataManager->word[dataManager->wordIdxTable[hashIdx]];
		if(!strcmp(curData->wordStr, wordStr)) {
			return 0;
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
		Word* curWord = &dataManager->word[dataManager->wordIdxTable[hashIdx]];
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
	for(i=0; i<=dataManager->wordNum; ++i) {
		free(dataManager->word[i].wordVec);
	}
	free(dataManager->word);
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

