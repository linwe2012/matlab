#pragma once

#include "qapplication.h"

QWidget* GetTargetWidget();
QSize& GetCurrentLayout();
QGraphicsView* GetImageView();
QPixmap* GetPixelMap();
void ReRenderPixelMap();