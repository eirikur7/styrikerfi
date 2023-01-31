#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

struct ListItem {
    struct ListItem *next;
    int        value;
};

struct ListItem *listHead = NULL;

void 
appendItem(int value)
{
    //...implement this
    // append to the end of the list

    // Allocate memory for the new element
    struct ListItem *item = (struct ListItem *)malloc(sizeof(struct ListItem));
    // Missing here: error handling
    // Note that the assignment 3 specifies a specific way errors shall be handled

    // Set the content of the new item to value
    item->value = value;

    // We append at the end, so the next pointer of the new element shall be NULL
    item->next = NULL;

    // Special case: if the list is empty, we just set listHead to our new element,
    // making it the first and only element in the list
    if (listHead == NULL) {
        listHead = item;
        return;
    }

    // Otherwise, we have to find the end of the list
    // We go through the list until we find the last element (the one with next==NULL).
    struct ListItem *lastel = listHead;
    while (lastel->next)
        lastel = lastel->next;

    // and then we append our new element to the last
    lastel->next = item;
}

int 
removeFirstItem()
{
    //implement this
    // removes the first list item from the list and returns its value; returns - 1 if list is empty
    if (listHead == NULL)
        return -1;

    // We remove the first element by updating the listHead to point to the element after the current
    // listHead.  We also remember the removed element (the "old" listhead) in tbr ("to be removed")
    struct ListItem *tbr = listHead;
    listHead = listHead->next;

    // We get the value from the list head first, as after the free, the value is no longer available
    int val = tbr->value;
    free(tbr);
    return val;
}

int 
containsItem(int value)
{
    //implement this
    // return true(1) if list contains value, false(0)
    
    // we iterate over the list, starting with the first element
    struct ListItem *listElement = listHead;
    int isContained = 0;

    // while we have not yet reached the end
    while(listElement) {
        if(listElement->value == value)
            isContained = 1;
        // advance to the next element
        listElement = listElement->next;
    }
    return isContained;
}

int isEmpty() {
    // Shorthand notation in C for a comparison plus assignment
    // if the condition is true, the result is the value after the "?" / before the ":"
    // if it is false, the result is the value after the ":"
    return listHead == NULL ? 1 : 0;

    // equivalent to:
    // if (listHead==NULL) { return 1; } else { return 0; }

    // equivalent to
    // (because C uses 1 as "true" and 0 as "false")
    return (listHead==NULL);
}

int main() {
    appendItem(42);
    appendItem(4711);
    removeFirstItem();
    appendItem(42);
    appendItem(4);
    for(int i=0; i<5; i++) printf("%d\n", removeFirstItem());
}
