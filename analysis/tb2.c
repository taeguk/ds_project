#include "analysis.c"

#define INPUT_FILE "input.txt"
#define OUTPUT_FILE "output.txt"

프리 해주는거 만들자.

/*
 * Function Name	: init_analysis
 * Parameter		: wordManager
 * Return value		: void
 * Role				: 
 *		1. get words' wordCnt.
 *			- count the number of the words
 *		2. create wordIdxTable.
 *		3. create axisIdxTable.
 *			- select proper axis words
 * Using function	:
 *		1. 
 * Description		:
 *		none
 */
void init_analysis(WordManager* wordManager)
{
	FILE * fp = fopen(INPUT_FILE, "r");
	int i,j;

	//wordManager->word[0] <- junk
	wordManager->wordNum = 0;

	// get words' wordCnt and create wordIdxTable.
	while(1) {
		Word* curWord = wordManager->word[wordManager->wordNum];
		read_word_from_file(fp, curWord->word);

		if(!check_separator(curWord->word)) {
			register_word(wordManager, curWord->word);
		} else if(get_separator_type(curWord->word) == SEP_EOF) {
			break;
		}
	}

	fclose(fp);

	// create axisIdxTable.
	create_axisIdxTable(wordManager);
}

void create_axisIdxTable(WordManager *wordManager)
{
	Word sampleWord[SAMPLE_NUM+1];
	int sampleWordIdx[SAMPLE_NUM+1];
	int sampleCnt = 0;

	WordManager simulManager;
	double rate[SIMULATION_NUM+1];
	WordIdx simulWordIdx[SIMULATION_NUM+1];
	int axisNum;

	int i,j;

	// select sample word 
	if(wordManager->wordNum <= SAMPLE_NUM) {
		memcpy(sampleWord, wordManager->word, sizeof(Word) * (wordManager->wordNum+1));
		sampelCnt = wordManager->wordNum;
	} else {
		for(i=1; sampelCnt < SAMPLE_NUM; ++i) {
			int wordIdx;
			if(wordIdx = wordManager->wordIdxTable[i]) {
				sampleWord[++sampleCnt] = wordManager->word[wordIdx];
			}
		}
	}
	// insertion sort ascending order
	for(i=1; i <= sampleCnt; ++i) {
		int wi = sampleWordIdx[i] = i;
		for(j=i-1; j >= 1 && sampleWord[sampleWordIdx[j]].wordCnt > sampleWord[wi].wordCnt; --j)
			sampleWordIdx[j+1] = sampleWordIdx[j];
		sampleWordIdx[j+1] = wi;
	}
/*	
	for(i=1; i <= sampleCnt; ++i) {
		Word word = sampleWord[i];
		for(j=i-1; j >= 1 && sampleWord[j].wordCnt > word.wordCnt; --j)
			sampleWord[j+1] = sampleWord[j];
		sampleWord[j+1] = word;
	}
*/	
	if(sampleCnt * FILTER_RATE - 11 <= SIMULATION_NUM) {
		simulManager.word = sampleWord;
		simulManager.wordNum = simulManager.wordNum;
		simulManager.wordIdx = wordIdxTable;
		simulManager.axisIdxTable = (AxisIdx*) calloc(simulManager.wordNum+1, sizeof(AxisIdx));
		for(i=1; i <= simulManager.wordNum; ++i) {
			simulManager.axisIdxTable[i] = i;
		}
	} else {
		simulManager.word = (Word*) calloc(SIMULATION_NUM+1, sizeof(Word));
		simulManager.wordNum = SIMULATION_NUM;
		simulManager.axisNum = 
		simulManager.wordIdxTable = (WordIdx*) calloc(MAX_HASH_SIZE, sizeof(WordIdx));
		simulManager.axisIdxTable = (AxisIdx*) calloc(SIMULATION_NUM+1, sizeof(AxisIdx));
		for(i=1,j=sampleCnt*FILTER_RATE; i <= SIMULATION_NUM; ++i,--j) {
			simulManager.word[i] = sampleWord[sampleWordIdx[j]];
			HashIdx hashIdx = check_word_existence(&simulManager, simulManager.word[i].wordStr);
			simulManager.wordIdxTable[hashIdx] = i;
			simulManager.axisIdxTable[i] = i;
		}
	}
	
	calculate_vector(simulManager);
	
	for(i=1; i <= simulManager.wordNum; ++i) {
		rate[i] = 0;
		simulWordIdx[i] = i;
		for(j=1; j <= simulManager.wordNum; ++j) {
			rate[i] += (double) simulManager.word[i].wordVec[j];
		}
		rate[i] = simulManager.word[i].wordVec[i] / rate[i];
	}

	// sort rate desc order. and get high 200rank i.
	// high 200 rank is axis.
	for(i=1; i <= simulManager.wordNum; ++i) {
		double r = rate[i];
		WordIdx wi = simulWordIdx[i];
		for(j=i-1; j >= 1 && rate[j] < r; --j) {
			rate[j+1] = rate[j];
			simulWordIdx[j+1] = simulWordIdx[j];
		}
		rate[j+1] = r;
		simulWordIdx[j+1] = wi;
	}

	axisNum = (simulManager.wordNum < AXIS_NUM ? simulManager.wordNum : AXIS_NUM);
	for(i=1; i <= axisNum; ++i) {
		WordIdx wordIdx = get_wordIdx(wordManager, simulManager.word[simulWordIdx[i]].wordStr);
		wordManager->axisIdxTable[i] = wordIdx;
	}

	free(simulManager);
}

