#ifndef NODE_H
#define NODE_H
#include <QList>
#include <QMap>
#include <QSet>
#include "pattern.h"

class node
{

public:
    node(int layer=0);
    ~node();

    //build frequenct-list (from frequent to rare)
    static QList<int> makefList(QList< QSet<int> > &setList, int threshold);    //input list of item set
    static QList<int> makefList(QList<QPair<QSet<int>,int> > condDb ,int threshold);    //input list of item set with count

    //node member
    int layer;
    int value;
    int count;
    QMap< int, node* > children;    //child nodes

    // input set of int and frequent-list (from frequent to rare) , build FP-tree structure
    void accept(QSet<int> &input , QList<int> &fList);

    //recursively build condtional FP-tree and discover frequent itemset
    void discover(int threshold, QSet<int> &set, QList<int> &fList, QList<QSet<int> > &result);

    //debug function
    void show();    //show FP-tree structure

private:
    //about conditional db
    void projectDB(int target, QList<QPair<QSet<int>,int >  > &db );
    void projectRecursive(int target, QSet<int> &parent, QList<QPair<QSet<int>,int >  > &db );

};

#endif // NODE_H
