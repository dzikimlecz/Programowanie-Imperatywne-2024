#include <stdio.h>
#include <stdlib.h>


#define NUM_RANKS 13
#define NUM_SUITS 4
#define NUM_CARDS 52
#define HAND_SIZE NUM_CARDS / 2

#define Card int


typedef struct {
    int code;
    int hand1, hand2, conflicts;
    Card* cards2;
} Result;

Result war(int seed, int doWars, int maxConflicts);


int main() {
    int seed, noWars, conflicts, doWars;
    scanf("%d %d %d", &seed, &noWars, &conflicts);
    if (noWars == 0) {
        doWars = 1;
    } else if (noWars == 1){
        doWars = 0;
    } else {
        return 987654321;
    }
    Result result = war(seed, doWars, conflicts);
    switch (result.code) {
        case 0:
        case 1:
            printf("%d %d %d\n", result.code, result.hand1, result.hand2);
            break;
        case 2:
            printf("2 %d\n", result.conflicts);
            break;
        case 3:
            printf("3\n");
            Card* ptr = result.cards2;
            if (ptr == NULL) {
                return 76487234;
            }
            while(*ptr != -1) {
                printf("%d ", *ptr);
                ++ptr;
            }
            printf("\n");
            free(result.cards2);
            break;
        default:
            return 123456789;
    }
    return 0;
}




// Funkcja do tasowania talii kart
void shuffleDeck(Card* deck, int size) {
    for (int i = 0; i < size - 1; ++i) {
        int j = rand() % (size - i) + i;
        Card temp = deck[i];
        deck[i] = deck[j];
        deck[j] = temp;
    }
}

void initDeck(Card* deck, int size) {
    for (Card card = 0; card < NUM_CARDS; card++) {
       deck[card] = card;
    }
}

void dealCards(Card cards[NUM_CARDS], Card player1[NUM_CARDS], Card player2[NUM_CARDS]) {
    for (int i = 0; i < HAND_SIZE; i++) {
        player1[i] = cards[i];
        player2[i] = cards[HAND_SIZE + i];
    }
}

// Funkcja do porównywania kart
int compareCards(Card card1, Card card2) {
    return (card1 / NUM_SUITS) - (card2 / NUM_SUITS);
}


Card takeCard(Card cards[NUM_CARDS], int* ix, int* size) {
    if (*size) {
        --(*size);
        Card val = cards[*ix];
        ++(*ix);
        *ix %= NUM_CARDS;
        return val;
    }
    return -1;
}

int pushCard(Card cards[NUM_CARDS], Card c, int* ix, int* size) {
    if (*size >= NUM_CARDS) {
        return 0;
    }
    if (*size == 0) {
        *ix = 0;
        cards[0] = c;
        *size = 1;
    } else {
        cards[(*ix + *size) % NUM_CARDS] = c;
        ++(*size);
    }
    return 1;
}

void collectCards(
    Card hand[NUM_CARDS],
    int* handStart,
    int* handSize,
    Card stack[NUM_CARDS],
    int stackSize
) {
     for (int i = 0; i < stackSize; ++i) {
         pushCard(hand, stack[i], handStart, handSize);
     }
}


Result war(int seed, int doWars, int maxConflicts) {
    // Inicjalizacja talii kart
    Card deck[NUM_CARDS];
    initDeck(deck, NUM_CARDS);

    // Tasowanie talii kart
    srand(seed);
    shuffleDeck(deck, NUM_CARDS);

    // Deklaracja graczy
    Card player1[NUM_CARDS] = {};
    Card player2[NUM_CARDS] = {};

    // Rozdanie kart graczom
    dealCards(deck, player1, player2);

    // Rozmiary ręki każdego z graczy
    int hand1, hand2;
    hand1 = hand2 = HAND_SIZE;

    // Początek ręki każdego z graczy
    int hand1Start, hand2Start;
    hand1Start = hand2Start = 0;

    Result result;
    result.code = -1;
    result.cards2 = NULL;

    // Konflikty
    int conflicts = maxConflicts;
    int gameOngoing = conflicts;
    Card c1[NUM_CARDS] = {};
    Card c2[NUM_CARDS] = {};
    int c1top, c2top;

    while (gameOngoing) {
        c1top = c2top = 0;
        int unresolved = 1;
        while (gameOngoing && unresolved) {
            c1[c1top++] = takeCard(player1, &hand1Start, &hand1);
            c2[c2top++] = takeCard(player2, &hand2Start, &hand2);
            int conf = compareCards(c1[c1top - 1], c2[c2top - 1]);
            --conflicts;
            if (conf > 0) {
                // p2 < p1
                unresolved = 0;
                collectCards(player1, &hand1Start, &hand1, c1, c1top);
                collectCards(player1, &hand1Start, &hand1, c2, c2top);
            } else if (conf < 0) {
                // p1 < p2
                unresolved = 0;
                collectCards(player2, &hand2Start, &hand2, c2, c2top);
                collectCards(player2, &hand2Start, &hand2, c1, c1top);
            } else if (conflicts) {
                // war
                if (doWars) {
                    c1[c1top++] = takeCard(player1, &hand1Start, &hand1);
                    c2[c2top++] = takeCard(player2, &hand2Start, &hand2);
                    if (c1[c1top - 1] == -1 || c2[c2top - 1] == -1) {
                        gameOngoing = 0;
                        result.code = 1;
                        result.hand1 = hand1 + c1top;
                        result.hand2 = hand2 + c2top;
                    }
                } else {
                    unresolved = 0;
                    collectCards(player1, &hand1Start, &hand1, c1, c1top);
                    collectCards(player2, &hand2Start, &hand2, c2, c2top);
                }
            } else {
                // nie starczyło konfliktów.
                gameOngoing = 0;
                result.code = 0;
                result.hand1 = hand1 + c1top;
                result.hand2 = hand2 + c2top;
            }
            if (!hand1 || !hand2) {
                gameOngoing = 0;
                if (!unresolved) {
                    if (hand1) {
                        result.code = 2;
                        result.conflicts = maxConflicts - conflicts;
                    } else if (hand2) {
                        result.code = 3;
                        result.cards2 = (Card*) malloc(sizeof(Card) * (hand2 + 1));
                        for (int i = 0; i < hand2; ++i) {
                            result.cards2[i] = player2[(hand2Start + i) % NUM_CARDS];
                        }
                        result.cards2[hand2] = -1;
                    }
                } else {
                    gameOngoing = 0;
                    result.code = 1;
                    result.hand1 = hand1 + c1top;
                    result.hand2 = hand2 + c2top;
                }
            }
        }
    }
    return result;
}
