//
//  SimpleSlider.h
//
//  Copyright (c) 2017 Ryan Bourne. All rights reserved.
//  https://www.ryanbournedev.wordpress.com & https://twitter.com/ryanbourne28
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy
//  of this software and associated documentation files (the "Software"), to deal
//  in the Software without restriction, including without limitation the rights
//  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//  copies of the Software, and to permit persons to whom the Software is
//  furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//  THE SOFTWARE.
//

#ifndef SimpleSlider_h
#define SimpleSlider_h

#include "cocos2d.h"

using namespace std;
using namespace cocos2d;
using namespace ui;

typedef enum {
  LabelFormatPercentage,
  LabelFormatDecimal
} SSLabelFormatTypes;

class SimpleSlider: public Node {
public:
  SimpleSlider();
  ~SimpleSlider();
  
  static SimpleSlider *create(int startValue, string unselectedSpriteName, string selectedSpriteName, string handleSpriteName, string fontName, SSLabelFormatTypes formatType, string lowDefault, string midDefault, string highDefault, const function<void()> &sliderActivated, const function<void(int sliderValue)> &sliderChanging, const function<void(int sliderValue)> &sliderChanged);
  
  void setEnabled(bool enabled);
  void setSliderWillChangeOnEnd(bool willChange);
  
  void sliderShouldFinish();
  
private:
  virtual bool init(int startValue, string unselectedSpriteName, string selectedSpriteName, string handleSpriteName, string fontName, SSLabelFormatTypes formatType, string lowDefault, string midDefault, string highDefault, const function<void()> &sliderActivated, const function<void(int sliderValue)> &sliderChanging, const function<void(int sliderValue)> &sliderChanged);
  
  bool touchBegan(Touch *touch, Event *event);
  void touchMoved(Touch *touch, Event *event);
  void touchEnded(Touch *touch, Event *event);
  
  void setSliderPositionWith(Vec2 touchLocation);
  
  void updateSliderWith(Vec2 touchLocation);
  void updateBarWith(int sliderValue);
  
  int calculateNewSliderValue();
  Vec2 getPositionFor(int sliderValue);
  
  Sprite *bar_unselected;
  Sprite *bar_selected;
  Sprite *handle;
  
  Label *valueLabel;
  
  SSLabelFormatTypes labelFormatType;
  
  int sliderValue;
  
  bool sliderEnabled;
  bool sliderInUse;
  bool sliderWillChangeOnEnd;
  
  string default_0;
  string default_50;
  string default_100;
  
  function<void()> sliderActivatedCallback;
  function<void(int sliderValue)> sliderChangingCallback;
  function<void(int sliderValue)> sliderChangedCallback;
};

#endif /* SimpleSlider_h */
