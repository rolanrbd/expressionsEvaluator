#include "Expression.h"

Expression::Expression():
    opT(OPT_UNSET)
{

}

void Expression::setExpression(QString s)
{
    if(s.isEmpty())
        return;

    exp = s;
    opT = toPostorder();
}

QString Expression::toString()
{
    QString result = "";
    if(expPostorder.isEmpty())
        return result;
    else
    {
        foreach (TokenItemType tkn, expPostorder) {
            result += tkn.value + " ";
        }
    }
    return result;
}

OperationType Expression::toPostorder()
{
    OperationType opType = OPT_UNSET;

    QStringList items = exp.split(" ");
    QVector<TokenItemType> expression;
    QVector<TokenItemType> stackOp;
    TokenItemType tkn;
    QString index = 0;

    if((items[0] == "+" || items[0] == "-" || items[0] == "*") &&
            items[1] == "(" && items[items.size() - 1] == ")")
    {
        opType = items[0] == "+" ?
                    OPT_ADD :
                    items[0] == "-" ? OPT_DEL : OPT_KEEP;
        items.removeLast();
        items.removeFirst();
        items.removeFirst();
    }

    foreach (QString s, items)
    {
        s = s.trimmed();

        if(s == "==" || s == ">" || s == "<" || s == ">=" || s == "<=" || s == "!=")
        {
            tkn.type = TKT_OP_R;
            tkn.priority = 5;
            tkn.value = s;
        }
        else if(s == "*" || s == "/")
        {
            tkn.type = TKT_OP_A;
            tkn.priority = 3;
            tkn.value = s;
        }
        else if(s == "+" || s == "-")
        {
            tkn.type = TKT_OP_A;
            tkn.priority = 4;
            tkn.value = s;
        }
        else if(s == "&&" || s == "||")
        {
            tkn.type = TKT_OP_L;
            tkn.priority = 6;
            tkn.value = s;
        }
        else if(s == "!")
        {
            tkn.type = TKT_OP_R;
            tkn.priority = 7;
            tkn.value = s;
        }
        else if(isPosition(s, index))
        {
            tkn.type = TKT_POS;
            tkn.priority = 10;
            tkn.value = s;
        }
        else if(isNumber(s))
        {
            tkn.type = TKT_LIT;
            tkn.priority = 10;
            tkn.value = s;
        }
        else if(s == ")")
        {
            tkn.type = TKT_PAR_D;
            tkn.priority = 2;
            tkn.value = s;
        }
        else if(s == "(")
        {
            tkn.type = TKT_PAR_I;
            tkn.priority = 1;
            tkn.value = s;
        }
        else
            continue;

        expression.push_back(tkn);
    }

    while (!expression.isEmpty())
    {
        tkn = expression.front();
        expression.pop_front();

        switch (tkn.priority) {
            case 1:
                stackOp.push_back(tkn);
            break;
            case 2:
                while (stackOp.last().type != TKT_PAR_I) {
                    tkn = stackOp.last();
                    stackOp.pop_back();
                    if(tkn.type != TKT_PAR_I)
                        expPostorder.push_back(tkn);
                }
                stackOp.pop_back();
            break;
            case 10:
                expPostorder.push_back(tkn);
            break;
            case 3:
            case 4:
            case 5:
            case 6:
                while (!stackOp.isEmpty() && stackOp.last().type != TKT_PAR_I
                       && stackOp.last().priority <= tkn.priority)
                {
                    expPostorder.push_back(stackOp.last());
                    stackOp.pop_back();
                }

                stackOp.push_back(tkn);
            break;
            default:
                break;
        }
    }
    while (!stackOp.isEmpty()) {
        expPostorder.push_back(stackOp.last());
        stackOp.pop_back();
    }

    return opType;
}

bool Expression::isPosition(QString s, QString &p)
{
    if(s.size() > 2 || s.size() < 2 || s[0].toUpper() != 'P')
        return false;
    p = s[1];
    return true;
}

bool Expression::isNumber(QString s)
{
    return s.toInt() != 0 ? true : false;
}

