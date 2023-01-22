#include "bludiste.h"
//#include <QWidget>
#include <QColor>
#include "dialog.h"
#include <QMessageBox>
#include <QPalette>
#include "astar.h"
#include "Exception.h"
#include <vector>
#include <QRgb>
#include <QIcon>



Bludiste::Bludiste(QWidget *parent)
    : QMainWindow(parent), rows(10), columns(10), is_start(false), is_obstacle(false), is_finish(false), is_cleaning(false), start(-1), finish(-1)
{
    setup();
    initialize();
    setWindowIcon(QIcon(":/icons/icons/bludiste.png"));
}

Bludiste::~Bludiste()
{
}



void Bludiste::initialize()     //Inicializuje mapu
{
    for(int i = 0; i < buttons.size(); i++)
    {
        buttons[i]->deleteLater();
    }
    buttons.clear();

    layout->setSpacing(0);      //Nulové mezery mezi řádky a sloupci gridlayoutu
    layout->setContentsMargins(0,0,0,0);


    for(int i = 0; i < rows; i++)
    {
        layout->setRowStretch(i, 30);
        for(int j = 0; j < columns; j++)
        {
            layout->setColumnStretch(j, 30);
            QPushButton* button = new QPushButton();
            button->setStyleSheet("Background-color: white; border-color: solid black");

            button->setFixedSize(30,30);
            layout->addWidget(button, i, j);

            connect(button, &QPushButton::clicked, this, [=](){ this->make_map(i, j); });       //Do slotu pošle souřadnice tlačítka umístěného v Gridlayoutu
            buttons.push_back(button);
        }
    }




    widget->resize((rows)*30, columns*30);
    this->resize((rows)*30+48, columns*30);
    this->setFixedHeight((rows)*30+48);
    this->setFixedWidth(columns*30);
}

void Bludiste::setup()
{
    widget = new QWidget;

    this->setCentralWidget(widget);
    layout = new QGridLayout;
    widget->setLayout(layout);

    menu_solve = new QMenu("Řešení");
    menu_choose = new QMenu("Bludiště");
    menu_parameters = new QMenu("Parametry");

    this->menuBar()->addMenu(menu_choose);
    this->menuBar()->addMenu(menu_solve);
    this->menuBar()->addMenu(menu_parameters);

    actCalculate = new QAction("Spočti");
    actReset = new QAction("Vyčisti");
    actChoose = new QAction("Vyber rozměry");

    actStart = new QAction("Start");
    actObstacle = new QAction("Překážka");
    actFinish = new QAction("Konec");
    actClean = new QAction("Vyčisti tlačítko");
    actCancelOptions = new QAction("Zruš výběr");

    //actualHeight = new QAction("Výška okna");

    toolbar = new QToolBar();
    this->addToolBar(toolbar);
    toolbar->addAction(actChoose);
    toolbar->addAction(actCalculate);
    toolbar->addSeparator();
    toolbar->addAction(actReset);


    menu_solve->addAction(actCalculate);
    menu_choose->addAction(actChoose);
    menu_choose->addAction(actReset);
    //menu_choose->addAction(actualHeight);

    menu_parameters->addAction(actStart);
    menu_parameters->addAction(actObstacle);
    menu_parameters->addAction(actFinish);
    menu_parameters->addAction(actClean);
    menu_parameters->addSeparator();
    menu_parameters->addAction(actCancelOptions);

    //actCalculate->setEnabled(false);

    connect(actCalculate, &QAction::triggered, this, &Bludiste::calculate);
    connect(actChoose, &QAction::triggered, this, &Bludiste::choose);
    connect(actReset, &QAction::triggered, this, &Bludiste::map_reset);
    //connect(actualHeight, &QAction::triggered, this, &Bludiste::showActualHeight);

    connect(actStart, &QAction::triggered, this, &Bludiste::start_slot);
    connect(actObstacle, &QAction::triggered, this, &Bludiste::obstacle_slot);
    connect(actFinish, &QAction::triggered, this, &Bludiste::finish_slot);
    connect(actClean, &QAction::triggered, this, &Bludiste::clean_slot);
    connect(actCancelOptions, &QAction::triggered, this, &Bludiste::cancel_slot);
}

void Bludiste::choose()     //Zvolím velikost mapy
{
    Dialog d;
    if(d.exec() == QDialog::Accepted)
    {
        rows = d.getRows();
        columns = d.getColumns();
        if(rows < 4 || columns < 4)
        {
            QMessageBox::information(this, "Velikost pole", "Zadal jsi moc malé pole pro výpočet");
            return;
        }
        actCalculate->setEnabled(true);
        initialize();

    }
}

void Bludiste::map_reset()      //Vyčistí mapu
{
    for(int i = 0; i < buttons.size(); i++)
    {
        buttons[i]->setStyleSheet("Background-color: white; border-color: solid black");
    }
}




