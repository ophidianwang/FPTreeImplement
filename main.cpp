#include "mainwindow.h"
#include <QApplication>
#include <QSet>
#include <QList>
#include <QDateTime>
#include <QDebug>
#include "fptree.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    QList<QSet<int> > setList;

    QSet<int> frequent1;
    frequent1.insert(7);
    frequent1.insert(23);
    frequent1.insert(29);


    QSet<int> frequent2;
    frequent2.insert(3);
    frequent2.insert(5);
    frequent2.insert(17);
    frequent2.insert(19);

    for(int i=0; i<1000; i++){
        QSet<int> current;

        //frequent part
        int seed = rand()%3;

        QSet<int> randSet;

        switch (seed) {
            case 1:
                randSet = frequent1;
                break;
            case 2:
                randSet = frequent2;
                break;
            default:
                randSet = frequent1 | frequent2;
                break;
        }

        QSetIterator<int> itr( randSet );
        while(itr.hasNext()){

            int item = itr.next();

            if(rand()%5 > 1 )  //60%
                current.insert( item );
            else
                current.insert(rand() % 30 );
        }

        //rand part
        while(true){

            current.insert(rand() % 30 );

            if(rand()%7 ==0)
                break;
        }
        setList.append(current);
    }

    /*
    qDebug() << "==========all item set==========";
    qDebug() << setList;
    */

    QDateTime before = QDateTime::currentDateTime();

    QList<QSet<int> > frequentSets =  fptree::findFrequentSet(150, 3, setList); //pack every thing into fptree::findFrequentSet

    qDebug() << "frequent sets: " << frequentSets;

    QDateTime after = QDateTime::currentDateTime();
    qDebug() << "spend time: " << after.toMSecsSinceEpoch()- before.toMSecsSinceEpoch() << "  mini seconds";

    qDebug() << "========== exam ==========";
    for(int i=0; i<frequentSets.size(); i++){
        QSet<int> findee = frequentSets.at(i);
        int findCount = countSub( findee, setList);
        qDebug() << findee << " : " << findCount;
    }


    return a.exec();
}
