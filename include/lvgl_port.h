#pragma once
#include <Arduino.h>
#include "ESP_Panel_Board_Custom.h"
#include "ESP_Panel_Conf.h"
#include <ESP_Panel_Library.h>
#include <ESP_IOExpander_Library.h>
//#include "lv_conf.h"
#include <lvgl.h>

#define TFT_HOR_RES 480
#define TFT_VER_RES 480
#define BYTE_PER_PIXEL (LV_COLOR_FORMAT_GET_SIZE(LV_COLOR_FORMAT_RGB565)) /*will be 2 for RGB565 */

ESP_Panel *panel = nullptr;
ESP_PanelLcd *lcd = nullptr;
ESP_PanelTouch *touch = nullptr;
ESP_IOExpander *expander = nullptr;
ESP_PanelBacklight *backLight = nullptr;

static uint32_t tick(void);
static void displayInit(void);
static void displayFlush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map);
static void touchRead(lv_indev_t *indev, lv_indev_data_t *data);

uint32_t tick(void)
{
    return millis();
}

void displayInit(void)
{
    //Init ESP_Panel first
    panel = new ESP_Panel();
    panel->init();
    panel->begin();
    lcd = panel->getLcd();
    touch = panel->getTouch();
    backLight = panel->getBacklight();
    expander = panel->getExpander();

    
    //Start LVGL
    lv_init();
    lv_display_t* disp = lv_display_create(TFT_HOR_RES,TFT_VER_RES);
    lv_display_set_flush_cb(disp,displayFlush);
    lv_tick_set_cb(tick);

    //Buffers
    // LV_ATTRIBUTE_MEM_ALIGN
    // static uint8_t buf1[TFT_HOR_RES * TFT_VER_RES * BYTE_PER_PIXEL/2];
    
    // LV_ATTRIBUTE_MEM_ALIGN
    // static uint8_t buf2[TFT_HOR_RES * TFT_VER_RES * BYTE_PER_PIXEL/2];

    //Partial rendering
    // uint32_t buffer_size = TFT_HOR_RES * 120 *lv_color_format_get_size(lv_display_get_color_format(disp));
    // void *buf[2] = {nullptr};
    // buf[0] = heap_caps_malloc(buffer_size, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    // assert(buf[0]);
    // buf[1] = heap_caps_malloc(buffer_size, MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
    // assert(buf[1]);
    // lv_display_set_buffers(disp,buf[0],buf[1],buffer_size,LV_DISPLAY_RENDER_MODE_PARTIAL);

    //Direct or Full mode
    // uint32_t buffer_size = TFT_HOR_RES * TFT_VER_RES*lv_color_format_get_size(lv_display_get_color_format(disp));
    // void *buf[2] = {nullptr};
    // buf[0] = heap_caps_malloc(buffer_size, MALLOC_CAP_SPIRAM);
    // assert(buf[0]);
    // buf[1] = heap_caps_malloc(buffer_size, MALLOC_CAP_SPIRAM);
    // assert(buf[1]);
    // lv_display_set_buffers(disp,buf[0],buf[1],buffer_size,LV_DISPLAY_RENDER_MODE_FULL);

    //Third method for RGB screens
    uint32_t buffer_size = TFT_HOR_RES * TFT_VER_RES * lv_color_format_get_size(lv_display_get_color_format(disp));
    void *buf[2] = {nullptr};
    buf[0] = lcd->getRgbBufferByIndex(0);
    buf[1] = lcd->getRgbBufferByIndex(1);
    lv_display_set_buffers(disp,buf[0],buf[1],buffer_size,LV_DISPLAY_RENDER_MODE_DIRECT);

    //Prepare the Touch pointer
    lv_indev_t *indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    lv_indev_set_read_cb(indev,touchRead);


}

void displayFlush(lv_display_t *disp, const lv_area_t *area, uint8_t *px_map)
{
    const int offsetx1 = area->x1;
    const int offsetx2 = area->x2;
    const int offsety1 = area->y1;
    const int offsety2 = area->y2;

    //lv_draw_sw_rgb565_swap(px_map,lv_area_get_size(area));
    lcd->drawBitmap(offsetx1,offsety1,(offsetx2-offsetx1+1),(offsety2-offsety1+1),(const uint8_t*)px_map);

    lv_display_flush_ready(disp);
}

static void touchRead(lv_indev_t *indev, lv_indev_data_t *data)
{
    ESP_PanelTouchPoint point;
    int pointsRead = touch->readPoints(&point,1);
    if(pointsRead>0)
    {
        data->point.x = point.x;
        data->point.y = point.y;
        data->state = LV_INDEV_STATE_PRESSED;
    }
    else
    {
        data->state = LV_INDEV_STATE_RELEASED;
    }
}