#include <stdio.h>
#include "jsonlib.h"
#include "bison.tab.h"

int main(int argc, char **argv) {

    FILE *p = fopen("sample.json", "r");
    if (!p)
        return 1;

    json_pair_list_node_t *json = flexbison(p);
    json_object_print(json);

    printf("\n");
    printf("\n");
    json_value_t *mail = json_get_value(json, "email");
    if (mail)
        printf("MAIL: %s\n", mail -> data . string);
    printf("ID: %f\n", json_get_value(json, "id") -> data . number);
    printf("TIMEZONE: %s\n", json_get_value(json_get_value(json_get_value(json, "profile") -> data.object, "preferences") -> data.object, "timezone") -> data . string);
    if (json_get_value(json_get_value(json_get_value(json_get_value(json, "profile") -> data.object, "preferences") -> data.object, "notifications") -> data . object, "email") -> data.boolean)
        printf("Has Email!\n");
    if (!json_get_value(json_get_value(json_get_value(json_get_value(json, "profile") -> data.object, "preferences") -> data.object, "notifications") -> data . object, "sms") -> data.boolean)
        printf("Does not have sms!\n");

    if (!json_get_value(json, "Test"))
        printf("Does not have a 'Test' key!\n");

    json_free_object(json);

    return 0;
}
