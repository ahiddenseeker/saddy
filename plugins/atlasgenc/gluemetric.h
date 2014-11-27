/*! \file gluemetric.h
    \author HiddenSeeker

    Describes a metric which should be used, when merging objects
 */
#pragma once
#include "glueentry.h"
#include "glueorder.h"
#include "maybenumber.h"

#include <QtCore/QPair>

/*! An abstract metric,
    that finds best order to merge an images into one
 */
class GlueMetric
{
public:
    /*! A size pair to be indexed
        \param[in] size pair
     */
    typedef QVector<double> SizePair;
    /*! A pair of order and size
     */
    typedef QPair<QVector<GlueOrder>, GlueMetric::SizePair> OrdersAndSize;

    /*! Defines a default metric
     */
    GlueMetric();
    /*! Must be inherited
     */
    virtual ~GlueMetric();
    /*! Returns a metric for comparing orders
        \param[in] entries an entries
        \param[in] order an order, how they should be merged
        \return metric value
     */
    virtual double getMetric(
            const QVector<GlueEntry>& entries,
            const GlueOrder& order
    ) = 0;
    /*! Finds all possible orders, where metric is minimum
        \param[in] entries source entries to be merged
        \return all possible orders, where metric is minimum
     */
    QVector<GlueOrder> findMinMetricOrder(const QVector<GlueEntry>& entries);
    /*! Finds an all possible orders with minimal metric
        Returns empty list no entries supplied. Also can return empty orders in first element
        if one images.
        \param entries entries to be merged
        \return size and order
     */
    GlueMetric::OrdersAndSize findOrder(const QVector<GlueEntry> & entries);
    /*! Scans for minimum order in array of GlueMetric::findOrder results (results parameter)
        and finds metric width minimum from maximum from width and height, with given Array[MaybeNumber, result]
        \param[in] start result of computing previous minimum texture
        \param[in] results  an array of results
        \return computation result
     */
    QPair<MaybeNumber, GlueMetric::OrdersAndSize>
    static findMinimumOrder(
        const QPair<MaybeNumber, GlueMetric::OrdersAndSize> start,
        const GlueMetric::OrdersAndSize  & results
    );
    /*! Computes a maximum between size dimensions specified by index between images, specified by order
        on an entries of entries
        \param[in] entries source array of entry
        \param[in] order source order to work with
        \param[in] index 0 - for width, 1 - for height
        \return result
     */
    static double maxMerge(const QVector<GlueEntry>& entries, GlueOrder order, int index);

    /*! Computes a sum between size dimensions specified by +index+ between images, specified by +order+
        on an entries of +entries+
        \param[in] entries source array of entry
        \param[in] order source order to work with
        \param[in] index 0 - for width, 1 - for height
        \return result
     */
    static double sumMerge(const QVector<GlueEntry>& entries, GlueOrder order, int index);
};
