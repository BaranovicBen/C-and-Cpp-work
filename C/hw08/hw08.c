#ifndef __PROGTEST__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdbool.h>

const int PHONE_DIGITS = 10;

typedef struct TNode {
  char * m_Name;
  struct TNode * m_Child[PHONE_DIGITS];
} TNODE;

typedef struct {
  TNODE * m_Root;
  int m_Size;
} TPHONEBOOK;
#endif /* __PROGTEST__ */

static bool checkDigits (const char * phone)
{
    if (phone == NULL)
    {
        return false;
    }

    if (*phone == '\0')
    {
        return false;
    }

    for (const char *sc = phone; *sc != '\0'; sc++)
    {
        if (*sc < '0' || *sc > '9')
        {
            return false;
        }
    }
    return true;
}

static TNODE * buildLeaf (void)
{
    TNODE * freshN = (TNODE *) malloc(sizeof(TNODE));
    if (freshN == NULL)
    {
        return NULL;
    }
    freshN->m_Name = NULL;
    for (int i = 0; i < PHONE_DIGITS; i++)
    {
        freshN->m_Child[i] = NULL;
    }
    return freshN;
}

static void clearAll (TNODE * subtree)
{
    if (subtree == NULL)
    {
        return;
    }

    for (int idx = 0; idx < PHONE_DIGITS; idx++)
    {
        if (subtree->m_Child[idx] != NULL)
        {
            clearAll(subtree->m_Child[idx]);
        }
    }

    if (subtree->m_Name != NULL)
    {
        free(subtree->m_Name);
    }
    free(subtree);
}

void delBook (TPHONEBOOK * phonebook)
{
    if (phonebook == NULL)
    {
        return;
    }

    clearAll(phonebook->m_Root);
    phonebook->m_Root = NULL;
    phonebook->m_Size = 0;
}

bool addPhone (TPHONEBOOK * phonebook, const char * phone, const char * name)
{
    if (phonebook == NULL || phone == NULL || name == NULL)
    {
        return false;
    }

    if (!checkDigits(phone))
    {
        return false;
    }

    if (phonebook->m_Root == NULL)
    {
        phonebook->m_Root = buildLeaf();
        if (phonebook->m_Root == NULL)
        {
            return false; 
        }
    }

    TNODE * cursor = phonebook->m_Root;
    for (const char * px = phone; *px != '\0'; px++)
    {
        int d = *px - '0';
        if (cursor->m_Child[d] == NULL)
        {
            cursor->m_Child[d] = buildLeaf();
            if (cursor->m_Child[d] == NULL)
            {
                return false;
            }
        }
        cursor = cursor->m_Child[d];
    }

    if (cursor->m_Name == NULL)
    {
        phonebook->m_Size++;
    }
    else
    {
        free(cursor->m_Name);
        cursor->m_Name = NULL;
    }

    cursor->m_Name = (char *) malloc(strlen(name) + 1);
    if (cursor->m_Name == NULL)
    {
        return false;
    }
    strcpy(cursor->m_Name, name);

    return true;
}

static TNODE * compress (TNODE * node)
{
    if (node == NULL)
    {
        return NULL;
    }

    bool keepBranch = false;
    for (int cdx = 0; cdx < PHONE_DIGITS; cdx++)
    {
        if (node->m_Child[cdx] != NULL)
        {
            node->m_Child[cdx] = compress(node->m_Child[cdx]);
            if (node->m_Child[cdx] != NULL)
            {
                keepBranch = true;
            }
        }
    }

    if (!keepBranch && node->m_Name == NULL)
    {
        free(node);
        node = NULL;
    }
    return node;
}

bool delPhone (TPHONEBOOK * phonebook, const char * phone)
{
    if (phonebook == NULL || phone == NULL)
    {
        return false;
    }

    if (!checkDigits(phone))
    {
        return false;
    }

    if (phonebook->m_Root == NULL)
    {
        return false;
    }

    TNODE * route[256];
    int routeLen = 0;

    TNODE * cNode = phonebook->m_Root;
    for (const char * q = phone; *q != '\0'; q++)
    {
        route[routeLen++] = cNode;
        int dig = *q - '0';
        if (cNode->m_Child[dig] == NULL)
        {
            return false;
        }
        cNode = cNode->m_Child[dig];
    }
    route[routeLen++] = cNode;

    if (cNode->m_Name == NULL)
    {
        return false;
    }

    free(cNode->m_Name);
    cNode->m_Name = NULL;
    phonebook->m_Size--;

    for (int idx = routeLen - 1; idx >= 0; idx--)
    {
        TNODE * reduced = compress(route[idx]);
        if (idx == 0)
        {
            phonebook->m_Root = reduced;
        }
        else
        {
            int stepDigit = phone[idx - 1] - '0';
            route[idx - 1]->m_Child[stepDigit] = reduced;
        }
    }

    return true;
}

