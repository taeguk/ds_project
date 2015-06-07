#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

struct winsize w;
 
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
