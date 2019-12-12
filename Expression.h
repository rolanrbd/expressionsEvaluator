#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <QString>
#include <QVector>
#include <QPair>

#include "Enums/OperationType.h"
#include "Structs/TokenItemType.h"

class Expression
{
private:
    QString exp;
    QVector<TokenItemType> expPostorder;
    OperationType opT;
    OperationType toPostorder();
    bool isPosition(QString s, QString &p);
    bool isNumber(QString s);
public:
    Expression();

    void setExpression(QString s);
    QString getExpression(){return exp;}
    OperationType getOperationType(){return opT;}
    QString toString();
    bool exec(QVector<int> *P);
    bool exec(QVector<quint16> *P);
    bool exec(QVector<QString> *P);
    bool exec(QList<QPair<int, QString>> P);
};

#endif // EXPRESSION_H
