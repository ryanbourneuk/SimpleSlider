//
//  SimpleSlider.cpp
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

#include "SimpleSlider.h"

SimpleSlider::SimpleSlider()
: bar_unselected()
, bar_selected()
, handle()
, valueLabel()
, sliderValue(0)
, sliderEnabled(false)
, sliderInUse(false)
, sliderWillChangeOnEnd(false)
, sliderActivatedCallback()
, sliderChangingCallback()
, sliderChangedCallback()
{ }

SimpleSlider::~SimpleSlider() { }

SimpleSlider *SimpleSlider::create(int startValue, string unselectedSpriteName, string selectedSpriteName, string handleSpriteName, string fontName, SSLabelFormatTypes formatType, string lowDefault, string midDefault, string highDefault, const function<void()> &sliderActivated, const function<void(int sliderValue)> &sliderChanging, const function<void(int sliderValue)> &sliderChanged) {
  SimpleSlider *pRet = new(nothrow) SimpleSlider();
  
  if (pRet && pRet->init(startValue, unselectedSpriteName, selectedSpriteName, handleSpriteName, fontName, formatType, lowDefault, midDefault, highDefault, sliderActivated, sliderChanging, sliderChanged)) {
    pRet->autorelease();
  } else {
    CC_SAFE_DELETE(pRet);
  }
  
  return pRet;
}

bool SimpleSlider::init(int startValue, string unselectedSpriteName, string selectedSpriteName, string handleSpriteName, string fontName, SSLabelFormatTypes formatType, string lowDefault, string midDefault, string highDefault, const function<void()> &sliderActivated, const function<void(int sliderValue)> &sliderChanging, const function<void(int sliderValue)> &sliderChanged) {
  if (!Node::init()) return false;
  
  // Set initial variable values.
  sliderValue = startValue;
  labelFormatType = formatType;
  
  default_0 = lowDefault;
  default_50 = midDefault;
  default_100 = highDefault;
  
  sliderActivatedCallback = sliderActivated;
  sliderChangingCallback = sliderChanging;
  sliderChangedCallback = sliderChanged;
  
  // Set content size of node.
  auto winSize = Director::getInstance()->getVisibleSize();
  auto contentSize = Size(winSize.width, winSize.height * .1);
  this->setContentSize(contentSize);
  
  this->setAnchorPoint(Vec2(.5, .5));
  
  // Cascade the node opacity down to its children, so the SimpleSlider can fade in and out.
  this->setCascadeOpacityEnabled(true);
  
  // Create the unselected bar.
  bar_unselected = Sprite::createWithSpriteFrameName(unselectedSpriteName);
  this->addChild(bar_unselected,0);
  bar_unselected->setAnchorPoint(Vec2(0, .5));
  bar_unselected->setPosition(Vec2(contentSize.width * .075, contentSize.height * .5));
  
  // Create the selected bar.
  bar_selected = Sprite::createWithSpriteFrameName(selectedSpriteName);
  this->addChild(bar_selected,1);
  bar_selected->setAnchorPoint(Vec2(0, .5));
  bar_selected->setPosition(bar_unselected->getPosition());
  
  // Create the slider handle.
  handle = Sprite::createWithSpriteFrameName(handleSpriteName);
  this->addChild(handle,10);
  handle->setPosition(getPositionFor(sliderValue));
  
  // Create the label to the right of the slider.
  valueLabel = Label::createWithTTF("0%", fontName, handle->getContentSize().height*.8);
  this->addChild(valueLabel);
  valueLabel->setAnchorPoint(Vec2(.5, .5));
  
  // Get the midpoint between the end of the slider bar and the edge of the screen, for the label position.
  auto barEnd = Vec2(bar_unselected->getPosition().x+bar_unselected->getContentSize().width, bar_unselected->getPosition().y);
  valueLabel->setPosition(barEnd.getMidpoint(Vec2(contentSize.width, bar_unselected->getPosition().y)));
  
  // Calculate the label width, so the text always fits the gap between the slider and the edge of the screen.
  auto labelWidth = (contentSize.width-(bar_unselected->getPosition().x+bar_unselected->getContentSize().width))*.85;
  valueLabel->setDimensions(labelWidth, handle->getContentSize().height);
  valueLabel->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
  valueLabel->setOverflow(Label::Overflow::SHRINK);
  
  // Create touch listeners.
  auto touchListener = EventListenerTouchOneByOne::create();
  
  touchListener->onTouchBegan = [&](Touch *touch, Event *event) {
    return this->touchBegan(touch, event);
  };
  
  touchListener->onTouchMoved = [&](Touch *touch, Event *event) {
    this->touchMoved(touch, event);
  };
  
  touchListener->onTouchEnded = [&](Touch *touch, Event *event) {
    this->touchEnded(touch, event);
  };
  
  touchListener->onTouchCancelled = touchListener->onTouchEnded;
  
  _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
  
  // Update the selected bar to match the handle.
  this->updateBarWith(sliderValue);
  
  return true;
}

