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

/* Json type struct: contains the data type and the data itself */
typedef struct json_value {
    json_value_type_t type; /* Enum of the type inside of data */
    void *data; /* Data: the type of data depends on the one indicated by the type */

    struct json_value *next;
} json_value_t;

/* Json value: contains the key (null terminated char array) and the corresponding value */
typedef struct json_pair {
    char *key;
    struct json_value *value;

    struct json_pair *next;
} json_pair_t;

typedef struct json_object {
    json_pair_t *pair_list_head;
} json_object_t;

typedef struct json_array {
    json_value_t *value_list_head;
} json_array_t;


json_object_t* json_create_object();
json_array_t* json_create_array();

json_pair_t* json_create_pair(char *key, json_value_t *value);
json_value_t* json_create_value(json_value_type_t type, void *data);

void json_concatenate_pairs(json_pair_t *list, json_pair_t *pair);
void json_concatenate_values(json_value_t *list, json_value_t *value);

/* Returns the value from the Json Object that is associated with the given key. Returns NULL if it finds nothing */
json_value_t* json_get_value_from_object(json_object_t *json_object, char *key);

/* Returns the value from the Json Object that is at the given position (starts at 0). Returns NULL if there is no item */
json_value_t* json_get_value_from_array(json_array_t *json_array, unsigned int position);

/* Prints the given Json Object */
void json_object_print(json_object_t *json);

/* Prints the given Json Array */
void json_array_print(json_array_t *array);

/* Frees Json Object memory */
void json_free_object(json_object_t *json_object);

/* Parses the given file and returns a Json Object */
json_object_t* json_parse(FILE *file);

#endif
