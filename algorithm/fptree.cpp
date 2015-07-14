#include "fptree.h"

namespace fptree {

    //START OF NAMESPACE FPTREE

    /*
        input:
            1. frequent threshold
            2. mininum length of frequent set to be return
            3. list of item set (item map with int outside)
    */
    QList<QSet<int> > findFrequentSet(int threshold, int minLength, QList<QSet<int> > &examSets){

        QList<int> fList = node::makefList( examSets, threshold);

        //build original FP-tree
        node* root = new node();
        for(int i=0; i<examSets.size(); i++){
            QSet<int> set = examSets.at(i);
            root->accept( set, fList );
        }

        //recursive build conditional FP-tree and append frequent pattern to frequentSets
        QSet<int> empty;
        QList<QSet<int> > frequentSets; //storage of result without length limit
        root->discover( threshold, empty, fList, frequentSets);

        delete root;

        //frequent pattern length filter
        QList<QSet<int> > result;
        for(int i=0; i<frequentSets.size(); i++){
            if(frequentSets.at(i).size() >= minLength)
                result.append( frequentSets.at(i) );
        }

        return result;
    }

    //END OF NAMESPACE FPTREE

}
