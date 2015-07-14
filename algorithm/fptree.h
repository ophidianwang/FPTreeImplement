#ifndef FPTREE_H
#define FPTREE_H

#include <QList>
#include <QSet>
#include "node.h"
#include "pattern.h"

namespace fptree {

    QList<QSet<int> > findFrequentSet(int threshold , int minLength, QList<QSet<int> > &examSets);

}

#endif // FPTREE_H

