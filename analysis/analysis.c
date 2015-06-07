#include "analysis.h"

int main(int argc, char *argv[])
{
	WordManager* wordManager;
	time_t wStart, wEnd, start, end;

	if(argc != 3) {
		fprintf(stderr, "[Usage] %s <INPUT_FILE> <OUTPUT_FILE>\n", argv[0]);
		return 1;
	}

	wStart = clock();

	fprintf(stdout, "[*] initializing for analysis...\n");
	start = clock();
	wordManager = init_analysis(argv[1]);
	end = clock();
	fprintf(stdout, "[T] initializing : %lf sec\n\n", (double)(end-start)/CLOCKS_PER_SEC);

	fprintf(stdout, "[*] calculating vector...\n");
	start = clock();
	calculate_vector(wordManager, argv[1]);
	end = clock();
	fprintf(stdout, "[T] calculating vector : %lf sec\n\n", (double)(end-start)/CLOCKS_PER_SEC);

	fprintf(stdout, "[*] exporting result...\n");
	start = clock();
	export_result(wordManager, argv[2]);
	end = clock();
	fprintf(stdout, "[T] exporting result : %lf sec\n\n", (double)(end-start)/CLOCKS_PER_SEC);

	wEnd = clock();
	fprintf(stdout, "[T] all progress : %lf sec\n\n", (double)(wEnd-wStart)/CLOCKS_PER_SEC);

	free_wordManager(wordManager);

	return 0;
}

WordManager* init_analysis(const char* filename)
{
	FILE * fp = fopen(filename, "r");
	WordManager *wordManager;
	time_t start, end;
	int i,j;

	// create word manager.	
	start = clock();
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
	fprintf(stdout, "\t\t[I] the number of words = %d\n", wordManager->wordNum);
	fclose(fp);
	end = clock();
	fprintf(stdout, "\t[T] counting words and creating wordIdxTable : %lf sec\n", (double)(end-start)/CLOCKS_PER_SEC);

	// create axisIdxTable.
	start = clock();
	fprintf(stdout, "\t[*] selecting axis words and creating axisIdxTable...\n");
	create_axisIdxTable(wordManager, filename);
	end = clock();
	fprintf(stdout, "\t[T] selecting axis words and creating axisIdxTable : %lf sec\n", (double)(end-start)/CLOCKS_PER_SEC);

	return wordManager;
}

