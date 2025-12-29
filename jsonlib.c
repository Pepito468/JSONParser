#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/types.h>
#include "jsonlib.h"

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
    new -> key = strndup(key + sizeof(char), strlen(key) - 2);
    new -> value = value;
    return new;
}

json_value_t* json_create_value(json_value_type_t type, void *data) {

    json_value_t *new_value = malloc(sizeof(json_value_t));
    new_value -> type = type;

    switch (type) {
        case JSTRING:
            new_value -> data.string = strndup(data + sizeof(char), strlen(data) - 2);
            break;
        case JNUMBER:
            new_value -> data.number = *((double*) data);
            break;
        case JOBJECT:
            new_value -> data.object = data;
            break;
        case JARRAY:
            new_value -> data.array = data;
            break;
        case JTRUE:
            new_value -> data.boolean = true;
            break;
        case JFALSE:
            new_value -> data.boolean = false;
            break;
        case JNULL: 
            new_value -> data.nullpt = NULL;
            break;
        default:
            printf("ERROR\n");
    }

    return new_value;
}

void json_object_print_recursive(json_pair_list_node_t *json, int depth) {
    printf("{\n");
    for (json_pair_list_node_t *current = json; current; current = current -> next) {
        for(int i = 0; i < depth; i++) printf(PADDING);
        printf("\"%s\" : ", current -> pair -> key);
        switch (current -> pair -> value -> type) {
            case JSTRING:
                printf("\"%s\"", current -> pair -> value -> data.string);
                break;
            case JNUMBER:
                printf("%f", current -> pair -> value -> data.number);
                break;
            case JOBJECT:
                json_object_print_recursive(current -> pair -> value -> data.object, depth + 1);
                break;
            case JARRAY:
                json_array_print(current -> pair -> value -> data.array, depth + 1);
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
                printf("\"%s\"", current -> value -> data.string);
                break;
            case JNUMBER:
                printf("%f", current -> value -> data.number);
                break;
            case JOBJECT:
                json_object_print_recursive(current -> value -> data.object, depth + 1);
                break;
            case JARRAY:
                json_array_print(current -> value -> data.array, depth + 1);
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
    for (json_value_list_node_t *current = array; current; current = current -> next) {
        switch (current -> value -> type) {
            case JSTRING:
                free(current -> value -> data.string);
                break;
            case JOBJECT:
                json_free_object(current -> value -> data.object);
                break;
            case JARRAY:
                json_free_array(current -> value -> data.array);
                break;
            default: /* Other types don't need to be freed */
                break;
        }
    }
    free(array);
}

/* Frees Json Object memory */
void json_free_object(json_pair_list_node_t *json) {
    for (json_pair_list_node_t *current = json; current; current = current -> next) {
        free(current -> pair -> key);
        switch (current -> pair -> value -> type) {
            case JSTRING:
                free(current -> pair -> value -> data.string);
                break;
            case JOBJECT:
                json_free_object(current -> pair -> value -> data.object);
                break;
            case JARRAY:
                json_free_array(current -> pair -> value -> data.array);
                break;
            default: /* Other types don't need to be freed */
                break;
        }
    }
    free(json);
}

