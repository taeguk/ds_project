#include "analysis.h"

int main(int argc, char *argv[])
{
	WordManager* wordManager;

	if(argc != 3) {
		fprintf(stderr, "[Usage] %s <INPUT_FILE> <OUTPUT_FILE>\n", argv[0]);
		return 1;
	}

	fprintf(stdout, "[*] initializing for analysis...\n");
	wordManager = init_analysis(argv[1]);

	fprintf(stdout, "[*] calculating vector...\n");
	calculate_vector(wordManager, argv[1]);

	fprintf(stdout, "[*] exporting result...\n");
	export_result(wordManager, argv[2]);

	free_wordManager(wordManager);

	return 0;
}

WordManager* init_analysis(const char* filename)
{
	FILE * fp = fopen(filename, "r");
	WordManager *wordManager;
	int i,j;

	// create word manager.	
	fprintf(stdout, "\t[*] creating word manager...\n");
	wordManager = (WordManager*) malloc_wrap(sizeof(WordManager));
	wordManager->word = (Word*) malloc_wrap(sizeof(Word) * (MAX_WORD_NUM+1));
	wordManager->wordNum = 0;
	wordManager->wordIdxTable = (WordIdx*) calloc_wrap(MAX_HASH_SIZE, sizeof(WordIdx));
	
	// get words' wordCnt and create wordIdxTable.
	fprintf(stdout, "\t[*] get words' wordCnt and create wordIdxTable...\n");
	while(1) {
		Word* curWord = &wordManager->word[wordManager->wordNum+1];
		read_word_from_file(fp, curWord->wordStr);

		if(!check_separator(curWord->wordStr)) {
			register_word(wordManager, curWord->wordStr);
		} else if(get_separator_type(curWord->wordStr) == SEP_EOF) {
			break;
		}
	}
	fprintf(stdout, "\t\t[*] the number of words = %d\n", wordManager->wordNum);

	fclose(fp);

	// create axisIdxTable.
	fprintf(stdout, "\t[*] creating axisIdxTable...\n");
	create_axisIdxTable(wordManager, filename);

	return wordManager;
}

