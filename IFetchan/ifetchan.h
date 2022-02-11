#pragma once
#include <QtWidgets/QMainWindow>
#include <vector>
#include "ui_ifetchan.h"
#include "imageframe.h"
#include "fourchan.h"


class IFetchan : public QMainWindow
{
    Q_OBJECT

public:
    IFetchan(QWidget *parent = Q_NULLPTR);

private slots:
    void on_load_pushButton_clicked();
    void on_open_pushButton_clicked();
    void on_clear_pushButton_clicked();
    void on_download_pushButton_clicked();

signals:
    void updateImages(std::vector<std::string>, std::string);
    void loadingFinished();
    void debugMsg(std::string);
    void setPBar(int);
    void incrementPBar();

private:
    int m_locX = 0;
    int m_locY = 0;

    std::vector<ImageFrameBox*> m_currentFrames;
    Ui::IFetchanClass ui;

    void buttons_locked(bool state);
    void handle_consoleDebug(std::string);
    void handle_setMaxProgressBar(int value);
    void handle_incrementProgressBar();
    void handle_loading_finished();
    void handle_new_frame(std::vector<std::string> im, std::string imgbytes_thumbnail);

    void load_images(std::string board, std::string threadid);
    void download_images();
};
