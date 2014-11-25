#include "mindiffmetric.h"

MinDiffMetric::MinDiffMetric()
{

}

MinDiffMetric::~MinDiffMetric()
{

}

double MinDiffMetric::getMetric(
   const QVector<GlueEntry>& entries,
   const GlueOrder& order
)
{
    double w = 0;
    double h = 0;


    QVector<GlueEntry> localentries;
    for(size_t i = 0; i < order.Images.size(); i++)
    {
        localentries << entries[order.Images[i]];
    }

    if (localentries.size() > 1)
    {
        size_t maxi = 0;
        size_t minj = 0;
        size_t index = (order.Mode == HORIZONTAL)? 1 : 0;
        for(size_t i = 0; i < localentries.size(); i++)
        {
            if (localentries[i][index] > localentries[maxi][index])
            {
                maxi = i;
            }
            if (localentries[i][index] < localentries[minj][index])
            {
                minj = i;
            }
        }

        if (order.Mode == HORIZONTAL)
        {
            w = localentries[minj][0];
            h = localentries[maxi][1] - localentries[minj][1];
        }
        else
        {
            w = localentries[maxi][0] - localentries[minj][0];
            h = localentries[minj][1];
        }
    }
    return w * h;
}
