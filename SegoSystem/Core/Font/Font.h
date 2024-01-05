#pragma once
#include <string>
#include <unordered_map>
#include "freetype/ftglyph.h"

class Texture2D;
class Font
{
public:
    Font* LoadFormFile(std::string font_file_path,uint16_t font_size);

    /// 获取Font实例
    /// param font_file_path ttf路径
    /// return
    static Font* GetFont(std::string font_file_path);
    //freetype 为字符生成bitmap
    void LoadCharacter(char ch);

 Texture2D* font_texture(){return font_texture_;}

private:
    uint16_t font_size_ = 20;//默认字体大小
    char* font_file_buffer_ = nullptr; //ttf字体加载到内存
    FT_Library ft_library_;
    FT_Face ft_face_;
    Texture2D* font_texture_;
    unsigned short font_texture_size_=1024;
private:
    static std::unordered_map<std::string,Font*> font_map_;//存储加载的字体 key：ttf路径 value：Font实例

};
