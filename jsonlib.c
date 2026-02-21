#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include "jsonlib.h"
#include "bison.tab.h"

#define PADDING "    "

json_value_list_node_t* json_add_value_to_head(json_value_list_node_t *list, json_value_t *value) {
    json_value_list_node_t *new = malloc(sizeof(json_value_list_node_t));
    new -> value = value;
    new -> next = list;
    list = new;
    return new;
}

json_pair_list_node_t* json_add_pair_to_head(json_pair_list_node_t *list, json_pair_t *pair) {
    json_pair_list_node_t *new = malloc(sizeof(json_pair_list_node_t));
    new -> pair = pair;
    new -> next = list;
    list = new;
    return list;
}

json_pair_t* json_create_pair(char *key, json_value_t *value) {
    json_pair_t *new = malloc(sizeof(json_pair_t));
    new -> key = key;
    new -> value = value;
    return new;
}


json_pair_list_node_t* json_parse(FILE *file) {
    return flexbison(file);
}

json_value_t* json_create_value(json_value_type_t type, void *data) {

    json_value_t *new_value = malloc(sizeof(json_value_t));
    new_value -> type = type;
    new_value -> data = data;

    return new_value;
}

void json_object_print_recursive(json_pair_list_node_t *json, int depth) {
    printf("{\n");
    for (json_pair_list_node_t *current = json; current; current = current -> next) {
        for(int i = 0; i < depth; i++) printf(PADDING);
        printf("\"%s\" : ", current -> pair -> key);
        switch (current -> pair -> value -> type) {
            case JSTRING:
                printf("\"%s\"", (char*) current -> pair -> value -> data);
                break;
            case JNUMBER:
                printf("%f", *(double*) current -> pair -> value -> data);
                break;
            case JOBJECT:
                json_object_print_recursive(current -> pair -> value -> data, depth + 1);
                break;
            case JARRAY:
                json_array_print(current -> pair -> value -> data, depth + 1);
                break;
            case JTRUE:
                printf("true");
                break;
            case JFALSE:
                printf("false");
                break;
            case JNULL: 
                printf("null");
                break;
            default:
                printf("ERROR\n");
        }
        if (current -> next)
            printf(",");
        printf("\n");
    }
    for(int i = 0; i < depth - 1; i++) printf(PADDING);
    printf("}");
}


void json_object_print(json_pair_list_node_t *json) {
    json_object_print_recursive(json, 1);
}

void json_array_print(json_value_list_node_t *array, int depth) {
    printf("[");
    for (json_value_list_node_t *current = array; current; current = current -> next) {
        switch (current -> value -> type) {
            case JSTRING:
                printf("\"%s\"", (char*) current -> value -> data);
                break;
            case JNUMBER:
                printf("%f", *(double*) current -> value -> data);
                break;
            case JOBJECT:
                json_object_print_recursive(current -> value -> data, depth + 1);
                break;
            case JARRAY:
                json_array_print(current -> value -> data, depth + 1);
                break;
            case JTRUE:
                printf("true");
                break;
            case JFALSE:
                printf("false");
                break;
            case JNULL: 
                printf("null");
                break;
            default:
                printf("ERROR\n");
        }
        if (current -> next)
            printf(", ");
    }
    printf("]");
}

json_value_t* json_get_value(json_pair_list_node_t *json, char *key) {
    for (json_pair_list_node_t *current = json; current; current = current -> next) {
        if (!strcmp(current -> pair -> key, key)) {
            return current -> pair -> value;
        }
    }
    return NULL;
}

/* Frees Json Array memory */
void json_free_array(json_value_list_node_t *array) {
    for (json_value_list_node_t *current = array; current;) {
        switch (current -> value -> type) {
            case JSTRING:
                free(current -> value -> data);
                break;
            case JOBJECT:
                json_free_object(current -> value -> data);
                break;
            case JARRAY:
                json_free_array(current -> value -> data);
                break;
            default: /* Other types don't need to be freed */
                break;
        }
        free(current -> value);
        json_value_list_node_t *temp = current;
        current = current -> next;
        free(temp);
    }
}

/* Frees Json Object memory */
void json_free_object(json_pair_list_node_t *json) {
    for (json_pair_list_node_t *current = json; current;) {
        free(current -> pair -> key);
        switch (current -> pair -> value -> type) {
            case JSTRING:
                free(current -> pair -> value -> data);
                break;
            case JNUMBER:
                free(current -> pair -> value -> data);
                break;
            case JOBJECT:
                json_free_object(current -> pair -> value -> data);
                break;
            case JARRAY:
                json_free_array(current -> pair -> value -> data);
                break;
            default: /* Other types don't need to be freed */
                break;
        }
        free(current -> pair -> value);
        free(current -> pair);
        json_pair_list_node_t *temp = current;
        current = current -> next;
        free(temp);
    }
}
