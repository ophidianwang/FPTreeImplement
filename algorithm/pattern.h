#ifndef PATTERN_H
#define PATTERN_H

#include <QList>
#include <QMap>
#include <QSet>
#include <QPair>

//about aprioi algorithm
int countSub(QSet<int> &sub, QList<QSet<int> > &list);
bool reachThreshold(QSet<int> &sub, QList<QSet<int> > &list, int threshold);
bool containSet(QSet<int> &set, QList<QSet<int> > &list);

//about merge sort
QList<QPair<int, int> > mergeSort(QList<QPair<int, int> > &list);

#endif // PATTERN_H

