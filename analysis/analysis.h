#ifndef __ANALYSIS_H__
#define __ANALYSIS_H__

#include <stdbool.h>

#define MAX_HASH_SIZE	(MAX_WORD_NUM * 20)
#define MAX_WORD_NUM	500000
#define AXIS_NUM		200

#define RELATION_QUEUE_SIZE (10 * 2 + 1)

typedef	int WordCnt;
typedef int WordVec;
typedef int HashIdx;
typedef int WordIdx;
typedef int AxisIdx;

typedef char SepType; //separator type

/*
 * ---  struct description  ---
 *	Struct for storing word's information
 * --- elements description ---
 * 1. wordStr
 *		- string of the word 
 * 2. wordCnt
 *		- wordCnt shows that the number of the word appeared times in whole input text.
 * 3. wordVec
 *		- relation between the word and axis words. If there is no relation between word and axis, the value of wordVec[axisIdx] is 0.
 */
typedef struct _Word {
	const char *wordStr;
	WordCnt wordCnt;
	WordVec wordVec[AXIS_NUM];
} Word;

/*
 * ---  struct description  ---
 *	Struct for managing words
 * --- elements description ---
 * 1. word
 *		- word array
 * 2. wordNum
 *		- the number of words
 * 3. wordIdxTable
 *		- the value is -1 if index hash is not registered.
 *		- wordIdx if it's registerd.
 *		- wordIdxTable[hashIdx] : wordIdx
 * 4. axisIdxTable
 *		- the value is -1 if index word is not an axis,
 *		- axisIdx if it's an axis.
 *		- axisIdxTable[wordIdx] : axisIdx
 */
typedef struct _WordManager {
	Word word[MAX_WORD_NUM];
	int wordNum;
	WordIdx wordIdxTable[MAX_HASH_SIZE];
	AxisIdx axisIdxTable[MAX_WORD_NUM];
} WordManager;

/*
 * Function Name	: 
 * Parameter		: 
 * Return value		: 
 * Role				: 
 *		1. 
 *			- 
 * Using function	:
 *		1. 
 * Description		:
 *		none
 */

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
void init_analysis(WordManager* wordManager);

/*
 * Function Name	: hash_word
 * Parameter		: a pointer of the word string
 * Return value		: hashIdx of the word
 * Role				: 
 *		1. get hashIdx of the word through hashing the word.
 *			- hash(word) = HashIdx
 * Using function	:
 *		1. 
 * Description		:
 *		none
 */
HashIdx hash_word(const char* wordStr);

/*
 * Function Name	: check_word_existence
 * Parameter		: wordManager, a pointer of the word string
 * Return value		: true if exist, false if not exist
 * Role				: 
 *		1. get hashIdx of the word using hash_word.
 *			- hash(word) = hashIdx
 *		2. check word's existence using wordIdxTable.
 * Using function	:
 *		1. hash_word
 * Description		:
 *		none
 */
inline bool check_word_existence(const WordManager* wordManager, const char* wordStr);

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
bool register_word(WordManager* wordManager, const char* wordStr);


/***********************************************/


/*
 * Function Name	: calculate_vector
 * Parameter		: wordManager 
 * Return value		: void
 * Role				: 
 *		1. initialize relation queue.
 *		2. read word from file
 *		3-1. if word is separator
 *			a. get type of separator
 *			b. process the separator
 *		3-2. else
 *			a. get wordIdx
 *			b. process relation
 *				
 * Using function	:
 *		1. 
 * Description		:
 *		none
 */
void calculate_vector(WordManager *wordManager);

/*
 * Function Name	: process_separator
 * Parameter		: rqueue, wordIdx
 * Return value		: void
 * Role				: 
 *		1. call insert_word_to_rqueue
 *		2. check the word at the middle of the relation queue is axis or not.
 *		3. if the word is axis, call update_vector
 * Using function	:
 *		1. 
 * Description		:
 *		none
 */
void process_relation(WordIdx *rqueue, WordIdx wordIdx);

/*
 * Function Name	: update_vector
 * Parameter		: rqueue, axisIdx
 * Return value		: void
 * Role				: 
 *		1. update vector of word that which is in the relation queue.
 *			- increase vector element of axisIdx axis.
 * Using function	:
 *		1. 
 * Description		:
 *		none
 */
