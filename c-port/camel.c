#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

float glob_s = 0.1;

size_t getline(char **lineptr, size_t *n, FILE *stream) {
    char *bufptr = NULL;
    char *p = bufptr;
    size_t size;
    int c;

    if (lineptr == NULL) {
        return -1;
    }
    if (stream == NULL) {
        return -1;
    }
    if (n == NULL) {
        return -1;
    }
    bufptr = *lineptr;
    size = *n;

    c = fgetc(stream);
    if (c == EOF) {
        return -1;
    }
    if (bufptr == NULL) {
        bufptr = malloc(128);
        if (bufptr == NULL) {
            return -1;
        }
        size = 128;
    }
    p = bufptr;
    while (c != EOF) {
        if ((p - bufptr) > (size - 1)) {
            size = size + 128;
            bufptr = realloc(bufptr, size);
            if (bufptr == NULL) {
                return -1;
            }
        }
        *p++ = c;
        if (c == '\n') {
            break;
        }
        c = fgetc(stream);
    }

    *p++ = '\0';
    *lineptr = bufptr;
    *n = size;

    return p - bufptr - 1;
}

struct gstats_t {
    int O;
    int P;
    int D;
    int p;
    int b;
    int A;
    int Z;
};

typedef struct gstats_t gstats;

float random() {
    glob_s = 1.9 * MIN(glob_s, 1 - glob_s);
    return glob_s;
}

void tab(int num) {
    int i;
    for (i = 0; i < num; i++) {
        printf(" ");
    }
    return;
}

int fnr(unsigned int p) {
    // return arc4random_uniform(p);
    return random() * p;
}

void ginit(gstats *state) {
    state->O = 4;
    state->P = 6;
    state->D = 0;
    state->p = 0;
    state->b = 0;
    state->A = 0;
    state->Z = 0;
}

void die() {
    printf("YOU DIED IN THE DESERT.\n");
    int val = fnr(10);
    if (val <= 1) {
        printf("THE NATIONAL CAMEL'S UNION IS NOT ATTENDING YOUR FUNERAL!!!\n");
    } else if (val <= 3) {
        printf("YOUR BODY WAS EATEN BY VULTURES AND IMPORTED CANNINBALS !!!\n");
    } else if (val <= 5) {
        printf("THE LOCAL SHEIK NOW USES YOUR SKULL FOR A CHANGE PURSE !!!\n");
    } else if (val <= 7) {
        printf("PEOPLE WITH LITTLE INTELLIGENCE SHOULD STAY OUT OF THE DESERT\n");
    } else {
        printf("TURKEYS SHOULD FLY, NOT RIDE CAMELS !!!!!!!\n");
    }
}

int berb(gstats *state) {
    printf("#7 ATTEMPT AN ESCAPE\n");
    printf("#8 WAIT FOR PAYMENT\n");
    printf("YOUR SUB-COMMAND ? \n");

    char *line = NULL;
    size_t linecap = 0;
    getline(&line, &linecap, stdin);

    int value = atoi(line);
    free(line);

    if (value == 8) {
        int val = fnr(100);
        if (val < 25) {
            printf("YOUR RANSOM HAS BEEN PAID AND YOU ARE FREE TO GO.\n");
            state->b = 0;
            return 0;
        }

        printf("THE LOCAL SULTAN IS COLLECTING.......JUST WAIT.......\n");
        return 0;
    }

    int val = fnr(10);
    if (val < 5) {
        printf("YOU WERE MORTALLY WOUNDED BY A PIG STABBER WHILE ESCAPING.\n");
        die();
        return 1;
    }

    printf("CONGRADULATIONS, YOU SUCCESSFULLY ESCAPED !!!!\n");
    state->b = 0;
    return 0;
}

void rd_RND(gstats *state) {
    int val = fnr(100);
    if (val <= 5) {
        printf("WILD BERBERS HIDDEN IN THE SAND HAVE CAPTURED YOU.\n");
        printf("LUCKILY tHE LOCAL SHEIK HAS AGREED TO THEIR RANSOM-\n");
        printf("DEMANDS.......BUT........WATCH FOR THE PYGMIES !!!\n");
        printf("YOU HAVE A NEW CHOICE OF SUB-COMMANDS\n");
        state->b = 1;
        return;
    }

    val = fnr(10);
    if (val <= 2) {
        printf("YOU HAVE ARRIVED AT AN OASES--------YOUR CAMEL IS\n");
        printf("FILLING yOUR CANTEEN AND EATING FIGS.\n");
        state->O = 4;
        state->P = 6;
        return;
    }

    val = fnr(100);
    if (val <= 5) {
        printf("YOU HAVE BEEN CAUGHT IN A SANDSTORM.....GOOD LUCK!\n");
        int distance = fnr(10);
        int direction = fnr(10);
        if (direction & 1) {
            state->D += distance;
        } else {
            state->D -= distance;
        }

        printf("YOUR NEW POSITION IS %d MILES SO FAR!\n", state->D);
        return;
    }

    val = fnr(100);
    if (val <= 5) {
        state->p++;
        printf("YOUR CAMEL HURT HIS HUMP\n");
        printf("LUCKILY tHE PYGMIES WERE FOOTWEARY !!!\n");
        return;
    }

    return;
}

void camel_dead() {
    printf("YOU DIRTY RAPSCALLION! YOU RAN YOUR POOR CAMEL TO DEATH !!\n");
    die();
}

