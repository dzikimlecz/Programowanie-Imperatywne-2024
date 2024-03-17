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

typedef struct {
    Card cards[NUM_CARDS];
    int start;
    int size;
} Hand;


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


Card takeCard(Hand* hand) {
    if (hand->size) {
        --hand->size;
        Card val = hand->cards[hand->start];
        hand->start++;
        hand->start %= NUM_CARDS;
        return val;
    }
    return -1;
}

int pushCard(Hand* hand, Card card) {
    if (hand->size >= NUM_CARDS) {
        return 0;
    }
    if (hand->size) {
        hand->cards[(hand->start + hand->size) % NUM_CARDS] = card;
        ++hand->size;
    } else {
        hand->start = 0;
        hand->cards[0] = card;
        hand->size = 1;
    }
    return 1;
}

void collectCards(
    Hand* hand,
    Card stack[NUM_CARDS],
    int stackSize
) {
     for (int i = 0; i < stackSize; ++i) {
         pushCard(hand, stack[i]);
     }
}

void collectAllCards(
    Hand* hand,
    Card ownStack[NUM_CARDS],
    int ownStackSize,
    Card otherStack[NUM_CARDS],
    int otherStackSize
) {
    collectCards(hand, ownStack, ownStackSize);
    collectCards(hand, otherStack, otherStackSize);
}

Result war(int seed, int doWars, int maxConflicts) {
    // Inicjalizacja talii kart
    Card deck[NUM_CARDS];
    initDeck(deck, NUM_CARDS);

    // Tasowanie talii kart
    srand(seed);
    shuffleDeck(deck, NUM_CARDS);



    // Deklaracja graczy
    Hand handA, handB;

    // Rozdanie kart graczom
    dealCards(deck, handA.cards, handB.cards);

    // Rozmiary ręki każdego z graczy
    handA.size = handB.size = HAND_SIZE;

    // Początek ręki każdego z graczy
    handA.start = handB.start = 0;

    Result result;
    result.code = -1;
    result.cards2 = NULL;

    // Konflikty
    int conflicts = maxConflicts;
    int gameOngoing = conflicts;
    Card stackA[NUM_CARDS] = {};
    Card stackB[NUM_CARDS] = {};
    int stackAsize, stackBsize;

    while (gameOngoing) {
        stackAsize = stackBsize = 0;
        int unresolved = 1;
        while (gameOngoing && unresolved) {
            stackA[stackAsize++] = takeCard(&handA);
            stackB[stackBsize++] = takeCard(&handB);
            int conf = compareCards(stackA[stackAsize - 1], stackB[stackBsize - 1]);
            --conflicts;
            if (conf > 0) {
                // p2 < p1
                unresolved = 0;
                collectAllCards(&handA, stackA, stackAsize, stackB, stackBsize);
            } else if (conf < 0) {
                // p1 < p2
                unresolved = 0;
                collectAllCards(&handB, stackB, stackBsize, stackA, stackAsize);
            } else if (conflicts) {
                // war
                if (doWars) {
                    stackA[stackAsize++] = takeCard(&handA);
                    stackB[stackBsize++] = takeCard(&handB);
                    if (stackA[stackAsize - 1] == -1 || stackB[stackBsize - 1] == -1) {
                        gameOngoing = 0;
                        result.code = 1;
                        result.hand1 = handA.size + stackAsize;
                        result.hand2 = handB.size + stackBsize;
                    }
                } else {
                    unresolved = 0;
                    collectCards(&handA, stackA, stackAsize);
                    collectCards(&handB, stackB, stackBsize);
                }
            } else {
                // nie starczyło konfliktów.
                gameOngoing = 0;
                result.code = 0;
                result.hand1 = handA.size + stackAsize;
                result.hand2 = handB.size + stackBsize;
            }
            if (!handA.size || !handB.size) {
                gameOngoing = 0;
                if (!unresolved) {
                    if (handA.size) {
                        result.code = 2;
                        result.conflicts = maxConflicts - conflicts;
                    } else if (handB.size) {
                        result.code = 3;
                        result.cards2 = (Card*) malloc(sizeof(Card) * (handB.size + 1));
                        for (int i = 0; i < handB.size; ++i) {
                            result.cards2[i] = handB.cards[(handB.start + i) % NUM_CARDS];
                        }
                        result.cards2[handB.size] = -1;
                    }
                } else {
                    gameOngoing = 0;
                    result.code = 1;
                    result.hand1 = handA.size + stackAsize;
                    result.hand2 = handB.size + stackBsize;
                }
            }
        }
    }
    return result;
}
