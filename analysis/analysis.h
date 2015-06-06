#ifndef __ANALYSIS_H__
#define __ANALYSIS_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define SIMULATION_NUM	1000

#define MAX_WORD_SIZE	100

#define MAX_HASH_SIZE	(MAX_WORD_NUM * 20)
#define MAX_WORD_NUM	500000
#define AXIS_NUM		200

#define RELATION_QUEUE_SIZE (10 * 2 + 1)

typedef	unsigned int WordCnt;
typedef double WordVec;
typedef unsigned int HashIdx;
typedef unsigned int WordIdx;
typedef unsigned int AxisIdx;

//typedef char SepType; //separator type
typedef enum {
	NOT_SEP = 0,
	SEP_EOF,
	SEP_TEXT,
	SEP_OVER
} SepType;

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
	char wordStr[MAX_WORD_SIZE];
	WordCnt wordCnt;
	WordVec* wordVec;	// wordVec[0] is junk.
} Word;

/*
 * ---  struct description  ---
 *	Struct for managing words
 * --- elements description ---
 * 1. word
 *		- word 
 *		- word[0] is junk
 * 2. wordNum
 *		- the number of words
 *		- not include junk word(word[0]).
 * 3. axisNum
 *		- the number of axis words
 * 4. wordIdxTable
 *		- the value is 0 if index hash is not registered.
 *		- the value is wordIdx if it's registerd.
 *		- wordIdxTable[hashIdx] : wordIdx
 * 5. axisIdxTable
 *		- the value is 0 if index word is not an axis,
 *		- axisIdx if it's an axis.
 *		- axisIdxTable[wordIdx] : axisIdx
 */

typedef struct _WordManager {
	Word *word;		// word[0] is junk.
	int wordNum;
	int axisNum;
	WordIdx *wordIdxTable;
	AxisIdx *axisIdxTable;		// axisIdxTable[0] is junk.
} WordManager;

/*
 * Function Name	: 
 * Parameter		: 
 * Return value		: 
 * Role				: 
 *		1. 
 *			- 
 */

/*
 * This is wrapping functions of malloc, calloc, realloc.
 * Check memory allocation failure.
 */
void* malloc_wrap(size_t size);
void* calloc_wrap(size_t num, size_t size);
void* realloc_wrap(void* ptr, size_t size);

/*
 * Function Name	: free_wordManager
 * Parameter		: a pointer of word manager
 * Return value		: void
 * Role				: 
 *		1. free word manager deeply.
 */
void free_wordManager(WordManager *wordManager);

/*
 * Function Name	: init_analysis
 * Parameter		: a pointer of input file name string
 * Return value		: a pointer of word manager
 * Role				: 
 *		1. get words' wordCnt.
 *			- count the number of the words
 *		2. create wordIdxTable.
 *		3. create axisIdxTable.
 *			- select proper axis words
 */
WordManager* init_analysis(const char* filename);

/*
 * Function Name	: create_axisIdxTable 
 * Parameter		: a pointer of word manager, a pointer of input file name string
 * Return value		: void
 * Role				: 
 *		1. select sample words randomly in word manager
 *			- and create sample manager using that.
 *		2. calculate vector of samples
 *		3. select axis words in samples.
 *			- using vector to itself
 */
void create_axisIdxTable(WordManager *wordManager, const char* filename);

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
 * Parameter		: a pointer of word manager, a pointer of the word string
 * Return value		: 0 if exist, hashIdx which will inserted if not exist
 * Role				: 
 *		1. get hashIdx of the word using hash_word.
 *			- hash(word) = hashIdx
 *		2. check word's existence using wordIdxTable.
 */
HashIdx check_word_existence(const WordManager* wordManager, const char* wordStr);

/*
 * Function Name	: register_word
 * Parameter		: a pointer of word manager, a pointer of the word string
 * Return value		: true if success, false if fail
 * Role				: 
 *		1. get hashIdx of the word using hash_word.
 *			- hash(word) = hashIdx
 *		2. check word's existence using wordIdxTable.
 *		3. if word exists, increase word's wordCnt and return false.
 *		4. else, insert word to wordManager, increase wordNum, register new wordIdx to wordIdxTable and return true.
 */
bool register_word(WordManager* wordManager, const char* wordStr);

/*
 * Function Name    : calculate_vector
 * Parameter        : a pointer of word manager, a pointer of input file name string
 * Return value     : void
 * Role             :
 *      1. initialize relation queue.
 *      2. read word from file
 *      3-1. if word is separator
 *          a. get type of separator
 *          b. process the separator
 *      3-2. else
 *          a. get wordIdx
 *          b. process relation
 *      4. normalize the vectors
 */
void calculate_vector(WordManager *wordManager, const char *fileName);

/*
 * Function Name    : hash_word
 * Parameter        : a pointer of the word string
 * Return value     : hashIdx of the word
 * Role             :
 *      1. get hashIdx of the word through hashing the word.
 *          - hash(word) = HashIdx
 */