void game() {
    gstats state;

    printf("GOOD LUCK AND GOOD CAMELING !!\n");
    printf("YOU ARE iN THE MIDDLE OF THE DESERT AT AN OASIS.\n");

    ginit(&state);

    while (1) {
        if (state.D > 199) {
            printf("YOU WIN, A PARTY IS BEING GIVEN IN YOUR HONOR.......\n");
            printf(".......THE PYGMIES ARE PLANNING TO ATTEND.......\n");
            return;
        }

        state.O--;
        if (state.O == 1) {
            printf("----------WARNING---------- GET A DRINK\n");
        }

        if (state.O < 0) {
            printf("YOU RAN OUT OF WATER...... SORRY CHUM!!!\n");
            return;
        }

        state.Z++;

        // pygmies are a bit faster than moderate camel speed
        int pygmy_advance = (fnr(100) + 25) / 10;

        // pygmies start after turn 3
        if (state.Z >= 4) {
            state.p += pygmy_advance;
        }

        // by source, the pygmies don't advance in Berber mode
        // by source, Berber mode is never set
        if (state.b) {
            if (berb(&state)) {
                return;
            }

            continue;
        }

        if (state.Z >= 4) {
            if (state.p > state.D) {
                printf("THE PYGMIES HAVE CAPTURED YOU.  CAMEL AND PEOPLE SOUP IS\n");
                printf("THEIR FAVORITE DISH !!!!!\n");
                return;
            }

            int separation = state.D - state.p;
            printf("THE PYGMIES ARE %d MILES BEHIND YOU.\n", separation);
        }

        printf("YOU HAVE TRAVELLED %d MILES ALLTOGETHER.\n", state.D);
        printf("WHAT IS YOUR COMMAND? ");

        char *line = NULL;
        size_t linecap = 0;
        getline(&line, &linecap, stdin);

        int value = atoi(line);
        free(line);

        // source didn't try to fix errors

        if (value == 1) {
            state.P--;
            if (state.P < 0) {
                die();
                return;
            }

            printf("BETTER WATCH FOR AN OASES !\n");
            state.O = 4 + 1; // +1 for the turn cost
            continue;
        } else if (value == 2) {
            state.A++;
            if (state.A == 8) {
                camel_dead();
                return;
            }

            // in source, game random events happened before movement
            int camel_advance = fnr(10);
            state.D += camel_advance;
            printf("YOUR CAMEL LIKES THIS PACE.\n");

            rd_RND(&state);
            continue;
        } else if (value == 3) {
            state.A += 3;
            if (state.A >= 8) {
                camel_dead();
                return;
            }

            // in source, game random events happened before movement
            int camel_advance = 2 * fnr(10);
            state.D += camel_advance;
            printf("YOUR CAMEL IS BURNING ACROSS THE DESERT SANDS.\n");

            rd_RND(&state);
            continue;
        } else if (value == 4) {
            printf("YOUR CAMEL THANKS YOU!\n");
            state.A = 0;
            continue;
        } else if (value == 5) {
            printf("Your camel has %d good days left.\n", 7 - state.A);
            printf("You have %d drinks left in your canteen.\n", state.P);
            printf("You can go %d commands without drinking.\n", state.O);
            // but pygmies still advance and water level decreases
            continue;
        } else {
            int chance = fnr(10);
            if (chance != 1) {
                die();
                return;
            }

            // only 10% chance to live
            printf("Help has found you in a state of unconsciousness.\n");
            state.P = 3;
            state.O = 4;
            continue;
        }
    }
}

int main() {
    char *line = NULL;
    size_t linecap = 0;

    tab(26);
    printf("CAMEL\n");
    tab(20);
    printf("CREATIVE COMPUTING\n");
    tab(18);
    printf("MORRISTOWN, NEW JERSEY\n");
    printf("\n\n\n");
    printf("WOULD YOU LIKE INSTRUCTIONS? ");

    int compare = 0;
    line = NULL;
    linecap = 0;
    getline(&line, &linecap, stdin);
    compare = strncasecmp(line, "n", 1);
    free(line);

    if (compare) {
        printf("\n");
        printf("   WELCOME TO CAMEL. THE QBJEST IS TO TRAVEL\n");
        printf("200 MILES ACROSS THE GREAT GOBI DESERT.\n");
        printf("A TRIBE OF KNOCK KNEED PIGMIES WILL BE CHASING YOU.\n");
        printf("YOU WILL BE ASKED FOR COMMANDS EVERY SO OFTEN.\n");
        printf("\n\n\n");
        printf("C O M M A N D S :\n");
        printf("#1 DRINK FROM YOUR CANTEEN\n");
        printf("#2 AHEAD MODERATE SPEED\n");
        printf("#3 AHEAD FULL SPEED\n");
        printf("#4 STOP FOR THE NIGHT\n");
        printf("#5 STATUS CHECK\n");
        printf("#6 HOPE FOR HELP\n");
        printf("\n\n\n\n\n");
        printf("YOU HAVE ONE QUART OF WATER WHICH WILL LAST YOU SIX DRINKS.\n");
        printf("YOU MAY RENEW YOUR WATER SUPPLY COMPLETELY AT AN OASES.\n");
        printf("YOU GET A HALF A QUART IF FOUND BY HELP.\n");
        printf("IF HELP DOES NOT FIND YOU AFTER COMMAND SIX, YOU LOSE.\n");
    }

    compare = 0;
    do {
        game();
        printf("\n\n");
        printf("WANT A NEW CAMEL AND A NEW GAME ?\n");
        line = NULL;
        linecap = 0;
        getline(&line, &linecap, stdin);
        compare = strncasecmp(line, "y", 1);
        free(line);
    } while (!compare);

    printf("-----------------\n");
    printf("     CHICKEN\n");
    printf("-----------------\n");

    return 0;
}
