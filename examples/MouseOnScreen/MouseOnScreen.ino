/*
  Created by Fabrizio Di Vittorio (fdivitto2013@gmail.com) - www.fabgl.com
  Copyright (c) 2019 Fabrizio Di Vittorio.
  All rights reserved.

  This file is part of FabGL Library.

  FabGL is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  FabGL is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with FabGL.  If not, see <http://www.gnu.org/licenses/>.
 */


#include "fabgl.h"


Sprite mouseSprite;

const uint8_t mouse_data[] = {
  0xff, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f,
  0xff, 0xff, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f,
  0xff, 0xc0, 0xff, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f,
  0xff, 0xc0, 0xc0, 0xff, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f,
  0xff, 0xc0, 0xc0, 0xc0, 0xff, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f,
  0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f,
  0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0x3f, 0x3f, 0x3f, 0x3f,
  0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0x3f, 0x3f, 0x3f,
  0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0x3f, 0x3f,
  0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0x3f,
  0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xc0, 0xc0, 0xff, 0x3f, 0x3f, 0xff, 0x3f, 0x3f, 0x3f, 0x3f,
  0xff, 0xc0, 0xff, 0x3f, 0xff, 0xc0, 0xc0, 0xff, 0x3f, 0x3f, 0x3f,
  0xff, 0xff, 0x3f, 0x3f, 0xff, 0xc0, 0xc0, 0xff, 0x3f, 0x3f, 0x3f,
  0xff, 0x3f, 0x3f, 0x3f, 0x3f, 0xff, 0xc0, 0xc0, 0xff, 0x3f, 0x3f,
  0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0xff, 0xc0, 0xc0, 0xff, 0x3f, 0x3f,
  0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0xff, 0xc0, 0xc0, 0xff, 0x3f,
  0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0xff, 0xc0, 0xc0, 0xff, 0x3f,
  0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0xff, 0xff, 0x3f, 0x3f,
};
const Bitmap mouse = Bitmap(11, 19, &mouse_data[0]);


int indicatorX = 300;
int indicatorY = 170;


void moveCursor()
{
  mouseSprite.moveTo(Mouse.position().X, Mouse.position().Y);

  // refresh sprites is not required when there are subsequent Canvas drawing functions
  //VGAController.refreshSprites();
  
  Canvas.setPenColor(Color::Blue);
  Canvas.setBrushColor(Color::Yellow);
  Canvas.drawTextFmt(indicatorX, indicatorY, " %d %d ", Mouse.position().X, Mouse.position().Y);
}


void setup()
{
  Serial.begin(115200); delay(500); Serial.write("\n\n\n"); // DEBUG ONLY

  // 8 colors
  //VGAController.begin(GPIO_NUM_22, GPIO_NUM_21, GPIO_NUM_19, GPIO_NUM_18, GPIO_NUM_5);

  // 64 colors
  VGAController.begin(GPIO_NUM_22, GPIO_NUM_21, GPIO_NUM_19, GPIO_NUM_18, GPIO_NUM_5, GPIO_NUM_4, GPIO_NUM_23, GPIO_NUM_15);

  VGAController.setResolution(VGA_640x350_70HzAlt1, 640, 350);
  //VGAController.setResolution(VGA_640x240_60Hz);    // select to have more free memory

  // Setup pins GPIO26 for CLK and GPIO27 for DATA
  Mouse.begin(GPIO_NUM_26, GPIO_NUM_27);
  Mouse.setAbsolutePositionArea(Canvas.getWidth(), Canvas.getHeight());

  Canvas.setBrushColor(Color::Blue);
  Canvas.clear();
  Canvas.selectFont(Canvas.getPresetFontInfo(80, 25));
  Canvas.setGlyphOptions(GlyphOptions().FillBackground(true));

  mouseSprite.addBitmap(&mouse);
  VGAController.setSprites(&mouseSprite, 1);

  moveCursor();
}


void loop()
{
  if (Mouse.updateAbsolutePosition()) {

    // left button writes pixels
    if (Mouse.buttons().left) {
      Canvas.setPenColor(Color::BrightRed);
      Canvas.setPixel(Mouse.position().X, Mouse.position().Y);
      Canvas.moveTo(Mouse.position().X, Mouse.position().Y);
    }

    // right button draw lines
    if (Mouse.buttons().right) {
      Canvas.setPenColor(Color::BrightGreen);
      Canvas.lineTo(Mouse.position().X, Mouse.position().Y);
    }

    // middle button clear screen
    if (Mouse.buttons().middle) {
      Canvas.setBrushColor(Color::Blue);
      Canvas.clear();
    }

    // mouse wheel moves cursor position indicator
    int wheelDelta = Mouse.wheelDelta();
    if (wheelDelta != 0) {
      indicatorY += wheelDelta;
      Canvas.setBrushColor(Color::Blue);
      Canvas.clear();
    }

    moveCursor();
  }
}
