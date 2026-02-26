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

json_object_t* json_parse(FILE *file) {
    return flexbison(file);
}

json_object_t* json_create_object() {
    json_object_t *new_object = malloc(sizeof(json_object_t));
    new_object -> pair_list_head = NULL;
    return new_object;
}

json_array_t* json_create_array() {
    json_array_t *new_array = malloc(sizeof(json_array_t));
    new_array -> value_list_head = NULL;
    return new_array;
}

void json_concatenate_pairs(json_pair_t *list, json_pair_t *pair) {
    list -> next = pair;
}

void json_concatenate_values(json_value_t *list, json_value_t *value) {
    list -> next = value;
}

json_pair_t* json_create_pair(char *key, json_value_t *value) {
    json_pair_t *new_pair = malloc(sizeof(json_pair_t));
    new_pair -> key = key;
    new_pair -> value = value;
    new_pair -> next = NULL;
    return new_pair;
}


json_value_t* json_create_value(json_value_type_t type, void *data) {

    json_value_t *new_value = malloc(sizeof(json_value_t));
    new_value -> type = type;
    new_value -> data = data;
    new_value -> next = NULL;
    return new_value;
}

void json_object_print(json_object_t *json) {
    printf("{\n");
    for (json_pair_t *current = json -> pair_list_head; current; current = current -> next) {
        printf("\"%s\" : ", current -> key);
        switch (current -> value -> type) {
            case JSTRING:
                printf("\"%s\"", (char*) current -> value -> data);
                break;
            case JNUMBER:
                printf("%f", *(double*) current -> value -> data);
                break;
            case JOBJECT:
                json_object_print(current -> value -> data);
                break;
            case JARRAY:
                json_array_print(current -> value -> data);
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
    printf("}");
}

void json_array_print(json_array_t *array) {
    printf("[");
    for (json_value_t *current = array -> value_list_head; current; current = current -> next) {
        switch (current -> type) {
            case JSTRING:
                printf("\"%s\"", (char*) current -> data);
                break;
            case JNUMBER:
                printf("%f", *(double*) current -> data);
                break;
            case JOBJECT:
                json_object_print(current -> data);
                break;
            case JARRAY:
                json_array_print(current -> data);
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

json_value_t* json_get_value(json_object_t *json, char *key) {
    for (json_pair_t *current = json -> pair_list_head; current; current = current -> next) {
        if (!strcmp(current -> key, key)) {
            return current -> value;
        }
    }
    return NULL;
}

/* Frees Json Array memory */
void json_free_array(json_array_t *json_array) {
    for (json_value_t *current = json_array -> value_list_head; current;) {
        switch (current -> type) {
            case JSTRING:
                free(current -> data);
                break;
            case JNUMBER:
                free(current -> data);
                break;
            case JOBJECT:
                json_free_object(current -> data);
                break;
            case JARRAY:
                json_free_array(current -> data);
                break;
            default: /* Other types don't need to be freed */
                break;
        }
        json_value_t *temp = current;
        current = current -> next;
        free(temp);
    }
    free(json_array);
}

/* Frees Json Object memory */
void json_free_object(json_object_t *json_object) {
    for (json_pair_t *current = json_object -> pair_list_head; current;) {
        free(current -> key);
        switch (current -> value -> type) {
            case JSTRING:
                free(current -> value -> data);
                break;
            case JNUMBER:
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
        json_pair_t *temp = current;
        current = current -> next;
        free(temp);
    }
    free(json_object);
}
