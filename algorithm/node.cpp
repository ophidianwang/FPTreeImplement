#include "node.h"
#include <QDebug>

/*
    Scan db (all item set) to build frequent list.
    Return a list of items which are sort from frequent to rare.
*/
QList<int> node::makefList(QList< QSet<int> > &setList, int threshold){

    //count each item's frequency
    QMap<int, int> accumulator;
    for(int i=0; i<setList.size(); i++){

        QSetIterator<int > itrSet( setList.at(i) );

        while(itrSet.hasNext()){

            int key = itrSet.next();

            if( accumulator.contains( key ) ){
                accumulator.insert( key, accumulator.value(key)+1 );
            }else{
                accumulator.insert( key, 1 );
            }

        }
    }

    //qDebug() << accumulator;

    //sort item with frequency
    QList< QPair<int,int> > pairList;
    QMapIterator<int, int> itrAcc(accumulator);
    while(itrAcc.hasNext()){
        itrAcc.next();
        if(itrAcc.value() < threshold)
            continue;

        pairList.append( qMakePair( itrAcc.key(), itrAcc.value() ) );
    }

    //sort pairList by second
    QList<QPair<int,int> > sortList  =mergeSort(pairList);

    //qDebug() << "sorted list of pair: " << sortList;

    //build frequenct-list (from frequent to rare)
    QList<int> fList;
    for( int i=sortList.size(); i>0; i--){
        fList.append( sortList.at( i-1 ).first );
    }

    return fList;
}

/*
    Scan db (all item set) to build frequent list; each item set has its own count.
    Return a list of items which are sort from frequent to rare.
    This function is used to build conditional frequent list.
*/
QList<int> node::makefList(QList<QPair<QSet<int>, int> > condDb, int threshold){
    //count each item's frequency
    QMap<int, int> accumulator;
    for(int i=0; i<condDb.size(); i++){

        QSetIterator<int > itrSet( condDb.at(i).first );
        int plus = condDb.at(i).second;

        while(itrSet.hasNext()){

            int key = itrSet.next();

            if( accumulator.contains( key ) ){
                accumulator.insert( key, accumulator.value(key)+plus );
            }else{
                accumulator.insert( key, plus );
            }

        }
    }
    //qDebug() << accumulator;

    //sort item with frequency
    QList< QPair<int,int> > pairList;
    QMapIterator<int, int> itrAcc(accumulator);
    while(itrAcc.hasNext()){
        itrAcc.next();
        if(itrAcc.value() < threshold)
            continue;

        pairList.append( qMakePair( itrAcc.key(), itrAcc.value() ) );
    }

    //sort pairList by second
    QList<QPair<int,int> > sortList  =mergeSort(pairList);

    //qDebug() << "sorted list of pair: " << sortList;

    //build frequenct-list (from frequent to rare)
    QList<int> fList;
    for( int i=sortList.size(); i>0; i--){
        fList.append( sortList.at( i-1 ).first );
    }

    return fList;
}

/*
    constructor
*/
node::node(int layer)
{
    this->layer = layer;
}

/*
    destructor
*/
node::~node()
{
    //delete child nodes
    QMapIterator<int, node*> itr(children);

    while(itr.hasNext()){
        itr.next();

        delete itr.value();
    }
}

/*
    put item set into node and build FP-tree
*/
void node::accept(QSet<int> &input, QList<int> &fList){
    //qDebug() << "layer: " << layer << " | input: " << input;

    node* childNode = 0;

    //from most frequent unit to lower one
    for( int i=0; i< fList.size(); i++){

        int current = fList.at(i);

        //check input item set's branch
        if( input.contains( current ) ){
            //qDebug() << "hit: " << current;

            //check if branch is created
            if( children.contains( current ) ){

                //qDebug() << " accumulate node: " << current << " @ layer: " << layer+1;

                childNode = children.value( current );
                childNode->count = childNode->count +1;

            }else{

                //qDebug() << " create node: " << current << " @ layer: " << layer+1;

                childNode = new node( layer+1 );
                childNode->value = current;
                childNode->count = 1;

                children.insert( current, childNode );

            }

            //put input to next layer
            QSet<int> next = input;
            next.remove( current );
            childNode->accept( next, fList );

            break;
        }

    }
}

/*
    build conditional db entry
    add conditional item set into given db reference
*/
void node::projectDB(int target, QList<QPair<QSet<int>,int >  > &db ){
    //only exec @ root node

    QSet<int> empty;

    QMapIterator<int, node*> itr(children);
    while(itr.hasNext()){
        itr.next();

        if( itr.value()->value == target){

        }else{
            itr.value()->projectRecursive(target, empty, db);
        }
    }
}

/*
    build conditional db recursive
    add conditional item set into given db reference
*/
void node::projectRecursive(int target, QSet<int> &parent, QList<QPair<QSet<int>,int >  > &db ){

    if(value == target){

        db.append( qMakePair( parent, count ) );

    }else{

        QSet<int> current = parent;
        current.insert(value);

        QMapIterator<int, node*> itr(children);
        while(itr.hasNext()){
            itr.next();
            itr.value()->projectRecursive(target, current, db);
        }

    }
}

/*
    recursively build conditional FP-tree and append frequent itemset to result reference
*/
void node::discover(int threshold, QSet<int> &set, QList<int> &fList, QList<QSet<int> > &result){

    //goal of recursive
    if(fList.isEmpty())
        return;

    //if there is a single prefix path
    if( children.size() == 1 ){

        node* onlyOne = children.first();

        QSet<int> prefix = set;
        prefix.insert( onlyOne->value );

        QList<int> subfList = fList;
        subfList.removeAll( onlyOne->value );

        //qDebug() << "single prefix, skip from "<< set << " to " << prefix << " | fList: " << fList << " to subfList: " << subfList;

        if( onlyOne->count >= threshold )
            result.append( prefix );

        onlyOne->discover( threshold, prefix, subfList, result);

        return;
    }

    //cond. root node pointer
    node* condRoot;

    for(int i=0; i<fList.size(); i++){

        int target = fList.at(i);
        QSet<int> next = set;
        next.insert( target );

        //append set to result
        result.append( next );

        //new cond. FP-tree root node
        condRoot = new node;
        condRoot->value = target;

        //build cond. db
        QList<QPair<QSet<int>,int> > condDB;
        projectDB(target, condDB);
        //qDebug() << "cond. db of " << next << " : " << condDB;

        //build cond. frequency-list
        QList<int> fListCond = makefList(condDB, threshold);
        //qDebug() << "cond. fList of " << next <<" : " << fListCond;

        //build cond. FP-tree
        for(int j=0; j<condDB.size(); j++){

            int count =condDB.at(j) .second;
            QSet<int> condSet = condDB.at(j).first;

            for(int k=0; k<count; k++){
                condRoot->accept( condSet , fListCond );
            }
        }
        //condRoot->show();

        //recursive run discover
        condRoot->discover(threshold, next, fListCond, result);

        //release resource
        delete condRoot;
    }
}

/*
    debug fuction
    display FP-tree structure in console
*/
void node::show(){

    QString tab = "\t";
    qDebug() << tab.repeated(layer) << "layer: " << layer << " | value: " << value << " | count: " << count;

    QMapIterator<int, node*> itr(children);

    while(itr.hasNext()){
        itr.next();

        itr.value()->show();
    }
}