bool SimpleSlider::touchBegan(Touch *touch, Event *event) {
  if (!sliderEnabled || !this->getBoundingBox().containsPoint(touch->getLocation())) {
    return false;
  }
  
  auto touchLocation = this->convertToNodeSpace(touch->getLocation());
  
  if (!bar_unselected->getBoundingBox().containsPoint(touchLocation) && !handle->getBoundingBox().containsPoint(touchLocation)) {
    return false;
  }
  
  sliderInUse = true;
  
  if (sliderActivatedCallback != nullptr) {
    sliderActivatedCallback();
  }
  
  this->updateSliderWith(touchLocation);
  
  if (sliderChangingCallback != nullptr) {
    sliderChangingCallback(sliderValue);
  }
  
  return true;
}

void SimpleSlider::touchMoved(Touch *touch, Event *event) {
  if (!sliderEnabled || !sliderInUse) return;
  
  auto touchLocation = this->convertToNodeSpace(touch->getLocation());
  
  this->updateSliderWith(touchLocation);
  
  if (sliderChangingCallback != nullptr) {
    sliderChangingCallback(sliderValue);
  }
}

void SimpleSlider::touchEnded(Touch *touch, Event *event) {
  if (!sliderEnabled || !sliderInUse) return;
  
  if (sliderWillChangeOnEnd) {
    auto touchLocation = this->convertToNodeSpace(touch->getLocation());
    
    this->updateSliderWith(touchLocation);
  }
  
  if (sliderChangingCallback != nullptr) {
    sliderChangingCallback(sliderValue);
  }
  
  sliderChangedCallback(sliderValue);
  
  sliderInUse = false;
}

void SimpleSlider::sliderShouldFinish() {
  sliderChangedCallback(sliderValue);
}

void SimpleSlider::setSliderPositionWith(Vec2 touchLocation) {
  auto barPos = bar_unselected->getPosition();
  
  handle->setPosition(Vec2(clampf(touchLocation.x , barPos.x, barPos.x + bar_unselected->getContentSize().width), barPos.y));
}

void SimpleSlider::updateSliderWith(Vec2 touchLocation) {
  this->setSliderPositionWith(touchLocation);
  
  sliderValue = calculateNewSliderValue();
  
  this->updateBarWith(sliderValue);
}

void SimpleSlider::updateBarWith(int sliderValue) {
  bar_selected->setScaleX((float)sliderValue / 100);
  
  if (sliderValue == 0 && !default_0.empty()) {
    valueLabel->setString(default_0);
  } else if (sliderValue == 50 && !default_50.empty()) {
    valueLabel->setString(default_50);
  } else if (sliderValue == 100 && !default_100.empty()) {
    valueLabel->setString(default_100);
  } else {
    switch (labelFormatType) {
      case LabelFormatDecimal:
        valueLabel->setString(StringUtils::format("%.2f", (float)sliderValue / 100));
        break;
        
      default:
        valueLabel->setString(StringUtils::format("%i%%", sliderValue));
        break;
    }
  }
}

int SimpleSlider::calculateNewSliderValue() {
  return (int)clampf(((handle->getPosition().x - bar_unselected->getPosition().x) / bar_unselected->getContentSize().width) * 100, 0, 100);
}

Vec2 SimpleSlider::getPositionFor(int sliderValue) {
  auto barPos = bar_unselected->getPosition();
  
  return Vec2(barPos.x+(bar_unselected->getContentSize().width * ((float)sliderValue / 100)), barPos.y);
}

void SimpleSlider::setEnabled(bool enabled) {
  sliderEnabled = enabled;
}

void SimpleSlider::setSliderWillChangeOnEnd(bool willChange) {
  sliderWillChangeOnEnd = willChange;
}
