#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGLWidget>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    timer->setInterval(1000/60);

    startGame();

}

void MainWindow::createGLScene(){
    qDebug()<<"Creating GL Scene";
    glscene = new GLScene(this);
    ui->frameL->layout()->addWidget(glscene);

    connect(timer,SIGNAL(timeout()),glscene,SLOT(updateGL()));

    connect(ui->actionZoomIn,  SIGNAL(triggered()), glscene, SLOT(zoomIn()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), glscene, SLOT(zoomOut()));

    connect(glscene,SIGNAL(mouseLeftButtonPressed(QPointF)), world,SLOT(grabItem(QPointF)));
    connect(glscene,SIGNAL(mouseRightButtonPressed(QPointF)),world,SLOT(createBox(QPointF)));
    connect(glscene,SIGNAL(mouseLeftButtonReleased()),       world,SLOT(dropItem()));
    connect(glscene,SIGNAL(mouseMoved(QPointF)),             world,SLOT(moveItem(QPointF)));
    connect(glscene,SIGNAL(keyPressed(int)),                 world,SLOT(handleKeyPressed(int)));
    connect(glscene,SIGNAL(keyReleased(int)),                world,SLOT(handleKeyReleased(int)));

    connect(world,SIGNAL(itemCreated(QBox2DItem*)),  glscene,SLOT(addItem(QBox2DItem*)));
    connect(world,SIGNAL(itemDestroyed(QBox2DItem*)),glscene,SLOT(removeItem(QBox2DItem*)));
    connect(world,SIGNAL(gameFinished()), this, SLOT(restartGame()));
    connect(glscene,SIGNAL(initialized()),world,SLOT(populate()));
}

void MainWindow::createQScene(){
    scene = new QScene(this);
    //scene->setItemIndexMethod(QGraphicsScene::NoIndex);
    //scene->setBackgroundBrush(Qt::white);
    //scene->setStickyFocus(false);

    view = new QBox2DView(ui->frameR);
    view->setScene(scene);
    //view->fitInView(QRectF(0,0,100,100),Qt::KeepAspectRatioByExpanding);
    ui->frameR->layout()->addWidget(view);

    connect(timer,SIGNAL(timeout()),scene,SLOT(advance()));

    connect(ui->actionZoomIn,  SIGNAL(triggered()), view, SLOT(zoomIn()));
    connect(ui->actionZoomOut, SIGNAL(triggered()), view, SLOT(zoomOut()));

    connect(view,SIGNAL(mouseLeftButtonPressed(QPointF)), world,SLOT(grabItem(QPointF)));
    connect(view,SIGNAL(mouseRightButtonPressed(QPointF)),world,SLOT(createBox(QPointF)));
    connect(view,SIGNAL(mouseLeftButtonReleased()),       world,SLOT(dropItem()));
    connect(view,SIGNAL(mouseMoved(QPointF)),             world,SLOT(moveItem(QPointF)));
    connect(view,SIGNAL(keyPressed(int)),                 world,SLOT(handleKeyPressed(int)));
    connect(view,SIGNAL(keyReleased(int)),                world,SLOT(handleKeyReleased(int)));

    connect(world,SIGNAL(itemCreated(QBox2DItem*)),  scene,SLOT(addItem(QBox2DItem*)));
    connect(world,SIGNAL(itemDestroyed(QBox2DItem*)),scene,SLOT(removeItem(QBox2DItem*)));
}

void MainWindow::createWorld(){
    qDebug()<<"Creating World";
    //world = new TestWorld(this);
    //world = new ExampleWorld(this);
    world = new ArcanoidWorld(this);
    world->setSettings(1.0f / 60.0f, 10, 10);

    connect(timer,SIGNAL(timeout()),world,SLOT(step()));
    qDebug()<<"Connect timer with World";
}

void MainWindow::deleteWorld(){
    glscene->clear();
    world->disconnect();
    delete world;
    world = NULL;
}

void MainWindow::deleteGLScene(){
    glscene->disconnect();
    delete glscene;
    glscene = NULL;
}

void MainWindow::startGame(){
    createWorld();
    createGLScene();
    timer->start();
}

void MainWindow::restartGame(){
    timer->disconnect();
    timer->stop();
    deleteWorld();
    deleteGLScene();
    startGame();
}

MainWindow::~MainWindow(){
    delete timer;
    delete world;
    delete ui;
}
