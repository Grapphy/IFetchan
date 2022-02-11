#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include "imageframe.h"

ImageFrameBox::ImageFrameBox(std::string filename, std::string thumbnail_url, std::string full_url, std::string imagebytes) {
	this->filename = filename;
	this->full_url = full_url;
    this->img_bytes = imagebytes;
}

void ImageFrameBox::CreateFrame(QWidget *parent) {
    this->setTitle(QString::fromStdString(this->filename));
    this->setMaximumSize(QSize(150, 150));
    this->setStyleSheet(QString::fromUtf8("color: rgb(255, 255, 255);background-color: rgb(30, 30, 30);"));
    this->setAlignment(Qt::AlignCenter);
    this->setFlat(true);
    this->setCheckable(true);

    QHBoxLayout* horizontalLayout = new QHBoxLayout(this);
    horizontalLayout->setSpacing(6);
    horizontalLayout->setContentsMargins(11, 11, 11, 11);

    QLabel* label = new QLabel(this);
    QPixmap imagemap = QPixmap();
    imagemap.loadFromData(QByteArray::fromStdString(this->img_bytes));
    label->setPixmap(imagemap);
    label->setScaledContents(true);
    horizontalLayout->addWidget(label);
}