
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <json-c/json.h>

void show_help()
{
    fprintf(stdout, "Usage: ktm_poi [-i FILE] [-h]\n");
    fprintf(stdout, "\t-i, --input\n");
    fprintf(stdout, "\t\tinput json file got from the KTM API that will get parsed\n");
    fprintf(stdout, "\t-h, --help\n");
    fprintf(stdout, "\t\tthis message\n");
}

char *get(struct json_object *new_obj, const char *key)
{
    struct json_object *o = NULL;
    if (!json_object_object_get_ex(new_obj, key, &o)) {
        return "";
    } else {
        return (char *)json_object_get_string(o);
    }
}

char *remove_char_from_str(char *str, char c)
{
    char *pr = str, *pw = str;
    while (*pr) {
        *pw = *pr++;
        pw += (*pw != c);
    }
    *pw = '\0';
    return str;
}

char *replace_char_from_str(char *str, char s, char d)
{
    char *pr = str;
    while (*pr) {
        *pr++;
        if (*pr == s) {
            *pr = d;
        }
    }
    return str;
}

int main(int argc, char **argv, char **env)
{
    json_object *root_obj, *el_obj;
    int i;
    int num;
    int q, opt_idx;
    char *file = NULL;

    if (argc == 1) {
        show_help();
        return EXIT_SUCCESS;
    }

    while (1) {
        opt_idx = 0;
        static struct option long_options[] = {
            {"input", 1, 0, 'i'},
            {"help", 0, 0, 'h'},
            {0, 0, 0, 0}
        };

        q = getopt_long(argc, argv, "i:h", long_options, &opt_idx);
        if (q == -1) {
            break;
        }
        switch (q) {
        case 'i':
            file = optarg;
            break;
        case 'h':
            show_help();
            break;
        default:
            break;
        }
    }

    if (file == NULL) {
        fprintf(stderr, "failure: input file is undefined\n");
        return EXIT_FAILURE;
    }

    root_obj = json_object_from_file(file);

    if (root_obj) {

        num = json_object_array_length(root_obj);

        for (i = 0; i < num; i++) {
            el_obj = json_object_array_get_idx(root_obj, i);

            if (el_obj == NULL) {
                json_object_put(root_obj);
                fprintf(stderr, "failure reading json element\n");
                return EXIT_FAILURE;
            }

            if (strlen(get(el_obj, "lng")) > 0) {
                fprintf(stdout, "%s", get(el_obj, "lng"));
            }

            if (strlen(get(el_obj, "lat")) > 0) {
                fprintf(stdout, ", %s", get(el_obj, "lat"));
            }

            if (strlen(get(el_obj, "name")) > 0) {
                //fprintf(stdout, ", %s", get(el_obj, "name"));
                fprintf(stdout, ", \"%s\"", remove_char_from_str(remove_char_from_str(get(el_obj, "name"), '-'), '"'));
                //fprintf(stdout, ", \"%s - ", remove_char_from_str(get(el_obj, "name"), '-'));
            }

            // add comment - max 40 chars long for garmin 60CSx
            fprintf(stdout, ", \"");

            //if (strlen(get(el_obj, "address")) > 0) {
                //fprintf(stdout, ",%s", get(el_obj, "address"));
            //    fprintf(stdout, ",%s", replace_char_from_str(get(el_obj, "address"), ',', ' '));
            //}

            if (strlen(get(el_obj, "tel")) > 0) {
                fprintf(stdout, "%s", remove_char_from_str(get(el_obj, "tel"), ' '));
            }

            if (strlen(get(el_obj, "website")) > 0) {
                fprintf(stdout, " %s", get(el_obj, "website"));
            }

            fprintf(stdout, "\"\n");
        }

        json_object_put(root_obj);
    }

    return 0;
}
