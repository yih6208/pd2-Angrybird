#include "bird.h"

Bird::Bird(float x, float y, float radius, QTimer *timer, QPixmap pixmap, b2World *world, QGraphicsScene *scene, int type):GameItem(world)
{
    // Set pixmap
    g_pixmap.setPixmap(pixmap);
    g_pixmap.setTransformOriginPoint(g_pixmap.boundingRect().width()/2,g_pixmap.boundingRect().height()/2);
    g_size = QSize(radius*2,radius*2);

    // Create Body
    b2BodyDef bodydef;
    bodydef.type = b2_dynamicBody;
    bodydef.bullet = true;
    bodydef.position.Set(x,y);
    bodydef.userData = this;
    g_body = world->CreateBody(&bodydef);
    b2CircleShape bodyshape;
    bodyshape.m_radius = radius;
    b2FixtureDef fixturedef;
    fixturedef.shape = &bodyshape;
    fixturedef.density = BIRD_DENSITY;
    fixturedef.friction = BIRD_FRICTION;
    fixturedef.restitution = BIRD_RESTITUTION;
    g_body->SetAngularDamping(3);
    g_body->CreateFixture(&fixturedef);

    // Bound timer
    connect(timer, SIGNAL(timeout()), this,SLOT(paint()));
    this->type=type;
    scene->addItem(&g_pixmap);
}

void Bird::setLinearVelocity(b2Vec2 velocity)
{
    g_body->SetLinearVelocity(velocity);
}

void Bird::SetPosion(QMouseEvent *pos){
    QPoint  MousePosXY=pos->pos();

    if(abs(MousePosXY.x()-150)>30||abs(MousePosXY.y()-350)>30){

        X=30*(MousePosXY.x()-150)/sqrt((pow(abs(MousePosXY.x()-150),2)+pow(abs(MousePosXY.y()-350),2)))+150;
        Y=30*(MousePosXY.y()-350)/sqrt((pow(abs(MousePosXY.x()-150),2)+pow(abs(MousePosXY.y()-350),2)))+350;
        //X=100*(MousePosXY.x()-150)/sqrt((pow(abs(MousePosXY.x()-150),2)+pow(abs(MousePosXY.y()-350),2)));
        //Y=sqrt((pow(abs(MousePosXY.x()-150),2)+pow(abs(MousePosXY.y()-350),2)));
        std::cout << X << ',' << Y << std::endl ;

    }
    else{
         X=MousePosXY.x();
         Y=MousePosXY.y();
         std::cout << X << ',' << Y << std::endl ;

    }
    g_body->SetTransform(b2Vec2(float32(X)/30.5-0.9,float32 (540-Y)/30.5+1),g_body->GetAngle());
    //g_body->SetLinearVelocity(b2Vec2(0.5f,0.000001f));


}
void Bird::SetPosion(QPoint *pos){
    std::cout<<"set poision" << pos->x() << ',' << pos->y() << std::endl ;

    g_body->SetTransform(b2Vec2(float32(pos->x())/30.5-0.9,float32 (pos->y())/30.5+1),g_body->GetAngle());
    g_body->SetLinearVelocity(b2Vec2(0.0,0.0f));


}
void Bird::shoot(){
    setLinearVelocity(b2Vec2((150-X)/2,(Y-350)/2));
}

