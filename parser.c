#include <stdio.h>
#include "jsonlib.h"

int main(int argc, char **argv) {

    if (argc != 3)
        return 1;

    FILE *p = fopen(argv[1], "r");
    if (!p)
        return 1;

    json_object_t *json = json_parse(p);
    fclose(p);
    json_object_print(json);

    printf("\n");
    printf("\n");
    json_value_t *mail = json_get_value(json, "email");
    if (mail)
        printf("MAIL: %s\n", (char*) mail -> data);

    json_value_t *id = json_get_value(json, "id");
    if (id)
        printf("ID: %f\n", *(double*)json_get_value(json, "id") -> data);

    json_value_t *profile = json_get_value(json, "profile");
    if (profile) {
        json_value_t *age = json_get_value(profile -> data, "age");
        if (age)
            printf("PROFILE -> AGE: %f\n", *(double*) age -> data);
    }

    if (json_get_value(json, argv[2]))
        printf("Has a \"%s\" key!\n", argv[2]);
    else
        printf("Does not have a \"%s\" key!\n", argv[2]);

    json_free_object(json);

    return 0;
}