void create_axisIdxTable(WordManager *wordManager, const char* filename)
{
	WordIdx *sortedWordIdx, *heap;
	WordManager *sampleManager;
	WordIdx sampleWordIdx[SIMULATION_NUM+1];
	WordVec *rate;
	time_t start, end;

	int sampleNum, gap;
	int i,j,p,c,tmp;

	/* 
	 * 1. select sample words in sorted words 
	 *	  and create sampleManager.
	 */
	fprintf(stdout, "\t\t[*] creating sample manager... [stage 1]\n");
	
	// sort all words, but using data reference moving for effectiveness.
	// using heap sort.
	start = clock();
	fprintf(stdout, "\t\t\t[*] sorting all words...\n");
	sortedWordIdx = (WordIdx*) malloc_wrap(sizeof(WordIdx) * (wordManager->wordNum+1));
	heap = sortedWordIdx;

	// construct min-heap.
	for(i=1; i <= wordManager->wordNum; ++i) {
		c = i;
		p = c / 2;
		while(p != 0 && wordManager->word[i].wordCnt < wordManager->word[heap[p]].wordCnt) {
			heap[c] = heap[p];
			c = p;
			p /= 2;
		}
		heap[c] = i;
	}

	// sort desc.
	for(i=wordManager->wordNum; i > 1; --i) {
		tmp = heap[i];
		heap[i] = heap[1];
		heap[1] = tmp;
		tmp = heap[1];
		p = 1;
		if(2*p+1 >= i) {
			c = 2*p;
		} else {
			c = (wordManager->word[heap[2*p]].wordCnt < wordManager->word[heap[2*p+1]].wordCnt ? (2*p) : (2*p+1));
		}
		while(c < i && wordManager->word[tmp].wordCnt > wordManager->word[heap[c]].wordCnt) {
			heap[p] = heap[c];
			p = c;
			if(2*p+1 >= i) {
				c = 2*p;
			} else {
				c = (wordManager->word[heap[2*p]].wordCnt < wordManager->word[heap[2*p+1]].wordCnt ? (2*p) : (2*p+1));
			}
		}
		heap[p] = tmp;
	}

	end = clock();
	fprintf(stdout, "\t\t\t[T] sorting all words : %lf sec\n", (double)(end-start)/CLOCKS_PER_SEC);
	
	// calculate a index gap.
/*	
	gap = (wordManager->wordNum/2) / SIMULATION_NUM;
	if(gap == 0) {
		gap = 1;
		sampleNum = wordManager->wordNum;
	} else {
		sampleNum = SIMULATION_NUM;
	}
*/
	if(wordManager->wordNum/16 < SIMULATION_NUM) {
		sampleNum = wordManager->wordNum/16;
		if(sampleNum == 0) sampleNum = 1;
	} else {
		sampleNum = SIMULATION_NUM;
	}
	fprintf(stdout, "\t\t\t[I] the number of samples = %d\n", sampleNum);
	
	// create sample manager.
	sampleManager = (WordManager*) malloc_wrap(sizeof(WordManager));
	sampleManager->word = (Word*) calloc_wrap(sampleNum+1, sizeof(Word));
	sampleManager->wordNum = sampleManager->axisNum = sampleNum;
	sampleManager->wordIdxTable = (WordIdx*) calloc_wrap(MAX_HASH_SIZE, sizeof(WordIdx));
	sampleManager->axisIdxTable = (AxisIdx*) calloc_wrap(sampleNum+1, sizeof(AxisIdx));
	sampleManager->word[0].wordVec = (WordVec*) malloc_wrap((sampleNum+1) * sizeof(WordVec));
	sampleManager->ptrAxis = (Word**) calloc_wrap(sampleNum+1, sizeof(Word*));

	// select sample words.
	fprintf(stdout, "\t\t\t[*] select sample words in sorted words\n");
	for(i=1,j=wordManager->wordNum * 0.005; i <= sampleManager->wordNum; ++i,j++) {
		//fprintf(stderr, "#%d, cnt = %u\n", j, wordManager->word[sortedWordIdx[j]].wordCnt);
		HashIdx hashIdx;
		sampleManager->word[i] = wordManager->word[sortedWordIdx[j]];
		sampleManager->word[i].wordVec = (WordVec*) malloc_wrap((sampleManager->axisNum+1) * sizeof(WordVec));
		check_word_existence(sampleManager, sampleManager->word[i].wordStr, &hashIdx);
		sampleManager->wordIdxTable[hashIdx] = i;
		sampleManager->axisIdxTable[i] = i;
		sampleManager->ptrAxis[i] = &sampleManager->word[i];
	}

	free(sortedWordIdx);

	/*
	 * 2. calculate vector of samples.
	 */
	fprintf(stdout, "\t\t[*] simulation using sample manager... [stage 2]\n");
	start = clock();
	calculate_vector(sampleManager, filename);
	end = clock();
	fprintf(stdout, "\t\t[T] simulation : %lf sec\n", (double)(end-start)/CLOCKS_PER_SEC);

	/*
	 * 3. select axis words in samples.
	 */
	fprintf(stdout, "\t\t[*] creating axisIdxTable... [stage 3]\n");	

	// calculate sample words' rates of vector to itself.
	fprintf(stdout, "\t\t\t[*] calculating sample words' rates of vector to itself...\n");
	start = clock();
	rate = (WordVec*) malloc_wrap((sampleManager->axisNum+1) * sizeof(WordVec));
	for(i=1; i <= sampleManager->wordNum; ++i) {
		rate[i] = 0;
		for(j=1; j <= sampleManager->axisNum; ++j) {
			rate[i] += sampleManager->word[i].wordVec[j];
		}
		rate[i] = sampleManager->word[i].wordVec[i] / rate[i];
	}
	end = clock();
	fprintf(stdout, "\t\t\t[T] calculating sample words' rates of vector to itself : %lf sec\n", (double)(end-start)/CLOCKS_PER_SEC);
	
	// sort sample words in vector to itself desc order.
	fprintf(stdout, "\t\t\t[*] sorting sample words...\n");
	start = clock();
	for(i=1; i <= sampleManager->wordNum; ++i) {
		WordIdx wi = sampleWordIdx[i] = i;
		for(j=i-1; j >= 1 && rate[sampleWordIdx[j]] < rate[wi]; --j)
			sampleWordIdx[j+1] = sampleWordIdx[j];
		sampleWordIdx[j+1] = wi;
	}
	end = clock();
	fprintf(stdout, "\t\t\t[T] sorting sample words : %lf sec\n", (double)(end-start)/CLOCKS_PER_SEC);

	free(rate);

	// create word manager's axisIdxTable.
	fprintf(stdout, "\t\t\t[*] create word manager's axisIdxTable...\n");
	wordManager->axisNum = (sampleManager->wordNum < AXIS_NUM ? sampleManager->wordNum : AXIS_NUM);
	wordManager->axisIdxTable = (AxisIdx*) calloc_wrap(wordManager->wordNum+1, sizeof(AxisIdx));
	wordManager->ptrAxis = (Word**) calloc_wrap(wordManager->axisNum+1, sizeof(Word*));
	for(i=1; i <= wordManager->axisNum; ++i) {
		WordIdx wordIdx = get_wordIdx(wordManager, sampleManager->word[sampleWordIdx[i]].wordStr);
		wordManager->axisIdxTable[wordIdx] = i;
		wordManager->ptrAxis[i] = &wordManager->word[wordIdx];
	}

	wordManager->word[0].wordVec = (WordVec*) malloc_wrap((wordManager->axisNum+1) * sizeof(WordVec));
	for(i=1; i <= wordManager->wordNum; ++i) {
		wordManager->word[i].wordVec = (WordVec*) malloc_wrap((wordManager->axisNum+1) * sizeof(WordVec));
	}

	free_wordManager(sampleManager);
}

