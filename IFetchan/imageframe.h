#pragma once
#include <iostream>
#include <QtWidgets/QGroupBox>

class ImageFrameBox : public QGroupBox
{
public:
    std::string filename;
    std::string full_url;
    std::string img_bytes;

    ImageFrameBox(std::string filename, std::string thumbnail_url, std::string full_url, std::string imagebytes);
    void CreateFrame(QWidget* parent = (QWidget*) nullptr);
};