HashIdx hash_word(const char* wordStr);

/*
 * Function Name    : normalize_vector
 * Parameter        : a pointer of word manager
 * Return value     : void
 * Role             :
 *      1. calculate size of the vectors
 *      2. divide the vectors by its size
 */
void normalize_vector(WordManager *wordManager);

/*
 * Function Name    : process_relation
 * Parameter        : a pointer of word manager, rqueue, a pointer of rear idx of rqueue, wordIdx
 * Return value     : void
 * Role             :
 *      1. call insert_word_to_rqueue
 *      2. check the word at the middle of the relation queue is axis or not.
 *      3. if the word is axis, call update_vector
 */
void process_relation(WordManager *wordManager, WordIdx *rqueue, int *pIdx, WordIdx wordIdx);

/*
 * Function Name    : update_vector
 * Parameter        : a pointer of word manager, rqueue, axisIdx
 * Return value     : void
 * Role             :
 *      1. update vector of word that which is in the relation queue.
 *          - increase vector element of axisIdx axis.
 */
void update_vector(WordManager *wordManager, WordIdx *rqueue, AxisIdx axisIdx);

/*
 * Function Name    : insert_word_to_rqueue
 * Parameter        : rqueue, wordIdx, a pointer of rear idx of rqueue
 * Return value     : void
 * Role             :
 *      1. insert word to relation queue at pIdx, by WordIdx value.
 *          - rqueue[pIdx] = wordIdx
 *      2. change pIdx value to next index.
 */
void insert_word_to_rqueue(WordIdx *rqueue, WordIdx wordIdx, int *pIdx);

/*
 * Function Name    : read_word_from_file
 * Parameter        : a file pointer, a pointer of word string
 * Return value     : void
 * Role             :
 *      1. read word from input text file. (token is whitespace)
 *      2. put string to wordStr.
 */
void read_word_from_file(FILE *fp, char *wordStr);

/*
 * Function Name    : get_axisIdx
 * Parameter        : a pointer of word manager, wordIdx
 * Return value     : axisIdx of the word if word of wordIdx is axis, 0 if not
 * Role             :
 *      1. get and return axisIdx of the word from parameter wordIdx
 */
inline AxisIdx get_axisIdx(WordManager *wordManager, WordIdx wordIdx);

/*
 * Function Name    : get_wordIdx
 * Parameter        : a pointer of word manager, a pointer of word string
 * Return value     : wordIdx of the string (return 0 if the word not exist)
 * Role             :
 *      1. call hash_word to get hash value of input string.
 *      2. get and return wordIdx of it by wordIdxTable array if collision does not appear.
 *      3. if there is collision, call collision_hash and get new wordIdx to return.
 */
WordIdx get_wordIdx(WordManager *wordManager, const char *wordStr);

/*
 * Function Name    : init_rqueue
 * Parameter        : rqueue
 * Return value     : void
 * Role             :
 *      1. initialize relation queue.
 *          - put junk word(something like /dev/null) to all elements of rqueue
 */
void init_rqueue(WordIdx *rqueue);

/*
 * Function Name    : next_rqueue_idx
 * Parameter        : rear idx of rqueue
 * Return value     : next rear idx of rqueue
 * Role             :
 *      1. return next index of relation queue
 */
inline int next_rqueue_idx(int idx);

/*
 * Function Name    : rqueue_mid_idx
 * Parameter        : rear idx of rqueue
 * Return value     : middle index of relation queue
 * Role             :
 *      1. return the middle index of relation queue
 */
inline int rqueue_mid_idx(int idx);

/*
 * Function Name    : check_separator
 * Parameter        : a pointer of word string
 * Return value     : true if word is seperator, false if not
 * Role             :
 *      1. check the parameter string wordStr is a separator or not.
 */
inline bool check_separator(char *wordStr);

/*
 * Function Name    : get_separator_type
 * Parameter        : a pointer of word string
 * Return value     : type of separator
 * Role             :
 *      1. return the type of separator wordStr.
 */
inline SepType get_separator_type(char *wordStr);

/*
 * Function Name    : process_separator
 * Parameter        : a pointer of word manager, rqueue, a pointer of rear idx of rqueue, type of seperator
 * Return value     : void
 * Role             :
 *      1. process particular operations to relation queue and vector, depending on separator Type.
 */
void process_separator(WordManager *wordManager, WordIdx *rqueue, int *pIdx, SepType sepType);

/*
 * Function Name	: collision_hash
 * Parameter		: hashIdx, a pointer of word string 
 * Return value		: next hashIdx
 * Role				: 
 *		1. 
 *			- 
 */
inline HashIdx collision_hash(HashIdx hashIdx, const char* WordStr);

/*
 * Function Name	: export_result
 * Parameter		: a pointer of word manager, a pointer of output file name string
 * Return value		: void
 * Role				: 
 *		1. export words, its' vector values and etc,...
 */
void export_result(const WordManager *wordManager, const char* filename);

#endif
