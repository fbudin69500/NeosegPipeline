#ifndef DEF_PriorProbability
#define DEF_PriorProbability

#include <QString>
#include <QDir>

struct PriorProbability {
QString     name;
QDir*       dir;
QString     input; 
QString     output;
};

#endif
