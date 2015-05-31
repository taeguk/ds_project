#include "analysis.c"

#define FILE_NAME "input.txt"

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
	FILE * fp = fopen(FILE_NAME, "r");

	//wordManager->word[0] <- junk
	wordManager->wordNum = 1;

	// get words' wordCnt and create wordIdxTable.
	while(1) {
		Word* curWord = &word[wordManager->wordNum];
		read_word_from_file(fp, curWord->word);

		if(!check_separator(curWord->word)) {
			register_word(wordManager, curWord->word);
		} else if(get_separator_type(curWord->word) == SEP_EOF) {
			break;
		}
	}

	fclose(fp);

	// create axisIdxTable.
	//get_axisList(wordManager->word+1, wordManager->wordNum-1, wordManager->axisIdxTable+1);
	WordManager tmp = {wordManager->word+1, wordManager->wordNum-1, wordManager->axisIdxTable+1};
	get_axisIdxTable(&tmp);
}

//void get_axisList(const Word *words, int wordNum, AxisIdx *axisCheck)
void get_axisIdxTable(WordManager* wordManager)
{
	int i;

	// recursive exit condition
	if(wordManager->wordNum <= AXIS_NUM) {
		for(i=0; i<wordManager->wordNum; ++i) {
			wordManager[axisIdxTable = 1;
		}
		*axisNum = wordNum;
		return;
	}

	static const int SPLIT_NUM = 10;
	int offset = wordNum / SPLIT_NUM;

	// divide problem
	for(i=0; i<SPLIT_NUM-1; ++i) {
		get_axisList(words+i*offset, offset, axisCheck+i*offset, axisNum);
	}
	get_axisList(words+i*offset, wordNum - (offset*(SPLIT_NUM-1)), axisCheck+i*offset, axisNum);

	// merge result
	
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
		wordManager->wordIdxTable[hashIdx] = wordManager->wordNum;
		++wordManager->wordNum;
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
