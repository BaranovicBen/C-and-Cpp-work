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
    for (int i = 0; i < HAND_SIZE; i++) {
        int value = getCardValue(hand[i]);
        if (value == RES_INVALID) {
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

    switch (type) {
        case 3: {
            highestCard1 = getHighestCard(hand1);
            highestCard2 = getHighestCard(hand2);
            suit1 = getHandSuit(hand1);
            suit2 = getHandSuit(hand2);
            int adjustedValue1 = highestCard1 - suit1;
            int adjustedValue2 = highestCard2 - suit2;

            if (adjustedValue1 == adjustedValue2) {
                return RES_DRAW;
            } else {
                return (adjustedValue1 > adjustedValue2) ? RES_WIN_A : RES_WIN_B;
            }
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

        case 7: {
            highestCard1 = getHighestCard(hand1);
            highestCard2 = getHighestCard(hand2);
            int value1 = getCardValue(highestCard1);
            int value2 = getCardValue(highestCard2);

            if (value1 == value2) {
                return RES_DRAW;
            } else {
                return (value1 > value2) ? RES_WIN_A : RES_WIN_B;
            }
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

            for (int i = 0; i < HAND_SIZE; i++) {
                valueCounts1[getCardValue(hand1[i])]++;
                valueCounts2[getCardValue(hand2[i])]++;
            }

            for (int v = 15; v >= 3; v--) {
                if (valueCounts1[v] == 2) pairValue1 = v;
                if (valueCounts2[v] == 2) pairValue2 = v;
            }

            if (pairValue1 != pairValue2) return (pairValue1 > pairValue2) ? RES_WIN_A : RES_WIN_B;

            for (int i = 0; i < HAND_SIZE; i++) {
                int value1 = getCardValue(sortedHand1[i]);
                int value2 = getCardValue(sortedHand2[i]);
                if (value1 != pairValue1 && value2 != pairValue2) {
                    if (value1 != value2) return (value1 > value2) ? RES_WIN_A : RES_WIN_B;
                }
            }
            return RES_DRAW;
        }

        default: {
            for (int i = 0; i < HAND_SIZE; i++) {
                int value1 = getCardValue(sortedHand1[i]);
                int value2 = getCardValue(sortedHand2[i]);
                if (value1 != value2) return (value1 > value2) ? RES_WIN_A : RES_WIN_B;
            }
            return RES_DRAW;
        }
    }
}



int comparePokerHands(const int hand1[], const int hand2[]) {
    if (doubleChecker(hand1, hand2) == RES_INVALID) {
        return RES_INVALID;
    }

    int a = determineBestHand(hand1);
    int b = determineBestHand(hand2);
    if (a == RES_INVALID || b == RES_INVALID) {
        return RES_INVALID;
    }

    if (a < b) {
        return RES_WIN_A;
    } else if (b < a) {
        return RES_WIN_B;
    } else if (a == b) {
        int result = tightComparrasion(hand1, hand2, a);
        switch (result) {
            case RES_WIN_A: return RES_WIN_A;
            case RES_WIN_B: return RES_WIN_B;
            case RES_DRAW: return RES_DRAW;
            case RES_INVALID:  return RES_INVALID;
        }
    }
    return RES_DRAW; 
}

#ifndef __PROGTEST__
int main ()
{
 int x0[] = { SPADES('5'), HEARTS('5'), CLUBS('5'), DIAMONDS('5'), HEARTS('X') };
  int y0[] = { SPADES('6'), SPADES('9'), SPADES('8'), SPADES('X'), SPADES('7') };
  assert ( comparePokerHands ( x0, y0 ) == RES_WIN_B );

  int x1[] = { SPADES('2'), HEARTS('2'), CLUBS('2'), SPADES('A'), DIAMONDS('2') };
  int y1[] = { CLUBS('A'), HEARTS('K'), HEARTS('A'), SPADES('K'), DIAMONDS('A') };
  assert ( comparePokerHands ( x1, y1 ) == RES_WIN_A );

  int x2[] = { CLUBS('3'), HEARTS('2'), HEARTS('3'), SPADES('2'), DIAMONDS('3') };
  int y2[] = { CLUBS('A'), CLUBS('9'), CLUBS('Q'), CLUBS('4'), CLUBS('J') };
  assert ( comparePokerHands ( x2, y2 ) == RES_WIN_A );

  int x3[] = { DIAMONDS('3'), HEARTS('7'), SPADES('5'), DIAMONDS('6'), SPADES('4') };
  int y3[] = { CLUBS('2'), CLUBS('4'), CLUBS('6'), CLUBS('3'), CLUBS('X') };
  assert ( comparePokerHands ( x3, y3 ) == RES_WIN_B );

  int x4[] = { DIAMONDS('3'), HEARTS('7'), SPADES('5'), DIAMONDS('6'), SPADES('4') };
  int y4[] = { CLUBS('2'), DIAMONDS('2'), CLUBS('6'), CLUBS('3'), HEARTS('2') };
  assert ( comparePokerHands ( x4, y4 ) == RES_WIN_A );

  int x5[] = { DIAMONDS('3'), HEARTS('7'), SPADES('3'), DIAMONDS('6'), SPADES('7') };
  int y5[] = { CLUBS('2'), DIAMONDS('2'), CLUBS('6'), CLUBS('3'), HEARTS('2') };
  assert ( comparePokerHands ( x5, y5 ) == RES_WIN_B );

  int x6[] = { DIAMONDS('3'), HEARTS('7'), SPADES('3'), DIAMONDS('6'), SPADES('7') };
  int y6[] = { CLUBS('2'), DIAMONDS('9'), CLUBS('K'), CLUBS('A'), HEARTS('2') };
  assert ( comparePokerHands ( x6, y6 ) == RES_WIN_A );

  int x7[] = { DIAMONDS('A'), HEARTS('J'), SPADES('Q'), DIAMONDS('X'), SPADES('2') };
  int y7[] = { CLUBS('2'), DIAMONDS('9'), CLUBS('K'), CLUBS('A'), HEARTS('2') };
  assert ( comparePokerHands ( x7, y7 ) == RES_WIN_B );

  int x8[] = { DIAMONDS('A'), HEARTS('J'), SPADES('Q'), DIAMONDS('X'), SPADES('2') };
  int y8[] = { CLUBS('Q'), DIAMONDS('K'), CLUBS('2'), CLUBS('A'), HEARTS('3') };
  assert ( comparePokerHands ( x8, y8 ) == RES_WIN_B );

  int x9[] = { DIAMONDS('A'), HEARTS('5'), SPADES('4'), DIAMONDS('5'), CLUBS('4') };
  int y9[] = { DIAMONDS('4'), DIAMONDS('K'), CLUBS('5'), SPADES('5'), HEARTS('4') };
  assert ( comparePokerHands ( x9, y9 ) == RES_WIN_A );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */