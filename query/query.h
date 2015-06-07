#ifndef __QUERY_H__
#define __QUERY_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

#define MAX_WORD_SIZE	100
#define MAX_HASH_SIZE	(500000 * 20)
//#define MAX_HASH_SIZE	(6000000 * 20)

typedef	unsigned int WordCnt;
typedef double WordVec;
typedef unsigned int HashIdx;
typedef unsigned int WordIdx;
typedef unsigned int AxisIdx;

/*
 * Function Name	: 
 * Parameter		: 
 * Return value		: 
 * Role				: 
 *		1. 
 *			- 
 */

typedef struct _Data {
	char wordStr[MAX_WORD_SIZE];
	WordCnt wordCnt;
	WordVec* wordVec;
} Data;	

typedef struct _DataManager {
	Data *data;
	int dataNum;
	int axisNum;
	WordIdx *wordIdxTable;
	AxisIdx *axisIdxTable;
	Data* *ptrAxis;	// array of pointers of axis word data.
	int mws;		// MAX_WORD_SIZE
} DataManager;

/*
 * Function Name    : menu
 * Parameter        : dataManager
 * Return value     : void
 * Role             :
 *      1. get standard input and transfer to chosen menu
 */
void menu(DataManager* dataManager);

/*
 * Function Name    : print_info_menu
 * Parameter        : dataManager
 * Return value     : void
 * Role             :
 *      1. print information summary of the input file
 *          - number of different words, number of axis words
 */
void print_info_menu(DataManager* dataManager);

/*
 * Function Name    : print_axis_menu
 * Parameter        : dataManager
 * Return value     : void
 * Role             :
 *      1. print information of the axises
 *          - number, word string, word count
 */
void print_axis_menu(DataManager* dataManager);

/*
 * Function Name    : view_specific_word_menu
 * Parameter        : dataManager
 * Return value     : void
 * Role             :
 *      1. get user word input
 *      2. print information of the input word.
 *          - word index, word string, word count, vector components
 */
void view_specific_word_menu(DataManager* dataManager);

/*
 * Function Name    : dot_product
 * Parameter        : dataManager, two words' WordIdx
 * Return value     : result of dot product between two word vectors
 * Role             :
 *      1. calculate dot product between two word vectors and return
 */
double dot_product(DataManager* dataManager, WordIdx wordIdx1, WordIdx wordIdx2);

/*
 * Function Name    : distance_between_words_menu
 * Parameter        : dataManager
 * Return value     : void
 * Role             :
 *      1. input two words from user
 *      2. print cos value of angular distance between two word vectors
 */
void distance_between_words_menu(DataManager* dataManager);

/*
 * Function Name    : get_related_words_menu
 * Parameter        : dataManager
 * Return value     : void
 * Role             :
 *      1. input word from user
 *      2. print most highly related words with inputted word.
 */
void get_related_words_menu(DataManager* dataManager);


/*
 * Function Name	: import_data
 * Parameter		: a pointer of file name
 * Return value		: a pointer of data manager
 * Role				: 
 *		1. import data from file. 
 *			- create data manager.
 */
DataManager* import_data(const char* filename);

/*
 * Function Name	: register_data
 * Parameter		: a pointer of data manager, a pointer of word string, wordIdx
 * Return value		: void
 * Role				: 
 *		1. get hash value of wordStr
 *			- using hash_word and collision_hash
 *		2. update data manager's wordIdxTable
 */
void register_data(DataManager* dataManager, const char* wordStr, WordIdx wordIdx);

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
 * Function Name	: collision_hash
 * Parameter		: hashIdx, a pointer of word string 
 * Return value		: next hashIdx
 * Role				: 
 *		1. find next hash value when hash collision occurs
 */
inline HashIdx collision_hash(HashIdx hashIdx, const char* WordStr);

/*
 * Function Name    : get_axisIdx
 * Parameter        : a pointer of data manager, wordIdx
 * Return value     : axisIdx of the word if word of wordIdx is axis, 0 if not
 * Role             :
 *      1. get and return axisIdx of the word from parameter wordIdx
 */
inline AxisIdx get_axisIdx(const DataManager *dataManager, WordIdx wordIdx);

/*
 * Function Name    : get_wordIdx
 * Parameter        : a pointer of data manager, a pointer of word string
 * Return value     : wordIdx of the string (return 0 if the word not exist)
 * Role             :
 *      1. call hash_word to get hash value of input string.
 *      2. get and return wordIdx of it by wordIdxTable array if collision does not appear.
 *      3. if there is collision, call collision_hash and get new wordIdx to return.
 */
WordIdx get_wordIdx(const DataManager *dataManager, const char *wordStr);

/*
 * Function Name	: free_dataManager
 * Parameter		: a pointer of data manager
 * Return value		: void
 * Role				: 
 *		1. free data manager deeply.
 */
void free_dataManager(DataManager *dataManager);

/*
 * This is wrapping functions of malloc, calloc, realloc.
 * Check memory allocation failure.
 */
void* malloc_wrap(size_t size);
void* calloc_wrap(size_t num, size_t size);
void* realloc_wrap(void* ptr, size_t size);
 
int getch(void);
inline void flush_stdin(void);


#endif
