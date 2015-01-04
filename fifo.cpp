#include <stddef.h>
#include <string.h>
#include "fifo.h"

CFifo::CFifo() :
    m_pArray(NULL),
    m_nSize(0),
    m_nIndexFirst(0),
    m_nIndexLast(0),
    m_nFreeElems(0),
    m_nOccupiedElems(0),
    m_bIsEmpty(true),
    m_bIsFull(false)
{
}

CFifo::~CFifo()
{
    delete m_pArray;
}

int CFifo::Initialize(unsigned int nSize)
{
    m_pArray = new unsigned char[nSize];
    if (m_pArray == NULL)
      return 1;

    m_nSize = nSize;
    m_nFreeElems = nSize;
    m_nOccupiedElems = 0;

    return 0;
}

int CFifo::PushArray(const unsigned char *pArray, unsigned int nSize)
{
    unsigned long nLeftToEnd;

    if (pArray == NULL || m_pArray == NULL)
      return 1;

    if (m_nSize < nSize)
      return 1;

    if (nSize > m_nFreeElems)
      DecreaseElemsCount(nSize - m_nFreeElems);

    nLeftToEnd = m_nSize - m_nIndexLast;
    if (nLeftToEnd >= nSize) {
      memcpy(m_pArray + m_nIndexLast, pArray, nSize);
    } else {
      memcpy(m_pArray + m_nIndexLast, pArray, nLeftToEnd);
      memcpy(m_pArray, pArray + nLeftToEnd, nSize - nLeftToEnd);
    }

    IncreaseElemsCount(nSize);
    return 0;
}

int inline CFifo::Push(unsigned char nNewElem)
{
    if (m_pArray == NULL)
      return 1;

    return PushArray(&nNewElem, 1);
}

unsigned char* CFifo::PopArray(unsigned int nSize)
{
    if (nSize > m_nOccupiedElems)
        return NULL;

    unsigned char *pArray = new unsigned char[nSize];
    unsigned int nLeftToEnd = m_nSize - m_nIndexFirst;
    if (nLeftToEnd >= nSize) {
      memcpy(pArray, m_pArray + m_nIndexFirst, nSize);
    } else {
      memcpy(pArray, m_pArray + m_nIndexFirst, nLeftToEnd);
      memcpy(pArray + nLeftToEnd, m_pArray, nSize - nLeftToEnd);
    }

    DecreaseElemsCount(nSize);
    return pArray;
}

unsigned char CFifo::Pop()
{
    unsigned char nChar = '\0';

    if ((m_pArray == NULL) || m_bIsEmpty)
      return nChar;

    nChar = m_pArray[m_nIndexFirst];
    DecreaseElemsCount(1);
    return nChar;
}

int CFifo::Reset()
{
    if (m_pArray == NULL)
      return 1;

    m_nFreeElems = m_nSize;
    m_nOccupiedElems = 0;
    m_nIndexFirst = 0;
    m_nIndexLast = 0;
    m_bIsEmpty = true;
    m_bIsFull = false;
    return 0;
}

unsigned char *CFifo::GetData(unsigned int *nSize)
{
    unsigned int nLeftToEnd = m_nSize - m_nIndexFirst;
    unsigned int nElemsCount = m_nSize - m_nFreeElems;
    unsigned char *pData = new unsigned char(nElemsCount);
    if (pData == NULL) {
        if (nSize != NULL)
            *nSize = 0;
        return pData;
    }

    if (nLeftToEnd >= nElemsCount) {
      memcpy(pData, m_pArray + m_nIndexFirst, nElemsCount);
    } else {
      memcpy(pData, m_pArray + m_nIndexFirst, nLeftToEnd);
      memcpy(pData + nLeftToEnd, m_pArray, nElemsCount - nLeftToEnd);
    }

    if (nSize != NULL)
      *nSize = nElemsCount;

    return pData;
}

void CFifo::IncreaseElemsCount(unsigned int nValueAdded)
{
    m_nIndexLast += nValueAdded;
    if (m_nIndexLast >= m_nSize)
      m_nIndexLast -= m_nSize;

    m_nFreeElems -= nValueAdded;
    m_nOccupiedElems += nValueAdded;
    m_bIsEmpty = false;
    if (m_nIndexFirst == m_nIndexLast)
      m_bIsFull = true;
}

void CFifo::DecreaseElemsCount(unsigned int nValueAdded)
{
    m_nIndexFirst += nValueAdded;
    if (m_nIndexFirst >= m_nSize)
      m_nIndexFirst -= m_nSize;

    m_nFreeElems += nValueAdded;
    m_nOccupiedElems -= nValueAdded;
    m_bIsFull = false;
    if (m_nIndexFirst == m_nIndexLast)
      m_bIsEmpty = true;
}
