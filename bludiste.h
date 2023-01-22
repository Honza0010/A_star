#ifndef BLUDISTE_H
#define BLUDISTE_H

#include <QMainWindow>
#include <QPushButton>
#include <QVector>
#include <QGridLayout>
#include <QHBoxLayout>
#include <QMenu>
#include <QMenuBar>
#include <QToolBar>
#include <QAction>
#include <QWidget>


class Bludiste : public QMainWindow
{
    Q_OBJECT

    QVector<QPushButton*> buttons;

    QWidget* widget;
    QGridLayout* layout;

    QMenu *menu_solve;
    QMenu *menu_choose;
    QMenu *menu_parameters;
    QToolBar *toolbar;

    QAction *actChoose;
    QAction *actReset;
    QAction *actCalculate;

    QAction *actStart;
    QAction *actObstacle;
    QAction *actFinish;
    QAction *actClean;
    QAction *actCancelOptions;



    int rows = 10;
    int columns = 10;


    bool is_start;      //Bool pokud volím na mapě start
    bool is_obstacle;   //Bool pokud volím na mapě překážku
    bool is_finish;     //Bool pokud volím na mapě konec
    bool is_cleaning;       //Bool pokud nevolím na mapě nic

    int start;      //Index startu ve vectoru buttons
    int finish;     //Index konce ve vectoru buttons


    void initialize();
    void setup();


public:
    Bludiste(QWidget *parent = nullptr);
    ~Bludiste();

private slots:
    void choose();
    void map_reset();
    void calculate();

    void start_slot();
    void finish_slot();
    void obstacle_slot();
    void clean_slot();
    void cancel_slot();

    void make_map(int i, int j);

};
#endif // BLUDISTE_H
