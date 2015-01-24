#ifndef FIFO_H
#define FIFO_H

template <typename T>
class CFifo
{
public:
    CFifo();
    ~CFifo();

    int Initialize(unsigned int nSize);
    int PushArray(const T *pArray, unsigned int nSize);
    int inline Push(T nNewElem);
    T* PopArray(unsigned int nSize);
    T Pop();
    int Reset();
    T *GetData(unsigned int *nSize);

    inline bool IsFull()
    {
        return m_bIsFull;
    }
    inline bool IsEmpty()
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
    T* m_pArray;
    unsigned int m_nSize,
                 m_nIndexFirst,
                 m_nIndexLast,
                 m_nFreeElems,
                 m_nOccupiedElems;
    bool m_bIsEmpty,
         m_bIsFull;
};

#endif // FIFO_H
