#ifndef __QUERY_H__
#define __QUERY_H__

#define MAX_WORD_SIZE	100

#define MAX_HASH_SIZE	(500000 * 20)

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
	Data* *ptrAxis;
	int mws;
} DataManager;

/*
 * Function Name	: import_data
 * Parameter		: a pointer of data manager, a pointer of file name
 * Return value		: void
 * Role				: 
 *		1. import data from file. 
 *			- create data manager.
 */
void import_data(DataManager* dataManager, const char* filename);


#endif