bool Expression::exec(QVector<int> *P)
{

    QVector<int> stackOp;

    if(expPostorder.isEmpty())
        opT = toPostorder();

    foreach (TokenItemType tkn, expPostorder) {
        switch (tkn.type) {
            case TKT_OP_A:
                if(tkn.value == "+" )
                {
                    int x,y;
                    x = stackOp.last(); stackOp.pop_back();
                    y = stackOp.last(); stackOp.pop_back();
                    stackOp.push_back(x + y);
                }
                else if(tkn.value == "-")
                {
                    int x,y;
                    x = stackOp.last(); stackOp.pop_back();
                    y = stackOp.last(); stackOp.pop_back();
                    stackOp.push_back(x - y);
                }
                else if(tkn.value == "*")
                {
                    int x,y;
                    x = stackOp.last(); stackOp.pop_back();
                    y = stackOp.last(); stackOp.pop_back();
                    stackOp.push_back(x * y);
                }
                else if(tkn.value == "/" )
                {
                    int x,y;
                    x = stackOp.last(); stackOp.pop_back();
                    y = stackOp.last(); stackOp.pop_back();
                    stackOp.push_back(x / y);
                }
            break;
        case TKT_OP_R:
            if(tkn.value == "==")
            {
                int x,y;
                x = stackOp.last(); stackOp.pop_back();
                y = stackOp.last(); stackOp.pop_back();
                stackOp.push_back(x == y ? 1 : 0);
            }
            else if(tkn.value == ">=")
            {
                int x,y;
                x = stackOp.last(); stackOp.pop_back();
                y = stackOp.last(); stackOp.pop_back();
                stackOp.push_back(x >= y ? 1 : 0);
            }
            else if(tkn.value == "<=" )
            {
                int x,y;
                x = stackOp.last(); stackOp.pop_back();
                y = stackOp.last(); stackOp.pop_back();
                stackOp.push_back(x <= y ? 1 : 0);
            }
            else if(tkn.value == "!=" )
            {
                int x,y;
                x = stackOp.last(); stackOp.pop_back();
                y = stackOp.last(); stackOp.pop_back();
                stackOp.push_back(x != y ? 1 : 0);
            }
            else if(tkn.value == ">" )
            {
                int x,y;
                x = stackOp.last(); stackOp.pop_back();
                y = stackOp.last(); stackOp.pop_back();
                stackOp.push_back(x > y ? 1 : 0);
            }
            else if(tkn.value == "<" )
            {
                int x,y;
                x = stackOp.last(); stackOp.pop_back();
                y = stackOp.last(); stackOp.pop_back();
                stackOp.push_back(x < y ? 1 : 0);
            }
        break;
        case TKT_OP_L:
            if(tkn.value == "&&" )
            {
                int x,y;
                x = stackOp.last(); stackOp.pop_back();
                y = stackOp.last(); stackOp.pop_back();
                stackOp.push_back(x && y ? 1 : 0);
            }
            else if(tkn.value == "||")
            {
                int x,y;
                x = stackOp.last(); stackOp.pop_back();
                y = stackOp.last(); stackOp.pop_back();
                stackOp.push_back(x || y ? 1 : 0);
            }
            else if(tkn.value == "!")
            {
                int x;
                x = stackOp.last(); stackOp.pop_back();
                stackOp.push_back(! x);
            }
        break;
        case TKT_LIT:
            stackOp.push_back(tkn.value.toInt());
        break;
        case TKT_POS:
            int index = TKN::posToInt(tkn.value);
            stackOp.push_back((*P)[index]);
         break;
        }
    }
    return stackOp.last();
}

bool Expression::exec(QVector<quint16> *P)
{
    QVector<int>* q = new QVector<int>();
    foreach (quint16 c, *P) {
        q->push_back(static_cast<int>(c));
    }
    bool rslt = exec(q);
    q->clear();
    delete q;
    return rslt;
}

bool Expression::exec(QVector<QString> *P)
{
    QVector<int>* q = new QVector<int>();
    foreach (QString c, *P) {
        q->push_back(c.toInt());
    }
    bool rslt = exec(q);
    q->clear();
    delete q;
    return rslt;
}

bool Expression::exec(QList<QPair<int, QString> > P)
{
    QVector<int>* q = new QVector<int>();
    for(int i = 0; i < P.size(); ++i) {
        q->push_back(P[i].first);
    }
    bool rslt = exec(q);
    q->clear();
    delete q;
    return rslt;
}
