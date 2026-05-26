#ifndef CVASTARNODEPOOL_H
#define CVASTARNODEPOOL_H

#include <new>
#include "CvGameCoreDLLPCH.h"
#include "CvAStarNode.h"

class CvAStarNodePool
{
public:
    CvAStarNodePool();
    ~CvAStarNodePool();

    void Initialize(int iColumns, int iRows);
    void DeInit();

    CvAStarNode* GetNode(int iX, int iY);
    CvAStarNode** GetRows();

    int GetColumns() const { return m_iColumns; }
    int GetRowsCount() const { return m_iRows; }

private:
    CvAStarNode* m_pNodes;
    CvAStarNode** m_ppRowStarts;
    int m_iColumns;
    int m_iRows;
};

inline CvAStarNodePool::CvAStarNodePool()
: m_pNodes(NULL)
, m_ppRowStarts(NULL)
, m_iColumns(0)
, m_iRows(0)
{
}

inline CvAStarNodePool::~CvAStarNodePool()
{
    DeInit();
}

inline void CvAStarNodePool::Initialize(int iColumns, int iRows)
{
    DeInit();

    m_iColumns = iColumns;
    m_iRows = iRows;

    m_ppRowStarts = reinterpret_cast<CvAStarNode**>(FMALLOCALIGNED(sizeof(CvAStarNode*) * m_iColumns, 64, c_eCiv5GameplayDLL, 0));
    m_pNodes = reinterpret_cast<CvAStarNode*>(FMALLOCALIGNED(sizeof(CvAStarNode) * m_iColumns * m_iRows, 64, c_eCiv5GameplayDLL, 0));

    for (int i = 0; i < m_iColumns; ++i)
    {
        m_ppRowStarts[i] = m_pNodes + (i * m_iRows);
        for (int j = 0; j < m_iRows; ++j)
        {
            CvAStarNode* pNode = &m_ppRowStarts[i][j];
            new(pNode) CvAStarNode();
            pNode->m_iX = i;
            pNode->m_iY = j;
        }
    }
}

inline void CvAStarNodePool::DeInit()
{
    if (m_ppRowStarts != NULL)
    {
        FFREEALIGNED(m_ppRowStarts);
        m_ppRowStarts = NULL;
    }

    if (m_pNodes != NULL)
    {
        FFREEALIGNED(m_pNodes);
        m_pNodes = NULL;
    }

    m_iColumns = 0;
    m_iRows = 0;
}

inline CvAStarNode* CvAStarNodePool::GetNode(int iX, int iY)
{
    return &m_ppRowStarts[iX][iY];
}

inline CvAStarNode** CvAStarNodePool::GetRows()
{
    return m_ppRowStarts;
}

#endif // CVASTARNODEPOOL_H