bool check_word_existence(const WordManager* wordManager, const char* wordStr, HashIdx *pHashIdx)
{
	HashIdx hashIdx = hash_word(wordStr);

	while(wordManager->wordIdxTable[hashIdx] != 0) {
		Word* curWord = &wordManager->word[wordManager->wordIdxTable[hashIdx]];
		if(!strcmp(curWord->wordStr, wordStr)) {
			*pHashIdx = hashIdx;
			return true;
		}
		hashIdx = collision_hash(hashIdx, wordStr);
	}
	*pHashIdx = hashIdx;
	return false;
}

bool register_word(WordManager *wordManager, const char* wordStr)
{
	HashIdx hashIdx;

	if(check_word_existence(wordManager, wordStr, &hashIdx)) {
		++wordManager->word[get_wordIdx(wordManager, wordStr)].wordCnt;
		return false;
	} else {
		wordManager->wordIdxTable[hashIdx] = ++wordManager->wordNum;
		wordManager->word[wordManager->wordNum].wordCnt = 1;
		if(wordManager->wordNum+1 > MAX_WORD_NUM) {
			fprintf(stderr, "[Error] MAX_WORD_NUM is small!\n");
			exit(1);
		}
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
	time_t start, end;
	int i,j;
	
	// initializing vectors
	fprintf(stdout, "\t\t\t[*] initializing vectors...\n");
	start = clock();
	for(i=1; i <= wordManager->wordNum; ++i) {
		for(j=1; j <= wordManager->axisNum; ++j) {
			wordManager->word[i].wordVec[j] = 0;
		}
	}
	end = clock();
	fprintf(stdout, "\t\t\t[T] initializing vectors : %lf sec\n", (double)(end-start)/CLOCKS_PER_SEC);

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
			wordManager->word[i].wordVec[j] /= wordManager->ptrAxis[j]->wordCnt;
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
		if('A' <= ch && ch <= 'Z') ch -= 'A'-'a';
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
	fwrite(wordManager->axisIdxTable+1, sizeof(AxisIdx), wordManager->wordNum, fp);
	
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
	free(wordManager->ptrAxis);
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
