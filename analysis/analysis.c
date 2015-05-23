/*
 * array wordCnt shows that the number of the word, represented by the index, appeared times in whole input text.
 * wordCnt[wordIdx] : count of a word
 */
WordCnt wordCnt[MAX_WORD_NUM];

/*
 * relation index of two words between normal word and axis word.
 * if there is no relation between word and axis, the value is 0.
 * wordVec[wordIdx][axisIdx] : axisIdx'th element of the wordIdx vector
 */
WordVec wordVec[MAX_WORD_NUM][AXIS_NUM];



Word word[MAX_WORD_NUM];
WordIdx wordIdxTable[MAX_HASH_SIZE];

/* 
 * the value is -1 if index word is not an axis,
 * axisIdx if it's an axis.
 * axisIdx[wordIdx] : axisIdx
 */
AxisIdx axisIdx[MAX_WORD_NUM];

/***********************************/
