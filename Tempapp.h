#pragma once
#include <string>
#include <vector>
#include "TempD.h"


class Tempapp {
public:
   Tempapp();
   void init();


private:
   void printMenu();
   void processUserOption(int userOption);
   int getUserOption();
   void computeCandlestickData();
   void makeTextPlot();
   void filterPlotData();
   void predictPlotData();

   TempD TempD;
   std::string currentTimestamp;
};
