# SimpleSlider
An easy to use and CRAZILY SIMPLE to implement slider control for Cocos2d-x!

## Why *SimpleSlider*?!

The *SimpleSlider* class exists to provide an incredibly simple way to implement slider-type controls into your Cocos2d-x iOS and Android game!

## But doesn't Cocos2d-x already have sliders built in?!

(shhhhhh... I didn't notice that before I built this!)

The built in slider class has a few more areas of additional functionality (different handle states for normal, selected, disabled; Scale9Sprite usage etc) which the *SimpleSlider* class does not have. However, the *SimpleSlider* makes up for it by instead having:

* A MIND-BOGGLINGLY simple implementation. You can create a slider with all the callback blocks you could ever need REALLY easily.
* Callbacks for when the slider:
    * Is initially selected - so you can play a sound effect to show the slider is selected.
    * Is in the process of being moved - so you can do 'light' functionality (like changing the current audio volume as the slider moves.
    * Has been unselected and is no longer moving - so you can do 'heavy' functionality (like saving the current audio volume to a file) which wouldn't be effecient to do many times over as the slider is moved around.
* Customisable value label, which can:
    * Can show either decimal or percent format, depending on your preference.
    * Can have custom strings set for 0%, 50% and 100% values.
    
## Here be dragons!

I am still learning C++. The *SimpleSlider* may have underlying issues which will be fixed later or is not as efficient as it could be. However, that's part of the learning process! Feel free to suggest improvements or fixes!
    
## So just how simple is it to integrate?!

To integrate the *SimpleSlider* class, simply:
1. Clone this repository, or download the two *SimpleSlider* files. Put them into your Cocos2d-x project.
2. Find the sprites you're using for your slider. For mine I created:
    * An 'unselected' sprite (a greyed out rectangle), with the 1x dimensions of 200 x 8.
    * A 'selected' sprite (a green rectangle), with the same dimensions as the unselected sprite (200 x 8).
    * A 'handle' sprite (a grey / silver) circle, with the 1x dimensions of 40 x 40.
3. Add the sprites to your project. If you're not using a spritesheet, change mentions of `createWithSpriteFrameName` to instead be `create`.
4. Where you want to integrate the slider, simply create it with:
    ```cpp
    auto slider = SimpleSlider::create(defaultValue, unselectedSpriteName, selectedSpriteName, handleSpriteName, fontName, format, zeroPercentDefault, fiftyPercentDefault, maxPercentDefault, activatedCallback, changingCallback, changedCallback);
    ```
    Where:
    * *'defaultValue'* is the starting value (in integer percent form), i.e.: 50.
    * *'unselectedSpriteName'* is the filename for the sprite that respresents the unselected slider bar.
    * *'selectedSpriteName'* is the filename for the sprite that represents the selected slider bar.
    * *'handleSpriteName'* is the filename for the sprite that represents the slider's 'handle'.
    * *'fontName' is the filename for the font used for the slider's value label.
    * *'format'* is the slider label format, either decimal or percentage: `LabelFormatDecimal` or `LabelFormatPercentage`.
    * *'zeroPercentDefault'* is the label that shows when the slider is at 0%, i.e.: "Off".
    * *'fiftyPercentDefault'* is the label that shows when the slider is at 50%, i.e.: "Default".
    * *'maxPercentDefault'* is the label that shows when the slider is at 100%, i.e: "Max".
    * *'activatedCallback'* is a callback block, which the slider calls when it is first touched either on the slider bar or the handle. Use it to play any UI sound effect for when the slider is selected.
    * *'changingCallback'* is another callback block, which the slider calls whenever the handle position changes. It passes back the slider value, which you can use in the callback block to do 'light' functionality such as changing the audio volume in real time. **Do not do 'heavy' functionality (such as file read/write) here, as this can be called a lot.**
    * *'changedCallback'* is another callback block, which the slider calls whenever the slider has been unselected and the position is set - and no changes are going to occur from that event. It passes back the slider value, which you can use in the callback block to do 'heavy' functionality, such as saving the slider value to a file to use for the audio volume later on. **Do your file read/write in this callback, as it is only called once!**
    
    This means a complete example of creating a *SimpleSlider* would be:
    ```cpp
    auto slider = SimpleSlider::create(50, "slider_unselected.png", "slider_selected.png", "slider_circle.png", "font.ttf", LabelFormatPercentage, "Off", "Default", "Max", [&]() {
      // slider activated
    }, [&](int sliderValue) {
      // slider is moving
    }, [&](int sliderValue) {
      // slider has stopped moving
    });
    ```
    
    If you wish to not have the *sliderActivated* or *sliderChanging* callbacks, you can also use:
    ```cpp
    auto slider = SimpleSlider::create(50, "slider_unselected.png", "slider_selected.png", "slider_circle.png", "font.ttf", LabelFormatPercentage, "Off", "Default", "Max", nullptr, nullptr, [&](int sliderValue) {
      // slider has stopped moving
    });
    ```
3. Add the newly created slider to your target layer/node/parent as a child, set the position of the *SimpleSlider* on the parent node.
4. Call `setEnabled()` on the slider object, with `true` or `false` to enable or disable it. This is great for if you're fading it in and out, and want it to be disabled as it's fading

## Any tips for using the *SimpleSlider* class?

* **The callbacks always receive the new slider value as an integer percent value (in the range 0% to 100%), regardless of the label format selected.**
* Be sure to call `setEnabled()` so that the slider can only be used when you want it to be used. It shouldn't be enabled during menu transitions, and as default it is created in the disabled state. You must enable it yourself.
* All parameters of the *SimpleSlider* `create(...)` function are required, apart from the *sliderActivated* and *sliderChanging* callback blocks which are optional and will function even if undefined.

## How can I ever repay you for this amazingness?!

If this has been useful to you, then great! Feel free to send me a tweet [@ryanbourne28](twitter.com/ryanbourne28) to let me know! If you really really liked it and a nice tweet just won't do, then feel free to help fund my hot chocolate cravings here: [https://www.paypal.me/ryanbourne](https://www.paypal.me/ryanbourne) - although donations are not necessary! :)
