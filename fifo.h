#ifndef FIFO_H
#define FIFO_H

class CFifo
{
public:
    CFifo();
    ~CFifo();

    int Initialize(unsigned int nSize);
    int PushArray(const unsigned char *pArray, unsigned int nSize);
    int inline Push(unsigned char nNewElem);
    unsigned char* PopArray(unsigned int nSize);
    unsigned char Pop();
    int Reset();
    unsigned char *GetData(unsigned int *nSize);

    inline int IsFull()
    {
        return m_bIsFull;
    }
    inline int IsEmpty()
    {
        return m_bIsEmpty;
    }
    inline unsigned int OccupiedElems()
    {
        return m_nOccupiedElems;
    }

private:
    void IncreaseElemsCount(unsigned int nValueAdded);
    void DecreaseElemsCount(unsigned int nValueAdded);

private:
    unsigned char* m_pArray;
    unsigned int m_nSize,
                 m_nIndexFirst,
                 m_nIndexLast,
                 m_nFreeElems,
                 m_nOccupiedElems;
    bool m_bIsEmpty,
         m_bIsFull;
};

#endif // FIFO_H