void update_vector(WordIdx *rqueue, AxisIdx axisIdx);

/*
 * Function Name	: insert_word_to_rqueue
 * Parameter		: rqueue, wordIdx, a pointer of rear idx of rqueue
 * Return value		: void
 * Role				: 
 *		1. insert word to relation queue at pIdx, by WordIdx value.
 *			- rqueue[pIdx] = wordIdx
 *		2. change pIdx value to next index.
 * Using function	:
 *		1. 
 * Description		:
 *		none
 */
void insert_word_to_rqueue(WordIdx *rqueue, WordIdx wordIdx, int *pIdx);

/*
 * Function Name	: read_word_from_file 
 * Parameter		: a file pointer, a pointer of word string
 * Return value		: void
 * Role				: 
 *		1. read word from input text file. (token is whitespace)
 *		2. put string to wordStr.
 * Using function	:
 *		1. 
 * Description		:
 *		none
 */
void read_word_from_file(FILE *fp, char *wordStr);

/*
 * Function Name	: check_axis
 * Parameter		: wordIdx
 * Return value		: true if word is axis, false if not
 * Role				: 
 *		1. check parameter wordIdx is an axis or not.
 * Using function	:
 *		1. 
 * Description		:
 *		none
 */
bool check_axis(WordIdx wordIdx);

/*
 * Function Name	: get_wordIdx
 * Parameter		: a 
 * Return value		: 
 * Role				: 
 *		1. call hash_word to get hash value of input string.
 *		2. get and return wordIdx of it by wordIdxTable array.
 * Using function	:
 *		1. 
 * Description		:
 *		none
 */
// 이 함수에서 할 일이 더 많을듯.
// wordStr바탕으로 해쉬값구한뒤 해쉬콜리전을 고려해야함으로 strcmp()로 단어문자열이랑 계속 비교하면서 맞는거 나올때까지 탐색해야함.
inline WordIdx get_wordIdx(const char *wordStr);

/*
 * Function Name	: init_rqueue
 * Parameter		: rqueue
 * Return value		: void
 * Role				: 
 *		1. initialize relation queue.
 *			- put junk word(something like /dev/null) to all elements of rqueue
 * Using function	:
 *		1. 
 * Description		:
 *		none
 */
void init_rqueue(WordIdx *rqueue);

/*
 * Function Name	: next_rqueue_idx
 * Parameter		: rear idx of rqueue
 * Return value		: next rear idx of rqueue
 * Role				: 
 *		1. return next index of relation queue 
 * Using function	:
 *		1. 
 * Description		:
 *		none
 */
inline int next_rqueue_idx(int idx);

/*
 * Function Name	: rqueue_mid_idx
 * Parameter		: rear idx of rqueue
 * Return value		: middle index of relation queue
 * Role				: 
 *		1. return the middle index of relation queue
 * Using function	:
 *		1. 
 * Description		:
 *		none
 */
inline int rqueue_mid_idx(int idx);

/*
 * Function Name	: check_separator
 * Parameter		: a pointer of word string
 * Return value		: true if word is seperator, false if not
 * Role				: 
 *		1. check the parameter string wordStr is a separator or not.
 * Using function	:
 *		1. 
 * Description		:
 *		none
 */
bool check_separator(char *wordStr);

/*
 * Function Name	: get_separator_type
 * Parameter		: a pointer of word string
 * Return value		: type of separator
 * Role				: 
 *		1. return the type of separator wordStr.
 * Using function	:
 *		1. 
 * Description		:
 *		none
 */
SepType get_separator_type(char *wordStr);

/*
 * Function Name	: process_separator
 * Parameter		: rqueue, a pointer of rear idx of rqueue, type of seperator
 * Return value		: void
 * Role				: 
 *		1. process particular operations to relation queue and vector, depending on separator Type.
 * Using function	:
 *		1. 
 * Description		:
 *		none
 */
void process_separator(WordIdx *rqueue, int *pIdx, SepType sepType);


/*
 * Function Name	: export_result
 * Parameter		: wordManager
 * Return value		: void
 * Role				: 
 *		1. export tables and words to file
 * Using function	:
 *		1. 
 * Description		:
 *		none
 */
void export_result(WordManager *wordManager);

#endif
