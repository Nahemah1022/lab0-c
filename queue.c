#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty node.
 * Return NULL if could not allocate space.
 */
bool create_node(list_ele_t **newh, char *s)
{
    size_t len = strlen(s) + 1;
    if (!(*newh = malloc(sizeof(list_ele_t))))
        return false;
    if (!((*newh)->value = malloc(len * sizeof(char)))) {
        free(*newh);
        return false;
    }
    (*newh)->next = NULL;

    memcpy((*newh)->value, s, len);
    return true;
}

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q) {
        list_ele_t *e = q->head;
        while (e) {
            free(e->value);
            list_ele_t *n = e->next;
            free(e);
            e = n;
        }
        free(q);
    }
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    if (!q || !create_node(&newh, s))
        return false;
    newh->next = q->head;
    q->head = newh;
    if (!((q->size)++))
        q->tail = newh;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newh;
    if (!q || !create_node(&newh, s))
        return false;
    if ((q->size)++) {
        q->tail->next = newh;
    } else {
        q->head = newh;
    }
    q->tail = newh;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head) {
        return false;
    }

    if (sp) {
        size_t minlen = (strlen(q->head->value) < bufsize - 1)
                            ? strlen(q->head->value)
                            : bufsize - 1;
        memcpy(sp, q->head->value, minlen);
        *(sp + minlen) = '\0';
    }
    list_ele_t *n = q->head;
    q->head = n->next;
    free(n->value);
    free(n);
    (q->size)--;
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q) {
        return 0;
    }
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head || !q->head->next) {
        return;
    }

    q->tail = q->head;
    list_ele_t *prev = NULL, *cur = q->head, *next;
    while (cur) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    q->head = prev;
}

void merge_sort(list_ele_t **list)
{
    if (!(*list) || !(*list)->next) {
        return;
    }

    list_ele_t *slow = *list, *fast = (*list)->next;
    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    list_ele_t *lhs = *list, *rhs = slow->next, *result = NULL;
    slow->next = NULL;
    merge_sort(&lhs);
    merge_sort(&rhs);

    list_ele_t **indirect = &result;
    // compare and merge list
    while (lhs && rhs) {
        if (strcasecmp(lhs->value, rhs->value) < 0) {
            *indirect = lhs;
            lhs = lhs->next;
        } else {
            *indirect = rhs;
            rhs = rhs->next;
        }
        indirect = &(*indirect)->next;
    }
    // concatenate remaining nodes
    if (lhs) {
        *indirect = lhs;
    } else if (rhs) {
        *indirect = rhs;
    }

    *list = result;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head || !q->head->next) {
        return;
    }

    merge_sort(&(q->head));
    list_ele_t *tmp = q->head;
    while (tmp->next) {
        tmp = tmp->next;
    }
    q->tail = tmp;
}