#include <alsa/asoundlib.h>
#include <stdio.h>

void print_card_list();
void print_device_hints(int card);

#define NUM_IFACES 4
const char *ifaces[NUM_IFACES] = {"pcm", "rawmidi", "timer", "seq"};

int main(){
    print_card_list();
    return 0;
}

void print_card_list(){
    int card = -1;
    char *name;
    char *longname;

    snd_card_next(&card);
    while (card != -1){
        if (snd_card_get_name(card, &name) < 0){
            fprintf(stderr, "Can't get name\n");
            goto finish;
        }
        if (snd_card_get_longname(card, &longname) < 0){
            fprintf(stderr, "Can't get longname\n");
            goto finish;
        }

        printf("\n%d: %s, %s\n", card, name, longname);
        print_device_hints(card);

        finish:
        free(name);
        free(longname);
        snd_card_next(&card);
    }
}

void print_device_hints(int card) {
    void **hints;
    printf("Print device hints for card %d\n", card);
    for (int i = 0; i < NUM_IFACES; ++i){
        int err = snd_device_name_hint(card, ifaces[i], &hints);
        if (err == 0){
            void **hints_iterator = hints;
            char *name;
            char *desc;
            char *ioid;
            while (*hints_iterator != NULL) {
                name = snd_device_name_get_hint(*hints_iterator, "NAME");
                desc = snd_device_name_get_hint(*hints_iterator, "DESC");
                ioid = snd_device_name_get_hint(*hints_iterator, "IOID");
                printf("\nName: %s\niface: %s\nDesc: %s\nioid: %s\n", name, ifaces[i], desc, ioid);
                free(name);
                free(desc);
                free(ioid);
                hints_iterator++;
            }
            snd_device_name_free_hint(hints);
        }
    }
}
