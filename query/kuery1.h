
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
 * Function Name    : get_close_words_menu
 * Parameter        : dataManager
 * Return value     : void
 * Role             :
 *      1. input word from user
 *      2. print most highly related words with inputted word.
 */
void get_close_words_menu(DataManager* dataManager);
