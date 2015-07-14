#include "pattern.h"
#include <QDebug>

/*
    count how many item set contain target subset
*/
int countSub(QSet<int> &sub, QList<QSet<int> > &list){
    int count = 0;
    for(int i=0; i<list.size(); i++){
        if( list.at(i).contains(sub) )
            count++;
    }
    return count;
}

/*
    check if target subset appears more than threshold times
*/
bool reachThreshold(QSet<int> &sub, QList<QSet<int> > &list, int threshold){
    int count = 0;
    for(int i=0; i<list.size(); i++){
        if( list.at(i).contains(sub) )
            count++;
        if(count >= threshold)
            return true;
    }
    return false;
}

/*
    check if target set is DB (all item set)
*/
bool containSet(QSet<int> &set, QList<QSet<int> > &list){
    for(int i=0; i<list.size(); i++){
        if( list.at(i) == set)
            return true;
    }
    return false;
}

/*
    implement of merge sort, sort with pair.second
*/
QList<QPair<int, int> > mergeSort(QList<QPair<int, int> > &list){
    int size = list.size();

    //single unit, recursive sort ends here
    if(size < 2)
        return list;

    QList<QPair<int, int> > result;

    QList<QPair<int, int> > first = list.mid(0, (int)(size/2));
    QList<QPair<int, int> > second = list.mid((int)(size/2));

    /*
    qDebug() << "first: " << first;
    qDebug() << "second: " << second;
    */

    QList<QPair<int, int> > firstSort = mergeSort(first);
    QList<QPair<int, int> > secondSort = mergeSort(second);

    /*
    qDebug() << "firstSort: " << firstSort;
    qDebug() << "secondSort: " << secondSort;
    */

    int firstIndex = 0;
    int secondIndex = 0;

    while(result.size() < firstSort.size() + secondSort.size()){

        if(firstIndex == firstSort.size()){
            //all firstSort units are in result list, append rest of secondSort to result
            result.append( secondSort.mid( secondIndex ) );

        }else if(secondIndex == secondSort.size()){
            //all secondSort units are in result list, append rest of firstSort to result
            result.append( firstSort.mid( firstIndex ) );

        }else if( firstSort.at(firstIndex).second < secondSort.at(secondIndex).second ){

            result.append( firstSort.at( firstIndex ) );
            firstIndex++;

        }else{

            result.append( secondSort.at( secondIndex ) );
            secondIndex++;

        }

    }

    return result;
}

