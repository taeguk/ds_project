#ifndef __CALCVEC_H__
#define __CALCVEC_H__

#include <stdbool.h>
#include <math.h>
#include <stdio.h>
#include "analysis.h"

/*
 * Function Name    : calculate_vector
 * Parameter        : wordManager, fileName
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
void calculate_vector(WordManager *wordManager, char *fileName);

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
 * Parameter        : wordManager
 * Return value     : void
 * Role             :
 *      1. calculate size of the vectors
 *      2. divide the vectors by its size
 */
void normalize_vector(WordManager *wordManager);

/*
 * Function Name    : process_relation
 * Parameter        : wordManager, rqueue, pIdx, wordIdx
 * Return value     : void
 * Role             :
 *      1. call insert_word_to_rqueue
 *      2. check the word at the middle of the relation queue is axis or not.
 *      3. if the word is axis, call update_vector
 */
void process_relation(WordManager *wordManager, WordIdx *rqueue, int *pIdx, WordIdx wordIdx);

/*
 * Function Name    : update_vector
 * Parameter        : wordManager, rqueue, axisIdx
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
 * Parameter        : wordManager, wordIdx
 * Return value     : axisIdx of the word if word of wordIdx is axis, 0 if not
 * Role             :
 *      1. get and return axisIdx of the word from parameter wordIdx
 */
inline AxisIdx get_axisIdx(WordManager *wordManager, WordIdx wordIdx);

/*
 * Function Name    : get_wordIdx
 * Parameter        : wordManager, a pointer of word string
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
 * Parameter        : wordManager, rqueue, a pointer of rear idx of rqueue, type of seperator
 * Return value     : void
 * Role             :
 *      1. process particular operations to relation queue and vector, depending on separator Type.
 */
void process_separator(WordManager *wordManager, WordIdx *rqueue, int *pIdx, SepType sepType);

#endif
