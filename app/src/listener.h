#pragma once
#include <string>

class GUIListener {
public:
  virtual ~GUIListener() {}

  virtual void onButtonClick() {};
};

class EngineListener {
public:
  virtual ~EngineListener() {}

  virtual void onShowEvent(bool show) {};
};
