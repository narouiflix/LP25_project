#include <files-list.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

/*!
 * @brief clear_files_list clears a files list
 * @param list is a pointer to the list to be cleared
 * This function is provided, you don't need to implement nor modify it
 */
void clear_files_list(files_list_t *list) {
    while (list->head) {
        files_list_entry_t *tmp = list->head;
        list->head = tmp->next;
        free(tmp);
    }
}

/*!
 *  @brief add_file_entry adds a new file to the files list.
 *  It adds the file in an ordered manner (strcmp) and fills its properties
 *  by calling stat on the file.
 *  Il the file already exists, it does nothing and returns 0
 *  @param list the list to add the file entry into
 *  @param file_path the full path (from the root of the considered tree) of the file
 *  @return 0 if success, -1 else (out of memory)
 */
int add_file_entry(files_list_t *list, char *file_path) {

    if (strlen(file_path) >= 4096) {
        return -1;
    }

    files_list_entry_t *new_entry = (files_list_entry_t*) malloc(sizeof(files_list_entry_t));
    strcpy(new_entry->path_and_name, file_path);

    if (list->head == NULL) {
        list->head = new_entry;
        return 0;
    }

    files_list_entry_t *p_entry = list->head;

    while (p_entry != NULL && strcmp(p_entry->path_and_name, file_path) < 0) {
        p_entry = p_entry->next;
    }

    if (p_entry == list->head) {
        list->head->prev = new_entry;
        new_entry->next = list->head;
        list->head = new_entry;
    }
    else if (p_entry == NULL) {
        list->tail->next = new_entry;
        new_entry->prev = list->tail;
        list->tail = new_entry;
    }
    else {
        new_entry->next = p_entry;
        new_entry->prev = p_entry->prev;
        p_entry->prev->next = new_entry;
        p_entry->prev = new_entry;
    }

    return 0;
}

/*!
 * @brief add_entry_to_tail adds an entry directly to the tail of the list
 * It supposes that the entries are provided already ordered, e.g. when a lister process sends its list's
 * elements to the main process.
 * @param list is a pointer to the list to which to add the element
 * @param entry is a pointer to the entry to add. The list becomes owner of the entry.
 * @return 0 in case of success, -1 else
 */
int add_entry_to_tail(files_list_t *list, files_list_entry_t *entry) {
    if (list == NULL || entry == NULL) {
        return -1;
    }

    if (list->head == NULL) {
        list->head = entry;
        list->tail = entry;
    }
    else {
        list->tail->next = entry;
        entry->prev = list->tail;
        list->tail = entry;
    }

    return 0;
}

/*!
 *  @brief find_entry_by_name looks up for a file in a list
 *  The function uses the ordering of the entries to interrupt its search
 *  @param list the list to look into
 *  @param file_path the full path of the file to look for
 *  @param start_of_src the position of the name of the file in the source directory (removing the source path)
 *  @param start_of_dest the position of the name of the file in the destination dir (removing the dest path)
 *  @return a pointer to the element found, NULL if none were found.
 */
files_list_entry_t *find_entry_by_name(files_list_t *list, char *file_path, size_t start_of_src, size_t start_of_dest) {
    files_list_entry_t *p_entry = list->head;

    while (p_entry != NULL && strcmp(p_entry->path_and_name, file_path) != 0) {
        p_entry = p_entry->next;
    }

    return p_entry;
}

/*!
 * @brief display_files_list displays a files list
 * @param list is the pointer to the list to be displayed
 * This function is already provided complete.
 */
void display_files_list(files_list_t *list) {
    if (!list)
        return;
    
    for (files_list_entry_t *cursor=list->head; cursor!=NULL; cursor=cursor->next) {
        printf("%s\n", cursor->path_and_name);
    }
}

/*!
 * @brief display_files_list_reversed displays a files list from the end to the beginning
 * @param list is the pointer to the list to be displayed
 * This function is already provided complete.
 */
void display_files_list_reversed(files_list_t *list) {
    if (!list)
        return;
    
    for (files_list_entry_t *cursor=list->tail; cursor!=NULL; cursor=cursor->prev) {
        printf("%s\n", cursor->path_and_name);
    }
}
