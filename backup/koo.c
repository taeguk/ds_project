#include "analysis.c"

void calculate_vector(WordManager *wordManager)
{
	WordIdx rqueue[RELATION_QUEUE_SIZE];
	char wordStr[MAX_WORD_SIZE];
	FILE *fp;
	SepType sepType;
	WordIdx wordIdx;
	int idx;

	fp = fopen(FILE_NAME, "r");
	init_rqueue(rqueue);
	for( ; ; ) {
		read_word_from_file(fp, wordStr);
		sepType = get_separator_type(wordStr);
		if(sepType) { //sepType != NOT_SEP, if separator
			process_separator(wordManager, rqueue, &idx, sepType);
		} else { //sepType == NOT_SEP, if not separator
			wordIdx = get_wordIdx(wordStr);
			process_relation(wordManager, rqueue, &idx, wordIdx);
		}
		if(sepType == SEP_EOF) break;
	}
	fclose(fp);
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
	for(i = 0; i < MAX_WORD_SIZE; i++) { wordStr[i] = 0; }
	for(i = 0; i < MAX_WORD_SIZE; i++) {
		ch = fgetc(fp);
		if(ch == EOF) {
			wordStr[0] = 0;
			wordStr[1] = SEP_EOF;
			return;
		}
		if(ch == ' ') { return; }
		wordStr[i] = ch;
	}
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
	//나중에
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
	if(sepType == SEP_TEXT || sepType == SEP_EOF) {
		for(i = 0; i < RELATION_QUEUE_SIZE / 2; i++) {
			process_relation(wordManager, rqueue, pIdx, 0);
		}
	}
}
