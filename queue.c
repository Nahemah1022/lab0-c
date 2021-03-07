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
    /* TODO: What if malloc returned NULL? */
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
    /* TODO: How about freeing the list elements and the strings? */
    if (q) {
        list_ele_t *e = q->head;
        while (e) {
            free(e->value);
            list_ele_t *n = e->next;
            free(e);
            e = n;
        }
        /* Free queue structure */
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
    /* TODO: What should you do if the q is NULL? */
    if (!q || !create_node(&newh, s))
        return false;
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
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
    /* TODO: You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    /* TODO: Remove the above comment when you are about to implement. */
    if (!q || !create_node(&newh, s))
        return false;
    /* Don't forget to allocate space for the string and copy it */
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

    char *pivot;
    int i = 0;
    list_ele_t *beg[q->size + 1], *end[q->size + 1], *junc = NULL;
    for (int j = 0; j <= q->size; ++j) {
        beg[j] = NULL;
        end[j] = NULL;
    }
    beg[0] = q->head;
    end[0] = q->tail;
    while (i >= 0) {
        list_ele_t *lh = beg[i], *rh = beg[i], *lt = beg[i], *rt = beg[i];
        if (beg[i] != end[i]) {
            pivot = beg[i]->value;
            list_ele_t *p = beg[i];
            while (p != end[i]) {
                p = p->next;
                if (strcasecmp(p->value, pivot) < 0) {
                    lt = (lh != beg[i]) ? (lt->next = p) : (lh = p);
                } else {
                    rt = (rh != beg[i]) ? (rt->next = p) : (rh = p);
                }
            }
            lt->next = beg[i];
            beg[i]->next = rh;
            rt->next = NULL;

            beg[i + 1] = beg[i];
            end[i + 1] = beg[i];
            beg[i] = lh;
            end[i] = lt;
            beg[i + 2] = rh;
            end[i + 2] = rt;
            i += 2;
        } else {
            if (beg[i] != junc)
                beg[i]->next = junc;
            junc = beg[i];
            i--;
        }
    }
    list_ele_t *ptr;
    for (ptr = beg[0]; ptr->next; ptr = ptr->next)
        ;
    q->head = beg[0];
    q->tail = ptr;
}