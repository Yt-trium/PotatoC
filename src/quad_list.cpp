#include "quad_list.h"



void quad_list_print (quad_list *ql)
{
    puts((ql != NULL?"List of quads:":"quad list empty."));
    while(ql != NULL){
        quad_print(ql->q);
        ql = ql->next;
    }
}

void quad_list_complete (quad_list* list, symbol label)
{
    while(list != NULL) {
        list->q->res = label;
        list = list->next;
    }
}

void quad_list_add(quad_list** dest, quad_list* src) {
    if(*dest == NULL) {
        *dest = src;
    }
    else {
        quad_list* scan = *dest;
        while(scan->next != NULL)
            scan = scan->next;
        scan->next = src;
    }
}

quad_list* quad_list_new  (quad *q)
{
    quad_list *ql = (quad_list*)malloc(sizeof(quad_list));
    ql->q = q;
    ql->previous = NULL;
    ql->next = NULL;
    return ql;
}

void quad_list_free (quad_list *ql){
    free(ql);
}
