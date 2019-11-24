#include "LeeAlgorithm.h"
#include <queue>
#include <QFile>
#include <QTextStream>


LeeAlgorithm::LeeAlgorithm()
{
}

void LeeAlgorithm::setVector(std::vector<std::vector<int>>& vector)
{
   for (uint32_t i = 0; i < this->maze->getHeight(); ++i)
   {
      std::vector<int> temp;
      for (uint32_t j = 0; j < this->maze->getWidth(); ++j)
      {
         temp.push_back(0);
      }
      vector.push_back(temp);
   }
}

struct Node
{
   int x, y, dist;
};

bool LeeAlgorithm::isValid(int curX, int curY, int nextX, int nextY)
{
   if (nextX < 0 || nextY < 0 || nextX >= this->maze->getWidth() || nextY >= this->maze->getHeight())
   {
      return false;
   }
   const auto& tile = this->maze->mazeArray[curY][curX];

   if (curX == nextX)
   {
      if (curY - nextY > 0)
      {
         if (Maze::CELL_PATH_N == (tile & Maze::CELL_PATH_N))
         {
            return true;
         }
      }
      else
      {
         if (Maze::CELL_PATH_S == (tile & Maze::CELL_PATH_S))
         {
            return true;
         }
      }
   }
   else
   {
      if (curX - nextX > 0)
      {
         if (Maze::CELL_PATH_W == (tile & Maze::CELL_PATH_W))
         {
            return true;
         }
      }
      else
      {
         if (Maze::CELL_PATH_E == (tile & Maze::CELL_PATH_E))
         {
            return true;
         }
      }
   }
   return false;
}


LeeAlgorithm::~LeeAlgorithm()
{
}

void LeeAlgorithm::solveMaze(Maze* maze)
{
   this->shortestWay.clear();
   this->leeArray.clear();

   this->maze = maze;


   const auto& startingPoint = this->maze->getStartingPoint();
   const auto& endPoint = this->maze->getEndPoint();

   setVector(this->leeArray);
   setVector(this->shortestWay);

   std::pair<int, int> direction[4];
   direction[0].first = 0; //up
   direction[0].second = -1;

   direction[1].first = 0; //down
   direction[1].second = 1;

   direction[2].first = -1; //left
   direction[2].second = 0;

   direction[3].first = 1; //right
   direction[3].second = 0;

   std::vector<std::vector<bool>> visited;
   
   for (int i = 0; i < maze->getHeight(); ++i)
   {
      visited.push_back(std::vector<bool>( this->maze->getWidth() ));
   }

  // memset(visited, 0, sizeof(visited[0][0]) * this->width * this->height);
   std::queue<Node> queueOfPoints;
   queueOfPoints.push({startingPoint.first, startingPoint.second, 0});
   visited[startingPoint.first][startingPoint.second] = true;
   leeArray[startingPoint.first][startingPoint.second] = 0;

   int xx, yy, dist;
   while (!queueOfPoints.empty())
   {
      Node temp = queueOfPoints.front();
      queueOfPoints.pop();

      for (int i = 0; i < 4; ++i)
      {
         xx = temp.x + direction[i].first;
         yy = temp.y + direction[i].second;
         dist = temp.dist;
         if (isValid(temp.x, temp.y, xx, yy) && !visited[yy][xx])
         {
            visited[yy][xx] = true;
            queueOfPoints.push({ xx, yy, dist + 1});
            leeArray[yy][xx] = dist + 1;
            //break;
         }

      }
   }

   std::pair<int, int> currPoint = endPoint;

   this->shortestWay[endPoint.second][endPoint.first] = Maze::CELL_SHORTEST;
   std::pair<int, int> lastPoint;
   std::pair<int, int> minPoint;
   std::pair<int, int> temp;
   bool first = true;
   bool notFinished = true;
   int curDist = leeArray[endPoint.second][endPoint.first];

   while (notFinished)
   {
      for (int i = 0; i < 4; ++i)
      {
         temp.first = currPoint.first + direction[i].first;
         temp.second = currPoint.second + direction[i].second;
         if (temp.first < 0 || temp.second < 0 || temp.first >= this->maze->getWidth() || temp.second >= this->maze->getHeight())
         {
            continue;
         }

         if (leeArray[temp.second][temp.first] == (curDist - 1))
         {
            minPoint.first = temp.first;
            minPoint.second = temp.second;
            curDist -= 1;
            break;
         }
      }

      lastPoint.first = currPoint.first;
      lastPoint.second = currPoint.second;

      currPoint.first = minPoint.first;
      currPoint.second = minPoint.second;
      
      this->shortestWay[currPoint.second][currPoint.first] = Maze::CELL_SHORTEST;

      if (currPoint.first == startingPoint.first && currPoint.second == startingPoint.second)
      {
         notFinished = false;
      }
   }
   
   /*while (notFinished)
   {
      bool firstLoop = true;
      for (int i = 0; i < 4; ++i)
      {
         temp.first = currPoint.first + direction[i].first;
         temp.second = currPoint.second + direction[i].second;
         if (temp.first < 0 || temp.second < 0 || temp.first >= this->width || temp.second >= this->height)
         {
            continue;
         }

         if (firstLoop || (temp.first != lastPoint.first || temp.second != lastPoint.second)) 
         {
            firstLoop = false;
            minPoint.first = temp.first;
            minPoint.second = temp.second;
         }
         else if (first || (temp.first != lastPoint.first || temp.second != lastPoint.second))
         {
            first = false;
            if (leeArray[minPoint.second][minPoint.first] > leeArray[temp.second][temp.first]) 
            {
               minPoint.first = temp.first;
               minPoint.second = temp.second;
            }
         }  
      }

      lastPoint.first = currPoint.first;
      lastPoint.second = currPoint.second;

      currPoint.first = minPoint.first;
      currPoint.second = minPoint.second;

      if (currPoint.first == endPoint.first && currPoint.second == endPoint.second)
      {
         notFinished = false;
      }

      this->shortestWay[lastPoint.second][lastPoint.first] = Maze::CELL_SHORTEST;

   }*/


   maze->shortestWayArray = this->shortestWay;
}

std::vector<std::vector<int>> LeeAlgorithm::getLeesArray() const
{
   return this->leeArray;
}
