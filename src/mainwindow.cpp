#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // Enable the event Filter
    qApp->installEventFilter(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *)
{
    // Setting the QGraphicsScene


    //scene = new QGraphicsScene(0,0,width(),ui->graphicsView->height());
    scene = new QGraphicsScene(0,0,1920,1080);
    ui->graphicsView->setScene(scene);

    // Create world
    world = new b2World(b2Vec2(0.0f, -9.8f));

    // Setting Size
    GameItem::setGlobalSize(QSizeF(32,18),size());

    // Create ground (You can edit here)
    itemList.push_back(new Land(16,1.5,32,3,QPixmap(":/ground.png").scaled(width(),height()/6.0),world,scene));
        shoot.setPixmap(QPixmap(":/shot.png"));
    scene->addItem(&shoot);



    shoot.setPos(QPointF(500,500));
    // Create bird (You can edit here)
    birdie[0] = new Bird(1,4,0.27f,&timer,QPixmap(":/red.png").scaled(height()/9.0,height()/9.0)   ,world,scene,red);
    birdie[1] = new Bird(2,4,0.27f,&timer,QPixmap(":/blue.png").scaled(height()/9.0,height()/9.0)  ,world,scene,blue);
    birdie[2] = new Bird(3,4,0.27f,&timer,QPixmap(":/yellow.png").scaled(height()/9.0,height()/9.0),world,scene,yellow);
    birdie[3] = new Bird(4,4,0.27f,&timer,QPixmap(":/black.png").scaled(height()/9.0,height()/9.0) ,world,scene,black);
    birdie[7] = new Bird(25,4,0.27f,&timer,QPixmap(":/pig.png").scaled(height()/9.0,height()/9.0) ,world,scene,pig);
    avaliable[7]=true;
    shootpos->setX(150);
    shootpos->setY(200);

    birdie[0]->SetPosion(shootpos);
    birdie[0]->g_body->SetAwake(false);

    // Setting the Velocity
    for (int i=0;i<4;i++){
        avaliable[i]=true;
        shooted  [i]=false;
        birdie[i]->setLinearVelocity(b2Vec2(0,0));
        itemList.push_back(birdie[i]);
    }
    for(int i=4;i<7;i++)avaliable[i]=false;


    // Timer
    connect(&timer,SIGNAL(timeout()),this,SLOT(tick()));
    connect(&sleep,SIGNAL(timeout()),this,SLOT(Ready()));
    connect(this,SIGNAL(quitGame()),this,SLOT(QUITSLOT()));
    connect(ui->QuiButton,SIGNAL(released()),this,SLOT(QUITSLOT()));
    connect(ui->RetryButton,SIGNAL(released()),this,SLOT(Retry()));
    timer.start(100/6);

}

