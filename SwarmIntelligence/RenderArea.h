#pragma once
#include <QGraphicsView>
#include <QPen>
#include <QBrush>

class Maze;
class Marker;

namespace Ui {
   class RenderArea;
}

class RenderArea : public QGraphicsView
{
   Q_OBJECT;

public:
   RenderArea(QWidget* parent = nullptr);
   ~RenderArea();

   void addMazeToScene(const Maze& maze);
   void clear();

   bool saveScreenshot(const QString& filePath);

   void drawShortestPath(const Maze* maze);

private:
   void createTile(const uint32_t& x, const uint32_t& y, const uint32_t& tileWidth, const uint32_t& tileHeight, const QColor& tileColor);
   void createMarker(const Maze& maze, const Marker& marker);


   QColor wallColor;
   QColor notVisitedTileColor;
   QColor visitedTileColor;
   QColor shortestPathColor;
   QColor startColor;
   QColor endColor;
   QPen noPen{ Qt::NoPen };

   Ui::RenderArea* ui;
   std::vector<QGraphicsItem*> sceneElements;
   QGraphicsScene* scene;
};

