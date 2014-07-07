//
//  commonUI.h
//  LengendOfFightDemons
//
//  Created by tanzuoliang on 14-5-21.
//
//

#ifndef __LengendOfFightDemons__commonUI__
#define __LengendOfFightDemons__commonUI__

#include <iostream>
#include "cocos2d.h"
#include "ui/CocosGUI.h"

#define FRAME_UI_CORNER_NAME                "thecorner.png"
#define FRAME_UI_LINE_NAME                  "theline.png"
#define FRAME_CLOSE_NORMAL_NAME             "heroui_close.png"
#define FRAME_CLOSE_PRESS_NAME              "heroui_close_press.png"
#define FRAME_NORMAL_TITLE_NAME             "heroui_title.png"
#define FRAME_BEAUTIFUL_TITLE_NAME          "pubui_titleframe_1.png"
#define FRAME_TITLE_LINE_NAME               "pubui_titleframe_2.png"

#define FRAME_BAG_BUTTOM_LINE               "backspaceui_floordown.png"
#define FRAME_BAG_TOP_LINE                  "backspaceui_floorup.png"

typedef enum
{
    NORMAL,
    BEAUTIFUL,
    BAG,
    NO_TITLE
}TitleStyle;

class BaseUIFrame:public cocos2d::Node
{
public:
    CREATE_FUNC(BaseUIFrame);
    bool init();
    cocos2d::ui::Button* closeBtn;
    cocos2d::LayerColor* colorLayer;
    void setFrameSize(float width,float height,std::string title="",TitleStyle titleStyle=TitleStyle::NORMAL,std::string bgFrameName="",cocos2d::ui::Widget* widget=nullptr);
private:
    
};


#endif /* defined(__LengendOfFightDemons__commonUI__) */
