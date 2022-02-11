#include "fourchan.h"
#include "ifetchan.h"
#include <QFileDialog>
#include <QtWidgets/QGroupBox>
#include <vector>
#include <thread>
#include <fstream>
#include "imageframe.h"

IFetchan::IFetchan(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.gridLayout_3->setSpacing(5);
    QPalette palette = ui.thread_lineEdit->palette();
    palette.setColor(QPalette::PlaceholderText, QColor(128, 128, 128));
    palette.setColor(QPalette::Text, QColor(255, 255, 255));
    ui.thread_lineEdit->setPalette(palette);

    QObject::connect(this, &IFetchan::updateImages, this, &IFetchan::handle_new_frame);
    QObject::connect(this, &IFetchan::loadingFinished, this, &IFetchan::handle_loading_finished);
    QObject::connect(this, &IFetchan::incrementPBar, this, &IFetchan::handle_incrementProgressBar);
    QObject::connect(this, &IFetchan::setPBar, this, &IFetchan::handle_setMaxProgressBar);
    QObject::connect(this, &IFetchan::debugMsg, this, &IFetchan::handle_consoleDebug);
}

void IFetchan::buttons_locked(bool state) {
    ui.load_pushButton->setDisabled(state);
    ui.download_pushButton->setDisabled(state);
    ui.clear_pushButton->setDisabled(state);
    ui.open_pushButton->setDisabled(state);
}

void IFetchan::handle_consoleDebug(std::string msg) {
    ui.debug_plainTextEdit->appendPlainText(QString::fromStdString(msg));
}

void IFetchan::handle_setMaxProgressBar(int value) {
    ui.status_progressBar->setMaximum(value);
}

void IFetchan::handle_incrementProgressBar() {
    ui.status_progressBar->setValue(ui.status_progressBar->value() + 1);
}

void IFetchan::handle_new_frame(std::vector<std::string> im, std::string imgbytes_thumbnail) {
    ImageFrameBox* imageframe = new ImageFrameBox(im[0] + im[1], im[3], im[2], imgbytes_thumbnail);
    imageframe->moveToThread(ui.centralWidget->thread());
    imageframe->CreateFrame(ui.scrollAreaWidgetContents);
    m_currentFrames.push_back(imageframe);

    ui.gridLayout_3->addWidget(imageframe, m_locY, m_locX, 1, 1);
    ui.frames_scrollArea->setWidget(ui.scrollAreaWidgetContents);

    m_locX += 1;

    if (m_locX == 4) {
        m_locY += 1;
        m_locX = 0;
    }
}

void IFetchan::handle_loading_finished() {
    ui.status_progressBar->setValue(0);
    buttons_locked(false);
}

void IFetchan::load_images(std::string board, std::string threadid) {
    FourchanREST api;
    FourchanResult results = api.GetThreadImages(board, threadid);

    emit setPBar(results.images.size());

    for (auto& im : results.images) {
        std::string imgbytes_thumbnail = api.GetImageBytes(im[3]);
        if (imgbytes_thumbnail != "") {
            emit debugMsg("Loading " + im[0] + im[1]);
            emit updateImages(im, imgbytes_thumbnail);
        }

        emit incrementPBar();
    }

    emit loadingFinished();
}

void IFetchan::download_images() {
    if (m_currentFrames.empty())
        return;

    FourchanREST api;
    emit setPBar(m_currentFrames.size());
    for (auto& imgf : m_currentFrames) {
        if (imgf->isChecked()) {
            std::string imgbytes_fullpic = api.GetImageBytes(imgf->full_url);
            std::string fullpath = ui.output_lineEdit->text().toStdString() + "/" + imgf->filename;

            QFile imageFile = QFile(QString::fromStdString(fullpath));
            imageFile.open(QIODevice::WriteOnly);
            imageFile.write(QByteArray::fromStdString(imgbytes_fullpic));
            imageFile.close();
            emit debugMsg("Downloaded " + imgf->filename);
            emit incrementPBar();
        }
    }
    emit loadingFinished();
}

void IFetchan::on_load_pushButton_clicked() {
    QString threadinput = ui.thread_lineEdit->text();
    
    std::string board;
    std::string threadid;

    if (threadinput.contains("http") || threadinput.contains("/")) {
        QStringList split = threadinput.split("/");
        threadid = split.back().toStdString();
        board = split[split.length() - 3].toStdString();
    }

    std::thread *thread = new std::thread(&IFetchan::load_images, this, board, threadid);
    buttons_locked(true);
}

void IFetchan::on_download_pushButton_clicked() {
    std::thread* thread = new std::thread(&IFetchan::download_images, this);
    buttons_locked(true);
}

void IFetchan::on_open_pushButton_clicked() {
    QString outputDir = QFileDialog::getExistingDirectory();
    ui.output_lineEdit->setText(outputDir);
    emit debugMsg("Setting outptudir to " + outputDir.toStdString());
}

void IFetchan::on_clear_pushButton_clicked() {
    while (ui.gridLayout_3->count()) {
        QLayoutItem *child = ui.gridLayout_3->takeAt(0);
        if (child->widget()) {
            child->widget()->deleteLater();
        }
    }

    if (!m_currentFrames.empty()) {
        m_currentFrames.clear();
    }

    m_locX = 0;
    m_locY = 0;

    ui.download_pushButton->setDisabled(true);
    ui.clear_pushButton->setDisabled(true);
}