void create_axisIdxTable(WordManager *wordManager, const char* filename)
{
	WordIdx *sortedWordIdx;
	WordManager *sampleManager;
	WordIdx sampleWordIdx[SIMULATION_NUM+1];

	int sampleNum, gap;
	int i,j;

	/* 
	 * 1. select sample words randomly in sorted words 
	 *	  and create sampleManager.
	 */
	fprintf(stdout, "\t[*] creating axisIdxTable... [stage 1]\n");
	// sort all words, but using data reference moving for effectiveness.
	// using insertion sort.
	fprintf(stdout, "\t\t[*] sort all words\n");
	sortedWordIdx = (WordIdx*) malloc_wrap(sizeof(WordIdx) * (wordManager->wordNum+1));
	for(i=1; i <= wordManager->wordNum; ++i) {
		int wi = sortedWordIdx[i] = i;
		for(j=i-1; j >= 1 && wordManager->word[sortedWordIdx[j]].wordCnt > wordManager->word[wi].wordCnt; --j)
			sortedWordIdx[j+1] = sortedWordIdx[j];
		sortedWordIdx[j+1] = wi;
	}

	// calculate a index gap.
	gap = wordManager->wordNum / SIMULATION_NUM;
	if(gap == 0) {
		gap = 1;
		sampleNum = wordManager->wordNum;
	} else {
		sampleNum = SIMULATION_NUM;
	}

	// create sample manager.
	fprintf(stdout, "\t\t[*] create sampleManager\n");
	sampleManager = (WordManager*) malloc_wrap(sizeof(WordManager));
	sampleManager->word = (Word*) calloc_wrap(sampleNum+1, sizeof(Word));
	sampleManager->wordNum = sampleManager->axisNum = sampleNum;
	sampleManager->wordIdxTable = (WordIdx*) calloc_wrap(MAX_HASH_SIZE, sizeof(WordIdx));
	sampleManager->axisIdxTable = (AxisIdx*) calloc_wrap(sampleNum+1, sizeof(AxisIdx));
	sampleManager->word[0].wordVec = (WordVec*) calloc_wrap(sampleNum+1, sizeof(WordVec));

	// select sample words randomly.
	fprintf(stdout, "\t\t[*] select sample words ramdomly\n");
	for(i=1,j=gap; i <= sampleManager->wordNum; ++i,j+=gap) {
		HashIdx hashIdx;
		sampleManager->word[i] = wordManager->word[sortedWordIdx[j]];
		sampleManager->word[i].wordVec = (WordVec*) calloc_wrap(sampleManager->axisNum+1, sizeof(WordVec));
		hashIdx = check_word_existence(sampleManager, sampleManager->word[i].wordStr);
		sampleManager->wordIdxTable[hashIdx] = i;
		sampleManager->axisIdxTable[i] = i;
	}

	/*
	 * 2. calculate vector of samples.
	 */
	fprintf(stdout, "\t[*] creating axisIdxTable... [stage 2]\n");
	calculate_vector(sampleManager, filename);

	/*
	 * 3. select axis words in samples.
	 */
	fprintf(stdout, "\t[*] creating axisIdxTable... [stage 3]\n");	
	// sort sample words in vector to itself desc order.
	fprintf(stdout, "\t\t[*] sort sample words\n");
	for(i=1; i <= sampleManager->wordNum; ++i) {
		WordIdx wi = sampleWordIdx[i] = i;
		for(j=i-1; j >= 1 && sampleManager->word[sampleWordIdx[j]].wordVec[sampleWordIdx[j]] < sampleManager->word[wi].wordVec[wi]; --j)
			sampleWordIdx[j+1] = sampleWordIdx[j];
		sampleWordIdx[j+1] = wi;
	}

	// create word manager's axisIdxTable.
	fprintf(stdout, "\t\t[*] create word manager's axisIdxTable\n");
	wordManager->axisNum = (sampleManager->wordNum < AXIS_NUM ? sampleManager->wordNum : AXIS_NUM);
	wordManager->axisIdxTable = (AxisIdx*) calloc_wrap(wordManager->wordNum, sizeof(AxisIdx));
	for(i=1; i <= wordManager->axisNum; ++i) {
		WordIdx wordIdx = get_wordIdx(wordManager, sampleManager->word[sampleWordIdx[i]].wordStr);
		wordManager->axisIdxTable[wordIdx] = i;
	}

	wordManager->word[0].wordVec = (WordVec*) calloc_wrap(wordManager->axisNum+1, sizeof(WordVec));
	for(i=1; i <= wordManager->wordNum; ++i) {
		wordManager->word[i].wordVec = (WordVec*) calloc_wrap(wordManager->axisNum+1, sizeof(WordVec));
	}

	free_wordManager(sampleManager);
}

HashIdx check_word_existence(const WordManager* wordManager, const char* wordStr)
{
	HashIdx hashIdx = hash_word(wordStr);

	while(wordManager->wordIdxTable[hashIdx] != 0) {
		Word* curWord = &wordManager->word[wordManager->wordIdxTable[hashIdx]];
		if(!strcmp(curWord->wordStr, wordStr)) {
			return 0;
		}
		hashIdx = collision_hash(hashIdx, wordStr);
	}
	return hashIdx;
}

bool register_word(WordManager *wordManager, const char* wordStr)
{
	HashIdx hashIdx;

	if((hashIdx=check_word_existence(wordManager, wordStr)) == 0) {
		++wordManager->word[wordManager->wordNum].wordCnt;
		return false;
	} else {
		wordManager->wordIdxTable[hashIdx] = ++wordManager->wordNum;
		return true;
	}
}


HashIdx hash_word(const char* wordStr)
{
	HashIdx hash = 2729;
	const char *str = wordStr;
    int c;
    while(c = *str++)
        hash = (hash * 585) + c;
    return hash % MAX_HASH_SIZE;
}

inline HashIdx collision_hash(HashIdx hashIdx, const char* WordStr)
{
	return (hashIdx + 1) % MAX_HASH_SIZE;
}

