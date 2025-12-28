#ifndef JSONLIB
#define JSONLIB

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define DEFAULT_MAX_SIZE 16
#define SIZE_INCREMENT_MULTIPLIER 2

typedef enum VALUE_TYPE {JSTRING, JNUMBER, JOBJECT, JARRAY, JTRUE, JFALSE, JNULL} json_value_type_t;

struct json_value;
struct json_pair;
struct pair_list_node;
struct value_list_node;

union data {
    char *string;
    double number;
    struct json_pair_list_node *object;
    struct json_value_list_node *array;
    bool boolean;
    void *nullpt;
};

/* Json type struct: contains the type and the data */
typedef struct json_value {
    json_value_type_t type; /* Enum of the type inside of data */
    union data data; /* Data: the type of data depends on the one indicated by the type */
} json_value_t;

/* Json Array as a list of values */
typedef struct json_value_list_node {
    json_value_t *value;
    struct json_value_list_node *next;
} json_value_list_node_t;

/* Json value: contains the key (null terminated char array) and the corresponding value */
typedef struct json_pair {
    char *key;
    struct json_value *value;
} json_pair_t;

/* Json Object as a list of pairs */
typedef struct json_pair_list_node {
    json_pair_t *pair;
    struct json_pair_list_node *next;
} json_pair_list_node_t;

json_value_list_node_t* json_add_value_to_head(json_value_list_node_t *list, json_value_t *value);

json_pair_list_node_t* json_add_pair_to_head(json_pair_list_node_t *list, json_pair_t *pair);

json_pair_t* json_create_pair(char *key, json_value_t *value);

json_value_t* json_create_value(json_value_type_t type, void *data);

/* Prints the given Json Object */
void json_object_print(json_pair_list_node_t *json);

/* Prints the given Json Array */
void json_array_print(json_value_list_node_t *array);

/* Returns the value from the Json that is associated with the given key. Returns NULL if it finds nothing */
json_value_t* json_get_value(json_pair_list_node_t *json, char *key);

/* Frees Json Object memory */
void json_free_object(json_pair_list_node_t *json);

#endif
