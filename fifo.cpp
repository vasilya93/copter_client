#include <stddef.h>
#include <string.h>
#include "fifo.h"

template <typename T>
CFifo<T>::CFifo() :
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

template <typename T>
CFifo<T>::~CFifo()
{
    delete m_pArray;
}

template <typename T>
int CFifo<T>::Initialize(unsigned int nSize)
{
    m_pArray = new T[nSize];
    if (m_pArray == NULL)
      return 1;

    m_nSize = nSize;
    m_nFreeElems = nSize;
    m_nOccupiedElems = 0;

    return 0;
}

template <typename T>
int CFifo<T>::PushArray(const T *pArray, unsigned int nSize)
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
      memcpy(m_pArray + m_nIndexLast, pArray, nSize * sizeof(T));
    } else {
      memcpy(m_pArray + m_nIndexLast, pArray, nLeftToEnd * sizeof(T));
      memcpy(m_pArray, pArray + nLeftToEnd, (nSize - nLeftToEnd) * sizeof(T));
    }

    IncreaseElemsCount(nSize);
    return 0;
}

template <typename T>
int inline CFifo<T>::Push(T nNewElem)
{
    if (m_pArray == NULL)
      return 1;

    if (IsFull())
      DecreaseElemsCount(1);

    m_pArray[m_nIndexLast] = nNewElem;

    IncreaseElemsCount(1);
    return 0;
}

template <typename T>
T *CFifo<T>::PopArray(unsigned int nSize)
{
    if (nSize > m_nOccupiedElems)
        return NULL;

    T *pArray = new T[nSize];
    unsigned int nLeftToEnd = m_nSize - m_nIndexFirst;
    if (nLeftToEnd >= nSize) {
      memcpy(pArray, m_pArray + m_nIndexFirst, nSize * sizeof(T));
    } else {
      memcpy(pArray, m_pArray + m_nIndexFirst, nLeftToEnd * sizeof(T));
      memcpy(pArray + nLeftToEnd, m_pArray, (nSize - nLeftToEnd) * sizeof(T));
    }

    DecreaseElemsCount(nSize);
    return pArray;
}

template <typename T>
T CFifo<T>::Pop()
{
    T nChar = (T) 0;

    if ((m_pArray == NULL) || m_bIsEmpty)
      return nChar;

    nChar = m_pArray[m_nIndexFirst];
    DecreaseElemsCount(1);
    return nChar;
}

template <typename T>
int CFifo<T>::Reset()
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

template <typename T>
T *CFifo<T>::GetData(unsigned int *nSize)
{
    unsigned int nLeftToEnd = m_nSize - m_nIndexFirst;
    unsigned int nElemsCount = m_nSize - m_nFreeElems;
    T *pData = new T[nElemsCount];
    if (pData == NULL) {
        if (nSize != NULL)
            *nSize = 0;
        return pData;
    }

    if (nLeftToEnd >= nElemsCount) {
      memcpy(pData, m_pArray + m_nIndexFirst, sizeof(T) * nElemsCount);
    } else {
      memcpy(pData, m_pArray + m_nIndexFirst, sizeof(T) * nLeftToEnd);
      memcpy(pData + nLeftToEnd, m_pArray, sizeof(T) * (nElemsCount - nLeftToEnd));
    }

    if (nSize != NULL)
      *nSize = nElemsCount;

    return pData;
}

template <typename T>
void CFifo<T>::IncreaseElemsCount(unsigned int nValueAdded)
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

template <typename T>
void CFifo<T>::DecreaseElemsCount(unsigned int nValueAdded)
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

template class CFifo<int>;
template class CFifo<unsigned char>;
