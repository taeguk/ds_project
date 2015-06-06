#include "calcvec.h"

void calculate_vector(WordManager *wordManager, char *fileName)
{
        WordIdx rqueue[RELATION_QUEUE_SIZE];
        char wordStr[MAX_WORD_SIZE];
        FILE *fp;
        SepType sepType;
        WordIdx wordIdx;
        int idx;

        fp = fopen(fileName, "r");
        init_rqueue(rqueue);
        for( ; ; ) {
                read_word_from_file(fp, wordStr);
                sepType = get_separator_type(wordStr);
                if(sepType) { //sepType != NOT_SEP, if separator
                        process_separator(wordManager, rqueue, &idx, sepType);
                } else { //sepType == NOT_SEP, if not separator
                        wordIdx = get_wordIdx(wordStr);
                        if(!wordIdx) { continue; }
                        process_relation(wordManager, rqueue, &idx, wordIdx);
                }
                if(sepType == SEP_EOF) break;
        }
        fclose(fp);
        normalize_vector(wordManager);
}

HashIdx hash_word(const char* wordStr) {
    HashIdx hash = 2729;
        char *str = wordStr;
    int c;
    while(c = *str++)
        hash = (hash * 585) + c;
    return hash % MAX_HASH_SIZE;
}

void normalize_vector(WordManager *wordManager)
{
        int i, j;
        double size;
        for(i = 1; i <= (wordManager->wordNum); i++) { //for all vectors
                //get size
                size = 0;
                for(j = 1; j <= (wordManager->axisNum); j++) { //for all components
                        size += (wordManager->word[i].wordVec[j]) * (wordManager->word[i].wordVec[j]);
                }

                //normalize
                if(!size) continue;
                size = sqrt(size);
                for(j = 1; j <= (wordManager->axisNum); j++) { //for all components
                        (wordManager->word[i].wordVec[j]) /= size;
                }
        }
}

void process_relation(WordManager *wordManager, WordIdx *rqueue, int *pIdx, WordIdx wordIdx)
{
        AxisIdx axisIdx;
        insert_word_to_rqueue(rqueue, wordIdx, pIdx);
        if(axisIdx = get_axisIdx(wordManager, rqueue[rqueue_mid_idx(*pIdx)])) {
                update_vector(wordManager, rqueue, axisIdx);
        }
}

void update_vector(WordManager *wordManager, WordIdx *rqueue, AxisIdx axisIdx)
{
        int i;
        for(i = 0; i < RELATION_QUEUE_SIZE; i++) { (wordManager->word[rqueue[i]].wordVec[axisIdx])++; }
}

void insert_word_to_rqueue(WordIdx *rqueue, WordIdx wordIdx, int *pIdx)
{
        rqueue[++(*pIdx)] = wordIdx;
}

void read_word_from_file(FILE *fp, char *wordStr)
{
        int i;
        char ch;

        //get word from file to wordStr
        for(i = 0; i < MAX_WORD_SIZE; i++) {
                ch = fgetc(fp);
                if(i == 0 && ch == ' ') {
                        while((ch = fgetc(fp)) != ' ');
                }
                if(ch == EOF) {
                        wordStr[0] = 0;
                        wordStr[1] = SEP_EOF;
                        return;
                }
                if(ch == ' ') {
                        wordStr[i] = 0;
                        return;
                }
                wordStr[i] = ch;
        }

        //more than MAX_WORD_SIZE, return error separator.
        while((ch = fgetc(fp)) != ' ' && ch != EOF);
        if(ch == EOF) {
                wordStr[0] = 0;
                wordStr[1] = SEP_EOF;
                return;
        } else {
                wordStr[0] = 0;
                wordStr[1] = SEP_OVER;
        }
}

inline AxisIdx get_axisIdx(WordManager *wordManager, WordIdx wordIdx)
{
        return wordManager->axisIdxTable[wordIdx];
}

WordIdx get_wordIdx(WordManager *wordManager, const char *wordStr)
{
        HashIdx hashIdx = hash_word(wordStr);

        while(wordManager->wordIdxTable[hashIdx] != 0) {
                Word* curWord = &wordManager->word[wordManger->wordIdxTable[hashIdx]];
                if(!strcmp(curWord->wordStr, wordStr)) {
                        return wordManger->wordIdxTable[hashIdx];
                }
                hashIdx = collision_hash(hashIdx, wordStr);
        }

        return 0;
}

void init_rqueue(WordIdx *rqueue)
{
        int i;
        for(i = 0; i < RELATION_QUEUE_SIZE; i++) { rqueue[i] = 0; }
}

inline int next_rqueue_idx(int idx)
{
        return ++idx % RELATION_QUEUE_SIZE;
}

inline int rqueue_mid_idx(int idx)
{
        return (++idx + RELATION_QUEUE_SIZE/2) % RELATION_QUEUE_SIZE;
}

inline bool check_separator(char *wordStr)
{
        return (get_separator_type(wordStr)) ? true : false;
}

inline SepType get_separator_type(char *wordStr)
{
        return (wordStr[0]) ? NOT_SEP : wordStr[1];
}

void process_separator(WordManager *wordManager, WordIdx *rqueue, int *pIdx, SepType sepType)
{
        int i;
        //put (size of rqueue / 2) of junk word into rqueue
        if(sepType == SEP_TEXT || sepType == SEP_EOF) {
                for(i = 0; i < RELATION_QUEUE_SIZE / 2; i++) {
                        process_relation(wordManager, rqueue, pIdx, 0);
                }
        }
}