const char * findPhone (const TPHONEBOOK * phonebook, const char * phone)
{
    if (phonebook == NULL || phone == NULL)
    {
        return NULL;
    }

    if (!checkDigits(phone))
    {
        return NULL;
    }

    if (phonebook->m_Root == NULL)
    {
        return NULL;
    }

    const TNODE * pointer = phonebook->m_Root;
    const char * nearestName = pointer->m_Name;

    for (const char * pr = phone; *pr != '\0'; pr++)
    {
        int d = *pr - '0';
        if (pointer->m_Child[d] == NULL)
        {
            break;
        }
        pointer = pointer->m_Child[d];
        if (pointer->m_Name != NULL)
        {
            nearestName = pointer->m_Name;
        }
    }

    return nearestName;
}

#ifndef __PROGTEST__
int main ()
{
  TPHONEBOOK b = { NULL, 0 };
  char tmpStr[100];
  const char * name;
  assert ( addPhone ( &b, "420", "Czech Republic" ) );
  assert ( addPhone ( &b, "42022435", "Czech Republic CVUT" ) );
  assert ( addPhone ( &b, "421", "Slovak Republic" ) );
  assert ( addPhone ( &b, "44", "Great Britain" ) );
  strncpy ( tmpStr, "USA", sizeof ( tmpStr ) - 1 );
  assert ( addPhone ( &b, "1", tmpStr ) );
  strncpy ( tmpStr, "Guam", sizeof ( tmpStr ) - 1 );
  assert ( addPhone ( &b, "1671", tmpStr ) );
  assert ( addPhone ( &b, "44", "United Kingdom" ) );
  assert ( b . m_Size == 6 );
  assert ( ! b . m_Root -> m_Name );
  assert ( ! b . m_Root -> m_Child[0] );
  assert ( ! strcmp ( b . m_Root -> m_Child[1] -> m_Name, "USA" ) );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Name );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Name );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[0] );
  assert ( ! strcmp ( b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Name, "Guam" ) );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[1] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[7] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Name );
  assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Name, "Czech Republic" ) );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[4] );
  assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Name, "Czech Republic CVUT" ) );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[9] );
  assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Name, "Slovak Republic" ) );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[3] );
  assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[4] -> m_Name, "United Kingdom" ) );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[9] );
  name = findPhone ( &b, "420800123456" );  assert ( !strcmp ( name, "Czech Republic" ) );
  name = findPhone ( &b, "420224351111" );  assert ( !strcmp ( name, "Czech Republic CVUT" ) );
  name = findPhone ( &b, "42022435" );  assert ( !strcmp ( name, "Czech Republic CVUT" ) );
  name = findPhone ( &b, "4202243" );  assert ( !strcmp ( name, "Czech Republic" ) );
  name = findPhone ( &b, "420224343258985224" );  assert ( !strcmp ( name, "Czech Republic" ) );
  name = findPhone ( &b, "42" );  assert ( !name );
  name = findPhone ( &b, "422" );  assert ( !name );
  name = findPhone ( &b, "4422" );  assert ( !strcmp ( name, "United Kingdom" ) );
  name = findPhone ( &b, "16713425245763" );  assert ( !strcmp ( name, "Guam" ) );
  name = findPhone ( &b, "123456789123456789" );  assert ( !strcmp ( name, "USA" ) );
  assert ( delPhone ( &b, "420" ) );
  assert ( delPhone ( &b, "421" ) );
  assert ( delPhone ( &b, "44" ) );
  assert ( delPhone ( &b, "1671" ) );
  assert ( !delPhone ( &b, "1672" ) );
  assert ( !delPhone ( &b, "1671" ) );
  name = findPhone ( &b, "16713425245763" );  assert ( !strcmp ( name, "USA" ) );
  name = findPhone ( &b, "4422" );  assert ( !name );
  name = findPhone ( &b, "420800123456" );  assert ( !name );
  name = findPhone ( &b, "420224351111" );  assert ( !strcmp ( name, "Czech Republic CVUT" ) );
  assert ( b . m_Size == 2 );
  assert ( ! b . m_Root -> m_Name );
  assert ( ! b . m_Root -> m_Child[0] );
  assert ( ! strcmp ( b . m_Root -> m_Child[1] -> m_Name, "USA" ) );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[1] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Name );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[4] );
  assert ( ! strcmp ( b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Name, "Czech Republic CVUT" ) );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[0] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[5] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[3] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[4] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[2] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[2] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[0] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[1] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[2] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[2] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[3] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[4] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[4] -> m_Child[9] );
  assert ( ! b . m_Root -> m_Child[5] );
  assert ( ! b . m_Root -> m_Child[6] );
  assert ( ! b . m_Root -> m_Child[7] );
  assert ( ! b . m_Root -> m_Child[8] );
  assert ( ! b . m_Root -> m_Child[9] );
  assert ( delPhone ( &b, "1" ) );
  assert ( delPhone ( &b, "42022435" ) );
  assert ( !addPhone ( &b, "12345XYZ", "test" ) );
  assert ( b . m_Size == 0 );
  assert ( ! b . m_Root );

  printf("Succes\n");


  delBook ( &b );
  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */