#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdbool.h>


const int SUITE_SPADES   = 0x000;
const int SUITE_HEARTS   = 0x080;
const int SUITE_CLUBS    = 0x100;
const int SUITE_DIAMONDS = 0x180;
const int RES_DRAW       = 0;
const int RES_WIN_A      = 1;
const int RES_WIN_B      = -1;
const int RES_INVALID    = 2;

#define SPADES(X)        ((X) | SUITE_SPADES)
#define HEARTS(X)        ((X) | SUITE_HEARTS)
#define CLUBS(X)         ((X) | SUITE_CLUBS)
#define DIAMONDS(X)      ((X) | SUITE_DIAMONDS)
#define HAND_SIZE 5

#endif /* __PROGTEST__ */

#define HAND_SIZE 5
#define SUITE_MASK 0x180
#define MAX_HANDS_FROM_DECK 10

int getCardValue(int card);
const char* getSuit(int card);
int determineBestHand(const int hand[]);

int getCardValue(int card) {
    int suit = card & SUITE_MASK;
    int rank = card - suit;

    if (rank >= '2' && rank <= '9') {
        return (rank - '0') + 1;
    }
    switch (rank) {
        case 'X': return 11;
        case 'J': return 12;
        case 'Q': return 13;
        case 'K': return 14;
        case 'A': return 15;
        default:
            return RES_INVALID;
    }
}

const char* getSuit(int card) {
    int suit = card & SUITE_MASK;
    if (suit == 0x000) return "Spades";
    if (suit == 0x080) return "Hearts";
    if (suit == 0x100) return "Clubs";
    if (suit == 0x180) return "Diamonds";
    return "Unknown";
}

int cardExists(int card, int handsFromDeck[], int *deckSize) {
    for (int i = 0; i < *deckSize; i++) {
        if (handsFromDeck[i] == card) {
            return 1; 
        }
    }
    handsFromDeck[(*deckSize)++] = card; 
    return 0;
}

int doubleChecker(const int hand1[], const int hand2[]){
   int handsFromDeck[MAX_HANDS_FROM_DECK] = {};
    int deckSize = 0;

    for (int i = 0; i < HAND_SIZE; i++) {
        if (cardExists(hand1[i], handsFromDeck, &deckSize) || cardExists(hand2[i], handsFromDeck, &deckSize)) {
            return RES_INVALID;
        }
    } 
    return 0;
}

int determineBestHand(const int hand[]) {
    int valueCount[16] = {0}; 
    bool isFlush = true;
    bool isStraight = false;
    int consecutiveCount = 0;
    int minRank = 15, maxRank = 3;

    const char* firstSuit = getSuit(hand[0]);
    printf("Debug: First suit of hand is %s\n", firstSuit);

    for (int i = 0; i < HAND_SIZE; i++) {
        int value = getCardValue(hand[i]);
        if (value == RES_INVALID) {
            printf("Debug: Invalid card value encountered\n");
            return RES_INVALID;
        } else if (value != -1) {
            valueCount[value]++;
            if (value < minRank) minRank = value;
            if (value > maxRank) maxRank = value;
        }
        
        if (getSuit(hand[i]) != firstSuit) {
            isFlush = false;
        }
    }

    printf("Debug: Value counts: ");
    for (int i = 3; i <= 15; i++) {
        printf("%d ", valueCount[i]);
    }
    printf("\n");

    for (int i = minRank; i <= maxRank; i++) {
        if (valueCount[i] > 0) {
            consecutiveCount++;
            if (consecutiveCount == 5) {
                isStraight = true;
                break;
            }
        } else {
            consecutiveCount = 0;
        }
    }

    bool fourOfAKind = false, threeOfAKind = false, pair = false, twoPair = false;
    int pairsCount = 0;

    for (int i = 3; i <= 15; i++) {
        if (valueCount[i] == 4) {
            fourOfAKind = true;
        } else if (valueCount[i] == 3) {
            threeOfAKind = true;
        } else if (valueCount[i] == 2) {
            pairsCount++;
        }
    }

    if (pairsCount == 1) pair = true;
    if (pairsCount == 2) twoPair = true;

    printf("Debug: Hand properties - isFlush: %d, isStraight: %d, fourOfAKind: %d, threeOfAKind: %d, pair: %d, twoPair: %d\n",
           isFlush, isStraight, fourOfAKind, threeOfAKind, pair, twoPair);

    if (isStraight && isFlush) {
        return 3;  
    } else if (fourOfAKind) {
        return 4;  
    } else if (threeOfAKind && pair) {
        return 5; 
    } else if (isFlush) {
        return 6; 
    } else if (isStraight) {
        return 7;
    } else if (threeOfAKind) {
        return 8; 
    } else if (twoPair) {
        return 9;
    } else if (pair) {
        return 10; 
    } else {
        return 11; 
    }
}

int getHighestCard(const int hand[]) {
    int maxCard = hand[0];
    int maxCardValue = getCardValue(maxCard);  

    for (int i = 1; i < HAND_SIZE; i++) {
        int currentCardValue = getCardValue(hand[i]);
        if (currentCardValue > maxCardValue) {
            maxCard = hand[i];
            maxCardValue = currentCardValue; 
        }
    }
    return maxCard;
}

int getHandSuit(const int hand[]) {
    return hand[0] & 0xF0;
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high) {
    int pivotValue = getCardValue(arr[high]);
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (getCardValue(arr[j]) > pivotValue) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1); 
        quickSort(arr, pi + 1, high);
    }
}

void sortHand(const int hand[], int sortedHand[]) {
    for (int i = 0; i < HAND_SIZE; i++) {
        sortedHand[i] = hand[i];
    }
    quickSort(sortedHand, 0, HAND_SIZE - 1);
}

int tightComparrasion(const int hand1[], const int hand2[], int type) {
    int highestCard1, highestCard2, suit1, suit2;
    int sortedHand1[HAND_SIZE];
    int sortedHand2[HAND_SIZE];
    
    sortHand(hand1, sortedHand1);
    sortHand(hand2, sortedHand2);

    int value1 = 0;
    int value2 = 0;

    switch (type) {
        case 3: 
            highestCard1 = getHighestCard(hand1);
            highestCard2 = getHighestCard(hand2);
            suit1 = getHandSuit(hand1);
            suit2 = getHandSuit(hand2);

            printf("Debug: Case 3 - Highest cards: Hand1 = %d, Hand2 = %d\n", highestCard1, highestCard2);
            printf("Debug: Case 3 - Suits: Hand1 = %d, Hand2 = %d\n", suit1, suit2);

            int adjustedValue1 = highestCard1 - suit1;
            int adjustedValue2 = highestCard2 - suit2;

            printf("Debug: Adjusted values - Hand1: %d, Hand2: %d\n", adjustedValue1, adjustedValue2);

            if (adjustedValue1 == adjustedValue2) {
                printf("Debug: Adjusted values are equal. Returning RES_DRAW\n");
                return RES_DRAW;
            } else {
                printf("Debug: Highest cards comparison result: %s\n", (adjustedValue1 > adjustedValue2) ? "RES_WIN_A" : "RES_WIN_B");
                return (adjustedValue1 > adjustedValue2) ? RES_WIN_A : RES_WIN_B;
            }
        case 4: {
            int valueCounts1[16] = {0}, valueCounts2[16] = {0};
            int fourKind1 = 0, fourKind2 = 0;

            for (int i = 0; i < HAND_SIZE; i++) {
                valueCounts1[getCardValue(hand1[i])]++;
                valueCounts2[getCardValue(hand2[i])]++;
            }

            for (int v = 15; v >= 3; v--) {
                if (valueCounts1[v] == 4) fourKind1 = v;
                if (valueCounts2[v] == 4) fourKind2 = v;
            }

            if (fourKind1 != fourKind2) return (fourKind1 > fourKind2) ? RES_WIN_A : RES_WIN_B;
            return RES_INVALID;
        }

        case 5: {
            int valueCounts1[16] = {0}, valueCounts2[16] = {0};
            int threeKind1 = 0, pair1 = 0;
            int threeKind2 = 0, pair2 = 0;

            for (int i = 0; i < HAND_SIZE; i++) {
                valueCounts1[getCardValue(hand1[i])]++;
                valueCounts2[getCardValue(hand2[i])]++;
            }

            for (int v = 15; v >= 3; v--) {
                if (valueCounts1[v] == 3) threeKind1 = v;
                if (valueCounts2[v] == 3) threeKind2 = v;
                if (valueCounts1[v] == 2) pair1 = v;
                if (valueCounts2[v] == 2) pair2 = v;
            }

            if (threeKind1 != threeKind2) return (threeKind1 > threeKind2) ? RES_WIN_A : RES_WIN_B;
            if (pair1 != pair2) return (pair1 > pair2) ? RES_WIN_A : RES_WIN_B;
            return RES_INVALID;
        }

        case 6: { 
            for (int i = 0; i < HAND_SIZE; i++) {
                int value1 = getCardValue(sortedHand1[i]);
                int value2 = getCardValue(sortedHand2[i]);
                if (value1 != value2) return (value1 > value2) ? RES_WIN_A : RES_WIN_B;
            }
            return RES_DRAW;
        }

        case 7:
            highestCard1 = getHighestCard(hand1);
            highestCard2 = getHighestCard(hand2);
            value1 = getCardValue(highestCard1);
            value2 = getCardValue(highestCard2);

            // Debug: Print highest cards and their values for both hands
            printf("Debug: Case 7 - Highest card in Hand1: %d, Value: %d\n", highestCard1, value1);
            printf("Debug: Case 7 - Highest card in Hand2: %d, Value: %d\n", highestCard2, value2);

            if (value1 == value2) {
                printf("Debug: Values are equal. Returning RES_DRAW\n");
                return RES_DRAW;
            } else {
                printf("Debug: Comparison result: %s\n", (value1 > value2) ? "RES_WIN_A" : "RES_WIN_B");
                return (value1 > value2) ? RES_WIN_A : RES_WIN_B;
            }
        case 8: { 
            int valueCounts1[16] = {0}, valueCounts2[16] = {0};
            int threeKind1 = 0, threeKind2 = 0;

            for (int i = 0; i < HAND_SIZE; i++) {
                valueCounts1[getCardValue(hand1[i])]++;
                valueCounts2[getCardValue(hand2[i])]++;
            }

            for (int v = 15; v >= 3; v--) {
                if (valueCounts1[v] == 3) threeKind1 = v;
                if (valueCounts2[v] == 3) threeKind2 = v;
            }

            if (threeKind1 != threeKind2) return (threeKind1 > threeKind2) ? RES_WIN_A : RES_WIN_B;
            return RES_INVALID;
        }

        case 9: {
            int valueCounts1[16] = {0}, valueCounts2[16] = {0};
            int pair1A = 0, pair2A = 0, kickerA = 0;
            int pair1B = 0, pair2B = 0, kickerB = 0;

            for (int i = 0; i < HAND_SIZE; i++) {
                valueCounts1[getCardValue(hand1[i])]++;
                valueCounts2[getCardValue(hand2[i])]++;
            }

            for (int v = 15; v >= 3; v--) {
                if (valueCounts1[v] == 2) {
                    if (pair1A == 0) pair1A = v;
                    else pair2A = v;
                } else if (valueCounts1[v] == 1) kickerA = v;

                if (valueCounts2[v] == 2) {
                    if (pair1B == 0) pair1B = v;
                    else pair2B = v;
                } else if (valueCounts2[v] == 1) kickerB = v;
            }

            if (pair1A != pair1B) return (pair1A > pair1B) ? RES_WIN_A : RES_WIN_B;
            if (pair2A != pair2B) return (pair2A > pair2B) ? RES_WIN_A : RES_WIN_B;
            if (kickerA != kickerB) return (kickerA > kickerB) ? RES_WIN_A : RES_WIN_B;
            return RES_DRAW;
        }

        case 10: { 
            int pairValue1 = 0, pairValue2 = 0;
            int valueCounts1[16] = {0}, valueCounts2[16] = {0};

            // Count values in both hands
            for (int i = 0; i < HAND_SIZE; i++) {
                valueCounts1[getCardValue(hand1[i])]++;
                valueCounts2[getCardValue(hand2[i])]++;
            }

            // Debug: Print value counts for each hand
            printf("Debug: Value counts for hand1: ");
            for (int v = 15; v >= 3; v--) {
                printf("%d ", valueCounts1[v]);
            }
            printf("\nDebug: Value counts for hand2: ");
            for (int v = 15; v >= 3; v--) {
                printf("%d ", valueCounts2[v]);
            }
            printf("\n");

            // Identify the pair values
            for (int v = 15; v >= 3; v--) {
                if (valueCounts1[v] == 2) pairValue1 = v;
                if (valueCounts2[v] == 2) pairValue2 = v;
            }

            // Debug: Print identified pair values
            printf("Debug: Pair value for hand1: %d\n", pairValue1);
            printf("Debug: Pair value for hand2: %d\n", pairValue2);

            // Compare the pair values
            if (pairValue1 != pairValue2) {
                printf("Debug: Pair values are different. Returning %s\n", (pairValue1 > pairValue2) ? "RES_WIN_A" : "RES_WIN_B");
                return (pairValue1 > pairValue2) ? RES_WIN_A : RES_WIN_B;
            }

            // Compare remaining cards if pairs are the same
            for (int i = HAND_SIZE - 1; i >= 0; i--) {
                int value1 = getCardValue(sortedHand1[i]);
                int value2 = getCardValue(sortedHand2[i]);
                
                // Skip if card is part of the pair
                if (value1 != pairValue1 && value2 != pairValue2) {
                    // Debug: Print card values being compared
                    printf("Debug: Comparing non-pair cards - Hand1: %d, Hand2: %d\n", value1, value2);
                    if (value1 != value2) {
                        printf("Debug: Non-pair card values are different. Returning %s\n", (value1 > value2) ? "RES_WIN_A" : "RES_WIN_B");
                        return (value1 > value2) ? RES_WIN_A : RES_WIN_B;
                    }
                }
            }
            printf("Debug: Hands are a draw. Returning RES_DRAW\n");
            return RES_DRAW;
        }

        default: 
            for (int i = 0; i < HAND_SIZE; i++) {
                int value1 = getCardValue(sortedHand1[i]);
                int value2 = getCardValue(sortedHand2[i]);
                if (value1 != value2) return (value1 > value2) ? RES_WIN_A : RES_WIN_B;
            }
            return RES_DRAW;
    }
}


int comparePokerHands(const int hand1[], const int hand2[]) {
    if (doubleChecker(hand1, hand2) == RES_INVALID) {
        printf("Debug: Invalid hand due to doubleChecker\n");
        return RES_INVALID;
    }

    int a = determineBestHand(hand1);
    int b = determineBestHand(hand2);
    if (a == RES_INVALID || b == RES_INVALID) {
        printf("Debug: One of the hands is invalid\n");
        return RES_INVALID;
    }

    printf("Debug: Hand strengths - Hand1: %d, Hand2: %d\n", a, b);

    if (a < b) {
        printf("A WON\n");
        return RES_WIN_A;
    } else if (b < a) {
        printf("B WON\n");
        return RES_WIN_B;
    } else if (a == b) {
        int result = tightComparrasion(hand1, hand2, a);
        switch (result) {
            case RES_WIN_A: printf("A WON\n"); return RES_WIN_A;
            case RES_WIN_B: printf("B WON\n"); return RES_WIN_B;
            case RES_DRAW: printf("DRAW\n"); return RES_DRAW;
            case RES_INVALID: printf("Debug: Comparison resulted in INVALID\n"); return RES_INVALID;
        }
    }
    return RES_DRAW; 
}

#ifndef __PROGTEST__
int main ()
{

int handA1[] = { CLUBS('A'), DIAMONDS('K'), HEARTS('Q'), DIAMONDS('J'), CLUBS('9') }; // High card: A, kickers: K, Q, J, 9
int handB1[] = { HEARTS('K'), DIAMONDS('Q'), SPADES('J'), CLUBS('X'), DIAMONDS('8') }; // High card: K, kickers: Q, J, 10, 8
assert(comparePokerHands(handA1, handB1) == RES_WIN_A);

int handA2[] = { CLUBS('K'), DIAMONDS('Q'), HEARTS('J'), SPADES('X'), CLUBS('8') }; // High card: K, kickers: Q, J, 10, 8
int handB2[] = { HEARTS('A'), DIAMONDS('K'), SPADES('Q'), CLUBS('J'), DIAMONDS('9') }; // High card: A, kickers: K, Q, J, 9
assert(comparePokerHands(handA2, handB2) == RES_WIN_B);

int handA3[] = { CLUBS('A'), DIAMONDS('K'), HEARTS('Q'), SPADES('J'), CLUBS('X') }; // High card: A, kickers: K, Q, J, 10
int handB3[] = { HEARTS('A'), SPADES('K'), SPADES('Q'), CLUBS('J'), DIAMONDS('9') }; // High card: A, kickers: K, Q, J, 9
assert(comparePokerHands(handA3, handB3) == RES_WIN_A);

int handA4[] = { CLUBS('A'), DIAMONDS('K'), HEARTS('Q'), SPADES('J'), CLUBS('8') }; // High card: A, kickers: K, Q, J, 8
int handB4[] = { HEARTS('A'), SPADES('K'), SPADES('Q'), CLUBS('J'), DIAMONDS('X') }; // High card: A, kickers: K, Q, J, 10
assert(comparePokerHands(handA4, handB4) == RES_WIN_B);

int handA5[] = { CLUBS('A'), DIAMONDS('K'), HEARTS('Q'), SPADES('J'), CLUBS('9') }; // High card: A, kickers: K, Q, J, 9
int handB5[] = { HEARTS('A'), SPADES('K'), DIAMONDS('Q'), CLUBS('J'), DIAMONDS('9') }; // Identical high card and kickers
assert(comparePokerHands(handA5, handB5) == RES_DRAW);

int handA6[] = { CLUBS('A'), DIAMONDS('A'), HEARTS('Q'), SPADES('J'), CLUBS('9') }; // Invalid due to two Aces
int handB6[] = { HEARTS('K'), DIAMONDS('Q'), SPADES('J'), CLUBS('X'), DIAMONDS('8') }; // High card: K
assert(comparePokerHands(handA6, handB6) == RES_INVALID);

int handA7[] = { CLUBS('A'), DIAMONDS('K'), HEARTS('Q'), SPADES('J'), CLUBS('9') }; // High card: A
int handB7[] = { HEARTS('K'), DIAMONDS('K'), SPADES('Q'), CLUBS('Q'), DIAMONDS('8') }; // Invalid due to two Kings and two Queens
assert(comparePokerHands(handA7, handB7) == RES_INVALID);

int handA8[] = { CLUBS('h'), DIAMONDS('K'), HEARTS('Q'), SPADES('J'), CLUBS('9') }; // Invalid due to 'h'
int handB8[] = { HEARTS('K'), DIAMONDS('Q'), SPADES('J'), CLUBS('X'), DIAMONDS('8') }; // High card: K
assert(comparePokerHands(handA8, handB8) == RES_INVALID);

int handA9[] = { CLUBS('A'), DIAMONDS('K'), HEARTS('Q'), SPADES('J'), CLUBS('9') }; // High card: A
int handB9[] = { HEARTS('K'), DIAMONDS('h'), SPADES('Q'), CLUBS('X'), DIAMONDS('8') }; // Invalid due to 'h'
assert(comparePokerHands(handA9, handB9) == RES_INVALID);

printf("PASSEDALL\n");
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */



//   int x10[] = { CLUBS('A'), CLUBS('2'), CLUBS('3'), CLUBS('4'), CLUBS('5') };
//   int y10[] = { HEARTS('J'), CLUBS('J'), SPADES('J'), HEARTS('2'), SPADES('2') };
//   assert ( comparePokerHands ( x10, y10 ) == RES_WIN_B );

//   int x11[] = { CLUBS('A'), CLUBS('K'), CLUBS('Q'), CLUBS('J'), CLUBS('X') };
//   int y11[] = { HEARTS('3'), CLUBS('3'), SPADES('3'), HEARTS('2'), SPADES('2') };
//   assert ( comparePokerHands ( x11, y11 ) == RES_WIN_A );

//   int x12[] = { CLUBS('A'), HEARTS('A'), CLUBS('Q'), HEARTS('Q'), CLUBS('J') };
//   int y12[] = { SPADES('A'), DIAMONDS('A'), SPADES('Q'), DIAMONDS('Q'), SPADES('J') };
//   assert ( comparePokerHands ( x12, y12 ) == RES_DRAW );

//   int x13[] = { DIAMONDS('A'), HEARTS('5'), SPADES('4'), DIAMONDS('5'), CLUBS('4') };
//   int y13[] = { DIAMONDS('4'), DIAMONDS('K'), CLUBS('5'), HEARTS('5'), HEARTS('4') };
//   assert ( comparePokerHands ( x13, y13 ) == RES_INVALID );

//   int x14[] = { DIAMONDS('A'), HEARTS('Z'), SPADES('4'), DIAMONDS('5'), CLUBS('4') };
//   int y14[] = { DIAMONDS('4'), DIAMONDS('K'), CLUBS('5'), SPADES('5'), HEARTS('4') };
//   assert ( comparePokerHands ( x14, y14 ) == RES_INVALID );

//  // Test high card with different suits
// int x15[] = { SPADES('A'), HEARTS('K'), DIAMONDS('Q'), CLUBS('J'), SPADES('9') };
// int y15[] = { CLUBS('A'), DIAMONDS('K'), HEARTS('Q'), SPADES('J'), DIAMONDS('8') };
// assert(comparePokerHands(x15, y15) == RES_WIN_A);

// // Test straight flush vs. flush
// int x16[] = { CLUBS('A'), CLUBS('K'), CLUBS('Q'), CLUBS('J'), CLUBS('X') };  // Straight flush
// int y16[] = { HEARTS('A'), HEARTS('K'), HEARTS('9'), HEARTS('8'), HEARTS('7') };  // Flush
// assert(comparePokerHands(x16, y16) == RES_WIN_A);

// // Test four of a kind vs. full house
// int x17[] = { SPADES('3'), DIAMONDS('3'), HEARTS('3'), CLUBS('3'), SPADES('2') };  // Four of a kind
// int y17[] = { CLUBS('K'), DIAMONDS('K'), HEARTS('K'), SPADES('A'), DIAMONDS('A') };  // Full house
// assert(comparePokerHands(x17, y17) == RES_WIN_A);

// // Test two pairs vs. pair
// int x18[] = { CLUBS('X'), DIAMONDS('X'), SPADES('5'), HEARTS('5'), DIAMONDS('2') };  // Two pairs
// int y18[] = { HEARTS('K'), DIAMONDS('K'), SPADES('9'), CLUBS('8'), SPADES('4') };  // Pair
// assert(comparePokerHands(x18, y18) == RES_WIN_A);

// // Test highest card when two pairs are equal
// int x19[] = { SPADES('K'), HEARTS('K'), DIAMONDS('Q'), CLUBS('Q'), DIAMONDS('X') };
// int y19[] = { CLUBS('K'), DIAMONDS('K'), SPADES('Q'), HEARTS('Q'), HEARTS('9') };
// assert(comparePokerHands(x19, y19) == RES_WIN_A);

// // Test two straights with different high cards
// int x20[] = { DIAMONDS('6'), SPADES('5'), HEARTS('4'), CLUBS('3'), SPADES('2') };  // Straight with 6-high
// int y20[] = { HEARTS('5'), DIAMONDS('4'), CLUBS('3'), SPADES('2'), HEARTS('A') };  // Straight with 5-high
// assert(comparePokerHands(x20, y20) == RES_INVALID);

// // Test pair vs. high card
// int x21[] = { CLUBS('9'), DIAMONDS('9'), HEARTS('K'), SPADES('J'), DIAMONDS('3') };  // Pair
// int y21[] = { CLUBS('A'), DIAMONDS('K'), SPADES('Q'), HEARTS('X'), CLUBS('8') };  // High card
// assert(comparePokerHands(x21, y21) == RES_WIN_A);

// // Test straight flush vs. four of a kind
// int x22[] = { CLUBS('6'), CLUBS('5'), CLUBS('4'), CLUBS('3'), CLUBS('2') };  // Straight flush
// int y22[] = { DIAMONDS('J'), HEARTS('J'), SPADES('J'), CLUBS('J'), DIAMONDS('7') };  // Four of a kind
// assert(comparePokerHands(x22, y22) == RES_WIN_A);

// // Test for invalid hand with duplicate cards
// int x23[] = { SPADES('A'), SPADES('A'), DIAMONDS('3'), HEARTS('4'), CLUBS('5') };
// int y23[] = { CLUBS('7'), DIAMONDS('8'), HEARTS('9'), SPADES('X'), HEARTS('J') };
// assert(comparePokerHands(x23, y23) == RES_INVALID);

// // Test for invalid hand with invalid rank
// int x24[] = { CLUBS('A'), HEARTS('A'), DIAMONDS('X'), CLUBS('Z'), SPADES('5') };
// int y24[] = { CLUBS('7'), DIAMONDS('8'), HEARTS('9'), SPADES('X'), HEARTS('J') };
// assert(comparePokerHands(x24, y24) == RES_INVALID);

// // Test full house vs. flush
// int x25[] = { CLUBS('8'), DIAMONDS('8'), SPADES('8'), CLUBS('K'), DIAMONDS('K') };  // Full house
// int y25[] = { HEARTS('A'), HEARTS('K'), HEARTS('Q'), HEARTS('J'), HEARTS('9') };  // Flush
// assert(comparePokerHands(x25, y25) == RES_WIN_A);

// // Test invalid hand with character 'Z'
// int x26[] = { CLUBS('A'), HEARTS('A'), DIAMONDS('X'), CLUBS('X'), SPADES('5') };
// int y26[] = { CLUBS('7'), DIAMONDS('8'), HEARTS('9'), SPADES('Z'), HEARTS('J') };
// assert(comparePokerHands(x26, y26) == RES_INVALID);

// // Test high card hands with all same values
// int x27[] = { CLUBS('9'), DIAMONDS('8'), HEARTS('7'), SPADES('6'), CLUBS('5') };
// int y27[] = { DIAMONDS('9'), HEARTS('8'), SPADES('7'), CLUBS('6'), HEARTS('5') };
// assert(comparePokerHands(x27, y27) == RES_DRAW);

// // Test ace-high straight flush vs. king-high straight flush
// int x28[] = { CLUBS('A'), CLUBS('K'), CLUBS('Q'), CLUBS('J'), CLUBS('X') };  // Ace-high
// int y28[] = { HEARTS('K'), HEARTS('Q'), HEARTS('J'), HEARTS('X'), HEARTS('9') };  // King-high
// assert(comparePokerHands(x28, y28) == RES_WIN_A);

// // Test for invalid hands with empty slots
// int x29[] = { CLUBS('A'), HEARTS('A'), DIAMONDS('X'), 0, 0 };
// int y29[] = { CLUBS('7'), DIAMONDS('8'), HEARTS('9'), SPADES('X'), HEARTS('J') };
// assert(comparePokerHands(x29, y29) == RES_INVALID);

// // Test flush with high card tie, lower secondary cards
// int x30[] = { HEARTS('K'), HEARTS('X'), HEARTS('8'), HEARTS('6'), HEARTS('4') };
// int y30[] = { DIAMONDS('K'), DIAMONDS('9'), DIAMONDS('8'), DIAMONDS('6'), DIAMONDS('5') };
// assert(comparePokerHands(x30, y30) == RES_WIN_A);

// // Test identical straights for draw
// int x31[] = { CLUBS('6'), DIAMONDS('5'), HEARTS('4'), SPADES('3'), HEARTS('2') };
// int y31[] = { HEARTS('6'), SPADES('5'), CLUBS('4'), DIAMONDS('3'), SPADES('2') };
// assert(comparePokerHands(x31, y31) == RES_DRAW);

// // Test full house tie with different kickers
// int x32[] = { SPADES('Q'), HEARTS('Q'), CLUBS('Q'), DIAMONDS('J'), HEARTS('J') };
// int y32[] = { SPADES('K'), HEARTS('K'), DIAMONDS('K'), DIAMONDS('X'), HEARTS('X') };
// assert(comparePokerHands(x32, y32) == RES_WIN_B);

// // Test straight flush vs. straight
// int x33[] = { HEARTS('7'), HEARTS('6'), HEARTS('5'), HEARTS('4'), HEARTS('3') };  // Straight flush
// int y33[] = { CLUBS('7'), DIAMONDS('6'), SPADES('5'), CLUBS('4'), SPADES('2') };  // Straight
// assert(comparePokerHands(x33, y33) == RES_WIN_A);

// // Test identical four of a kind with different kickers
// int x34[] = { CLUBS('9'), DIAMONDS('9'), HEARTS('9'), SPADES('9'), CLUBS('7') };
// int y34[] = { DIAMONDS('X'), HEARTS('X'), SPADES('X'), CLUBS('X'), CLUBS('6') };
// assert(comparePokerHands(x34, y34) == RES_WIN_B);
