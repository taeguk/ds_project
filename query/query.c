#include "query.h"

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