bool MainWindow::eventFilter(QObject *, QEvent *event)
{
    // Hint: Notice the Number of every event!
    if(event->type() == QEvent::MouseButtonPress && MousePress==false )
    {
        MousePress=true;
        /* TODO : add your code here */
        QMouseEvent  *MousePos = static_cast<QMouseEvent*>(event);
        QPoint        MousePosXY=MousePos->pos();
        //std::cout << MousePosXY.x()<< ','<<MousePosXY.y() << std::endl ;
        //birdie[9]->SetPosion(MousePos);
        //std::cout << "bird  "<<(25+((pos.x-birdie[i]->g_size.width()/2) * birdie[i]->g_windowsize.width())/birdie[i]->g_worldsize.width()) << ','
        //      <<(25+(1.0f - (pos.y+birdie[i]->g_size.height()/2)/birdie[i]->g_worldsize.height()) * birdie[i]->g_windowsize.height())<< std::endl ;
        //qDebug()  << MousePos->pos();
        //std::cout << "Press !" << std::endl ;

        std::cout << "mouse "<< MousePosXY.x() << ',' <<MousePosXY.y()<< std::endl ;

        b2Vec2 pos = birdie[current]->g_body->GetPosition();
        //move at start
        if(abs(MousePosXY.x()-(25+((pos.x-birdie[current]->g_size.width()/2) * birdie[current]->g_windowsize.width())/birdie[current]->g_worldsize.width()))<27
        &&(abs(MousePosXY.y()-(25+(1.0f - (pos.y+birdie[current]->g_size.height()/2)/birdie[current]->g_worldsize.height()) * birdie[current]->g_windowsize.height()))<27)){
            BirdAim=true;
            return 1;
        }

        if(avaliable[Activate]==true){
            if( birdie[Activate]->type==yellow){
                b2Vec2 Vel = birdie[Activate]->g_body->GetLinearVelocity();
                birdie[Activate]->g_body->SetLinearVelocity(b2Vec2(Vel.x+10,Vel.y-6));


            }

            if(birdie[Activate]->type==black){
                b2Vec2 ActivatePos = birdie[Activate]->g_body->GetPosition();
                int ActivateX = ((ActivatePos.x-birdie[Activate]->g_size.width()/2) * birdie[Activate]->g_windowsize.width())/birdie[Activate]->g_worldsize.width();
                int ActivateY = (1.0f - (ActivatePos.y+birdie[Activate]->g_size.height()/2)/birdie[Activate]->g_worldsize.height()) * birdie[Activate]->g_windowsize.height();

                for(int i=0;i<7;i++){
                    if(avaliable[i]==true){
                        b2Vec2 TargetPos   = birdie[i]->g_body->GetPosition();
                        int TargetX,TargetY;
                        int Vx,Vy;

                        TargetX   = ((TargetPos.x-birdie[i]->g_size.width()/2) * birdie[i]->g_windowsize.width())/birdie[i]->g_worldsize.width();
                        TargetY   = (1.0f - (TargetPos.y+birdie[i]->g_size.height()/2)/birdie[i]->g_worldsize.height()) * birdie[i]->g_windowsize.height();

                        if(i!=Activate&&abs(ActivateX-TargetX)<200&&abs(ActivateY-TargetY)<200){
                            if((TargetX-ActivateX)<0) Vx=-(200+(TargetX-ActivateX));
                            else Vx=(200-(TargetX-ActivateX));
                            if((TargetY-ActivateY)<0) Vy=-(200+(TargetY-ActivateY));
                            else Vy=(200-(TargetY-ActivateY));
                                birdie[i]->g_body->SetLinearVelocity(b2Vec2(float(Vx)/15,float(Vy)/25));
                        }

                    }
                }
                int PigX,PigY;
                b2Vec2 PigPos = birdie[7]->g_body->GetPosition();
                PigX = ((PigPos.x-birdie[7]->g_size.width()/2) * birdie[7]->g_windowsize.width())/birdie[7]->g_worldsize.width();
                PigY = (1.0f - (PigPos.y+birdie[7]->g_size.height()/2)/birdie[7]->g_worldsize.height()) * birdie[7]->g_windowsize.height();
                if(abs(ActivateX-PigX)<500&&abs(ActivateX-PigY)<500){
                    avaliable[7]=false;
                    delete birdie[7];
                }
                avaliable[Activate]=false;
                delete birdie[Activate];
            }

            if(birdie[Activate]->type==blue){
                b2Vec2 ActivateVel = birdie[Activate]->g_body->GetLinearVelocity();
                b2Vec2 ActivatePos = birdie[Activate]->g_body->GetPosition();
                for(int i=4;i<7;i++){
                    birdie[i] = new Bird(ActivatePos.x,ActivatePos.y,0.27f,&timer,QPixmap(":/blue.png").scaled(height()/9.0,height()/9.0),world,scene,5);
                    birdie[i]->setLinearVelocity(b2Vec2(2*ActivateVel.x,ActivateVel.y+6*(i-5)));
                    itemList.push_back(birdie[i]);

                }
                delete birdie[Activate];
                avaliable[Activate]=false;
            }
        }
    }

    if(event->type() == QEvent::MouseMove)
    {
        if(BirdAim==true){
            QMouseEvent  *MousePos = static_cast<QMouseEvent*>(event);
            QPoint  MousePosXY=MousePos->pos();
            birdie[current]->SetPosion(MousePos);

            //birdie[current]->setLinearVelocity(b2Vec2);
            //std::cout << "Move !" << std::endl ;

        }
    }
    if(event->type() == QEvent::MouseButtonRelease&&MousePress==true)
    {
        if(BirdAim==true){

            Activate=current;
            birdie[Activate]->shoot();
            shooted[Activate]=true;
            birdie[Activate]->g_body->SetAwake(true);
            MousePress=false;


            if(current<3){
                current++;
                sleep.start(500);

            }

            std::cout <<  birdie[1]->g_body->GetPosition().x << std::endl ;
            std::cout <<  birdie[2]->g_body->GetPosition().x << std::endl ;
            std::cout <<  birdie[3]->g_body->GetPosition().x << std::endl ;
            std::cout <<  birdie[0]->g_body->GetPosition().x << std::endl ;
            BirdAim=false;


        }
        /* TODO : add your code here */
        MousePress=false;

    }
    return false;
}
void MainWindow::Ready(){
    shootpos->setX(150);
    shootpos->setY(200);
    sleep.stop();
    birdie[current]->SetPosion(shootpos);
    birdie[current]->g_body->SetAwake(false);
    std::cout << "ready" << std::endl ;


}

void MainWindow::closeEvent(QCloseEvent *)
{
    // Close event
    emit quitGame();
}

void MainWindow::tick()
{
    world->Step(1.0/60.0,6,2);
    scene->update();
}

void MainWindow::QUITSLOT()
{
    // For debug
    std::cout << "Quit Game Signal receive !" << std::endl ;
    this->close();
}

void MainWindow::Retry()
{
    // For debug
    std::cout << "Quit Game Signal receive !" << std::endl ;

    delete birdie[0];
    if(avaliable[1]==true) delete birdie[1];
    delete birdie[2];
    if(avaliable[3]==true) delete birdie[3];
    if(avaliable[7]==true) delete birdie[7];


    birdie[0] = new Bird(1,4,0.27f,&timer,QPixmap(":/red.png").scaled(height()/9.0,height()/9.0)   ,world,scene,red);
    birdie[1] = new Bird(2,4,0.27f,&timer,QPixmap(":/blue.png").scaled(height()/9.0,height()/9.0)  ,world,scene,blue);
    birdie[2] = new Bird(3,4,0.27f,&timer,QPixmap(":/yellow.png").scaled(height()/9.0,height()/9.0),world,scene,yellow);
    birdie[3] = new Bird(4,4,0.27f,&timer,QPixmap(":/black.png").scaled(height()/9.0,height()/9.0) ,world,scene,black);
    birdie[7] = new Bird(25,4,0.27f,&timer,QPixmap(":/pig.png").scaled(height()/9.0,height()/9.0) ,world,scene,pig);

    shootpos->setX(150);
    shootpos->setY(200);

    birdie[0]->SetPosion(shootpos);
    birdie[0]->g_body->SetAwake(false);
    current=0;
    Activate=0;
    for (int i=0;i<4;i++){
        avaliable[i]=true;
    }

    //this=new MainWindow();
}