inline AxisIdx get_axisIdx(const WordManager *wordManager, WordIdx wordIdx)
{
	return wordManager->axisIdxTable[wordIdx];
}

WordIdx get_wordIdx(const WordManager *wordManager, const char *wordStr)
{
	HashIdx hashIdx = hash_word(wordStr);

	while(wordManager->wordIdxTable[hashIdx] != 0) {
		Word* curWord = &wordManager->word[wordManager->wordIdxTable[hashIdx]];
		if(!strcmp(curWord->wordStr, wordStr)) {
			return wordManager->wordIdxTable[hashIdx];
		}
		hashIdx = collision_hash(hashIdx, wordStr);
	}

	return 0;
}

void calculate_vector(WordManager *wordManager, const char *fileName)
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
			wordIdx = get_wordIdx(wordManager, wordStr);
			if(!wordIdx) { continue; }
			process_relation(wordManager, rqueue, &idx, wordIdx);
		}
		if(sepType == SEP_EOF) break;
	}
	fclose(fp);
	normalize_vector(wordManager);
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

void insert_word_to_rqueue(WordIdx *rqueue, WordIdx wordIdx, int *pIdx)
{
	*pIdx = next_rqueue_idx(*pIdx);
	rqueue[*pIdx] = wordIdx;
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

void read_word_from_file(FILE *fp, char *wordStr)
{
	int i;
	char ch;

	//get word from file to wordStr
	for(i = 0; i < MAX_WORD_SIZE; i++) {
		ch = fgetc(fp);
		if(i == 0 && (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')) {
			//while((ch = fgetc(fp)) != ' ');
			continue;
		}
		if(ch == EOF) {
			wordStr[0] = 0;
			wordStr[1] = SEP_EOF;
			return;
		}
		if(ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n') {
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

void export_result(const WordManager *wordManager, const char* filename)
{
	FILE * fp = fopen(filename, "wb");
	int mws = MAX_WORD_SIZE;
	int i;

	// export a number of words
	fwrite(&wordManager->wordNum, sizeof(wordManager->wordNum), 1, fp);
	// export a number of axis
	fwrite(&wordManager->axisNum, sizeof(wordManager->axisNum), 1, fp);
	// export MAX_WORD_SIZE
	fwrite(&mws, sizeof(mws), 1, fp);
	
	// export axisIdxTable
	fwrite(wordManager->axisIdxTable+1, sizeof(AxisIdx), wordManager->axisNum, fp);
	
	// export words
	for(i=1; i <= wordManager->wordNum; ++i) {
		Word *curWord = &wordManager->word[i];
		fwrite(curWord->wordStr, sizeof(char), MAX_WORD_SIZE, fp);
		fwrite(&curWord->wordCnt, sizeof(WordCnt), 1, fp);
		fwrite(curWord->wordVec+1, sizeof(WordVec), wordManager->axisNum, fp);
	}

	// not export wordIdxTable because it is depending on hash functions.

	fclose(fp);
}

void free_wordManager(WordManager *wordManager)
{
	int i;
	for(i=0; i<=wordManager->wordNum; ++i) {
		free(wordManager->word[i].wordVec);
	}
	free(wordManager->word);
	free(wordManager->wordIdxTable);
	free(wordManager->axisIdxTable);
	free(wordManager);
}

void* malloc_wrap(size_t size)
{
	void* ret = malloc(size);
	if(!ret) {
		fprintf(stderr, "[Error] cannot malloc. check memory please.\n");
		exit(1);
	}
	return ret;
}

void* calloc_wrap(size_t num, size_t size)
{
	void* ret = calloc(num, size);
	if(!ret) {
		fprintf(stderr, "[Error] cannot calloc. check memory please.\n");
		exit(1);
	}
	return ret;
}

void* realloc_wrap(void* ptr, size_t size)
{
	void* ret = realloc(ptr, size);
	if(!ret) {
		fprintf(stderr, "[Error] cannot realloc. check memory please.\n");
		exit(1);
	}
	return ret;
}
