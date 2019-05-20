#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButtonRes_clicked()
{
    int barrier, posun;
    int N = ui->lineEditN->text().toInt();



    omp_set_num_threads(N);

    int *M = new int[N];
    int *Y = new int[N];
    #pragma omp parallel
        {
        #pragma omp for ordered schedule(dynamic)
            for(int i=0; i<N;i++){
                #pragma omp ordered
                M[i]=i;
            }
        }
    //TESTOVACÍ VSTUPY
//    M[0]= 3;
//    M[1]= 2;
//    M[2]= 4;
//    M[3]= 7;
//    M[4]= 1;
//    M[5]= 5;
//    M[6]= 2;
//    for(int i=0; i<=N;i++){
//       qDebug()<<"element: "+ QString::number(InputX[i]);
//    }

    #pragma omp parallel shared(M,Y)
    {
        #pragma omp for
        for (int i = 0; i < N; i++)
        {
            Y[i] = M[i];
        }
     }

    barrier = static_cast<int>(ceil(log(N) / log(2)));
    for (int j = 0; j < barrier;j++)
    {
       posun = static_cast<int>(pow(2,j));
       #pragma omp parallel shared(M, Y)
        {
        #pragma omp for ordered schedule(dynamic)
            for (int i = posun; i < N; i++)
            {
                #pragma omp ordered
                Y[i] =Y[i]+ M[i - posun];
            }
        #pragma omp for ordered schedule(dynamic)
            for (int i = posun; i < N; i++)
            {
                #pragma omp ordered
                M[i] = Y[i];
            }
        }
    }

    QString output="";
    for(int i=0; i<=N-1;i++){
       output =output + "|"+ QString::number(M[i]);
    }

    ui->lineEditRes->setText(output);





}
