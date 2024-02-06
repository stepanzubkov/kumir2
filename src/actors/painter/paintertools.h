#ifndef PAINTERTOOLS_H
#define PAINTERTOOLS_H

#include <QTimerEvent>
#include <QStack>
#include <QColor>
#include "../_colorer/typecolor.h"

namespace ActorPainter
{
	extern QColor parseColor(const QString &css);
	extern QString CSS_RGB_toString(const QString &cssRgb);
	extern QString findColorName(const Actor_Colorer::Color &color);
	extern const QStringList &standardRussianColorNames();
}

#endif // PAINTERTOOLS_H