/*
 * Function Name	: register_word
 * Parameter		: wordManager, a pointer of the word string
 * Return value		: true if success, false if fail
 * Role				: 
 *		1. get hashIdx of the word using hash_word.
 *			- hash(word) = hashIdx
 *		2. check word's existence using wordIdxTable.
 *		3. if word exists, increase word's wordCnt and return false.
 *		4. else, insert word to wordManager, increase wordNum, register new wordIdx to wordIdxTable and return true.
 * Using function	:
 *		1. 
 * Description		:
 *		none
 */
bool register_word(WordManager* wordManager, const char* wordStr)
{
	HashIdx hashIdx;

	if((hashIdx=check_word_existence(WordManager, wordStr)) == 0) {
		++wordManager->word[wordManager->wordNum]->wordCnt;
		return false;
	} else {
		wordManager->wordIdxTable[hashIdx] = ++wordManager->wordNum;
		return true;
	}
}

/*
 * Function Name	: check_word_existence
 * Parameter		: wordManager, a pointer of the word string
 * Return value		: 0 if exist, hashIdx which will inserted if not exist
 * Role				: 
 *		1. get hashIdx of the word using hash_word.
 *			- hash(word) = hashIdx
 *		2. check word's existence using wordIdxTable.
 * Using function	:
 *		1. hash_word
 * Description		:
 *		none
 */
HashIdx check_word_existence(const WordManager* wordManager, const char* wordStr)
{
	HashIdx hashIdx = hash_word(wordStr);

	while(WordManager->wordIdxTable[hashIdx] != 0) {
		Word* curWord = &wordManager->word[wordManager->wordIdxTable[hashIdx]];
		if(!strcmp(curWord->wordStr, wordStr)) {
			return 0;
		}
		hashIdx = collision_hash(hashIdx, wordStr);
	}
	return hashIdx;
}

void export_result(WordManager *wordManager)
{
	FILE * fp = fopen(OUTPUT_FILE, "wb");
	AxisIdx axisNum = (wordManager->wordNum < AXIS_NUM ? wordManager->wordNum : AXIS_NUM);
	int mhs = MAX_HASH_SIZE;
	int i;

	// export a number of words
	fwrite(&wordManager->wordNum, sizeof(wordManager->wordNum), 1, fp);
	// export a number of axis
	fwrite(&axisNum, sizeof(axisNum), 1, fp);
	// export MAX_HASH_SIZE
	fwrite(&mhs, sizeof(mhs), 1, fp);

	// export words
	fwrite(wordManager->word, sizeof(Word), wordManager->wordNum + 1, fp);

	// export wordIdxTable
	fwrite(wordManager->wordIdxTable, sizeof(WordIdx), mhs, fp);

	// export axisIdxTable
	fwrite(wordManager->axisIdxTable, sizeof(AxisIdx), axisNum+1, fp);

	fclose(fp);
}

void free_wordManager(WordManager *wordManager)
{
	free(wordManager->word);
	free(wordManager->wordIdxTable);
	free(wordManager->axisIdxTable);
	free(wordManager);
}