void Bludiste::calculate()
{
    if(start == -1 || finish == -1)     //Pokud je start -1 nebo finish -1, tak jsme nezadali počáteční a koncový bod
    {
       QMessageBox::information(this, "Řešení", "Není co řešit, nebyl zadán start ani cíl");
       return;
    }


    try
    {

        Astar a(rows, columns);

        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < columns; j++)
            {
                QPalette pal =  buttons[i*columns + j]->palette();
                QColor col = pal.base().color();

                if(col == Qt::blue)     //Modré tlačítko je start
                {
                    a.setStart(i,j);
                }

                if(col == Qt::red)      //Červené tlačítko je konec
                {
                    a.setEnd(i,j);
                }

                if(col == Qt::black)       //Černé tlačítko je překážka
                {
                    a.setGrid(i, j, 1);
                }
            }
        }

        a.search();

        //Vykreslí žlutě všechny buňky, které algoritmus prošel
        std::vector<std::vector<bool>> explored = a.getExplored();

        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < columns; j++)
            {
                if(explored[i][j] == true)
                {
                    QPalette pal =  buttons[i*columns + j]->palette();
                    QColor col = pal.base().color();
                    //QColor col2(8,223,0);
                    if(col == Qt::green || col == Qt::black || col == Qt::blue || col == Qt::red)
                        continue;
                    buttons[i*columns + j]->setStyleSheet("Background-color: yellow; border-color: solid black");
                }
            }
        }
        //

        if(a.getHasSolution() == false)     //Pokud nemá bludiště řešení, tak ukončí funkci
        {
            QMessageBox::information(this, "Řešení", "Neexistuje řešení");
            return;
        }

        std::vector<Cell> path = a.getPath();       //Vektor cesty

        for(int i = 1; i < path.size()-1; i++)      //Vykreslí cestu zeleně (překreslí i některé žluté buňky, protože do cesty patří i ty prozkoumané)
        {
            buttons[path[i].x*columns + path[i].y]->setStyleSheet("Background-color: #00ff00; border-color: solid black");
        }
        start = -1;
        finish = -1;

        /*std::vector<std::vector<bool>> explored = a.getExplored();

        for(int i = 0; i < rows; i++)
        {
            for(int j = 0; j < columns; j++)
            {
                if(explored[i][j] == true)
                {
                    QPalette pal =  buttons[i*columns + j]->palette();
                    QColor col = pal.base().color();
                    //QColor col2(8,223,0);
                    if(col == Qt::green || col == Qt::black || col == Qt::blue || col == Qt::red)
                        continue;
                    buttons[i*columns + j]->setStyleSheet("Background-color: yellow; border-color: solid black");
                }
            }
        }*/
    }
    catch(MyException ex)
    {
        QMessageBox::information(this, "Chyba", "Nastala chyba při výpočtu");
    }

}





void Bludiste::start_slot()
{
    this->is_start = true;
    this->is_finish = false;
    this->is_obstacle = false;
    this->is_cleaning = false;
}

void Bludiste::finish_slot()
{
    this->is_finish = true;
    this->is_obstacle = false;
    this->is_start = false;
    this->is_cleaning = false;
}

void Bludiste::obstacle_slot()
{
    this->is_start = false;
    this->is_finish = false;
    this->is_obstacle = true;
    this->is_cleaning = false;
}

void Bludiste::clean_slot()
{
    this->is_cleaning = true;
    this->is_finish = false;
    this->is_obstacle = false;
    this->is_start = false;
}

void Bludiste::cancel_slot()
{
    this->is_start = false;
    this->is_finish = false;
    this->is_obstacle = false;
    this->is_cleaning = false;
}


//Slot, který po kliknutí na tlačítko v poli vyplňuje mapu překážkami, cílem, či startem
void Bludiste::make_map(int i, int j)
{
    if(is_start == true)
    {
        if(start != -1)
        {
            buttons[start]->setStyleSheet("Background-color: white; border-color: solid black");
        }

        buttons[i*columns + j]->setStyleSheet("Background-color: blue; border-color: solid black");
        //QPalette pal =  buttons[i*columns + j]->palette();
        //QColor col = pal.base().color();
        start = i*columns + j;

        if(i*columns + j == finish)
        {
            finish = -1;
        }
    }
    if(is_obstacle == true)
    {
        buttons[i*columns + j]->setStyleSheet("Background-color: black; border-color: solid black");

        if(i*columns + j == finish)
        {
            finish = -1;
        }
        if(i*columns + j == start)
        {
            start = -1;
        }

    }
    if(is_finish == true)
    {
        if(finish != -1)
        {
            buttons[finish]->setStyleSheet("Background-color: white; border-color: solid black");
        }
        buttons[i*columns + j]->setStyleSheet("Background-color: red; border-color: solid black");

        finish = i*columns + j;

        if(i*columns + j == start)
        {
            start = -1;
        }

    }

    if(is_cleaning == true)
    {
        buttons[i*columns + j]->setStyleSheet("Background-color: white; border-color: solid black");
        if(i*columns + j == finish)
        {
            finish = -1;
        }
        if(i*columns + j == start)
        {
            start = -1;
        }
    }
}
