#include "arcanoidworld.h"

ArcanoidWorld::ArcanoidWorld() : QBox2DWorld() {
}

void ArcanoidWorld::step(){
    QBox2DWorld::step();

    if (_contacts.size() == 0) {
        return;
    }

    QSet<QBox2DItem*> dest_items;
    for(uint i = 0; i < _contacts.size() ; ++i){
        ContactPoint cp = _contacts.at(i);
        QBox2DItem *item = ( cp.itemA == _ball ) ? cp.itemB :
                           ( cp.itemB == _ball ) ? cp.itemA : NULL;
        if (item && item->body()->GetType() != b2_staticBody ) {
            if (item == _paddle) continue;
            dest_items.insert(item);
        }
    }

    QSetIterator<QBox2DItem*> i(dest_items);
    while(i.hasNext()){
        QBox2DItem *item = i.next();
        _world->DestroyBody(item->body());
        _scene->removeItem(item);
        delete item;
    }
}

void ArcanoidWorld::create(QGraphicsScene* const scene) {
        _scene = scene;
        scene->setSceneRect(-200, 0, 400, 400);
        _world->SetGravity(b2Vec2(0, -1));

        QBox2DRectItem* groundl = new QBox2DRectItem();
        groundl->setPos(-205, 0);
        groundl->setShape(QRectF(0, 0, 5, 400));
        groundl->setBrush(QColor(128, 128, 128));
        groundl->setRestitution(1.0f);
        groundl->setBodyType(b2_staticBody);
        groundl->create(_world);
        scene->addItem(groundl);

        QBox2DRectItem* groundr = new QBox2DRectItem();
        groundr->setPos(200, 0);
        groundr->setShape(QRectF(0, 0, 5, 400));
        groundr->setBrush(QColor(128, 128, 128));
        groundr->setRestitution(1.0f);
        groundr->setBodyType(b2_staticBody);
        groundr->create(_world);
        _scene->addItem(groundr);

        QBox2DRectItem* groundup = new QBox2DRectItem();
        groundup->setPos(-200, 0);
        groundup->setShape(QRectF(0, 0, 400, 5));
        groundup->setBrush(QColor(128, 128, 128));
        groundup->setBodyType(b2_staticBody);
        groundup->create(_world);
        _scene->addItem(groundup);

        QBox2DRectItem* hor_item = new QBox2DRectItem();
        hor_item->setPos(0, 400);
        hor_item->setShape(QRectF(0, 0, 5, 5));
        hor_item->setBrush(QColor(128, 128, 128));
        hor_item->setBodyType(b2_dynamicBody);
        hor_item->setDensity(0.0f);
        hor_item->create(_world);
        _scene->addItem(hor_item);

        _paddle = new QBox2DRectItem();
        _paddle->setPos(-50, 400 - 5);
        _paddle->setShape(QRectF(0, 0, 100, 5));
        _paddle->setBrush(QColor(128, 128, 128));
        _paddle->setBodyType(b2_dynamicBody);

        _paddle->setFriction(1.0f);
        _paddle->setDensity(1.0f);
        _paddle->setRestitution(1.0f);
        _paddle->create(_world);
        //_paddle->body()->SetType(b2_staticBody);
        scene->addItem(_paddle);

        b2PrismaticJointDef hor_joint_def;
        b2Vec2 axis(1.0f, 0.0f);

        hor_joint_def.Initialize(hor_item->body(), _groundBody, b2Vec2(0,0), axis);
        hor_joint_def.lowerTranslation = -2.96f;
        hor_joint_def.upperTranslation = 2.96f;
        hor_joint_def.enableLimit = true;
        _world->CreateJoint(&hor_joint_def);

        b2PrismaticJointDef vert_joint_def;
        axis = b2Vec2(0.0f, 1.0f);

        vert_joint_def.Initialize(_paddle->body(), hor_item->body(), b2Vec2(0,0), axis);
        vert_joint_def.lowerTranslation = -0.5f;
        vert_joint_def.upperTranslation = 0.0f;
        vert_joint_def.enableLimit = true;
        _world->CreateJoint(&vert_joint_def);


        {
            int size = 5;
            QBox2DCircleItem *item = new QBox2DCircleItem();
            item->setShape(QRectF(-size , -size, size*2, size*2));
            item->setPos(0, 200);
            item->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
            item->setBodyType(b2_dynamicBody);
            item->setFriction(1.0f);
            item->setDensity(1.0f);
            item->setRestitution(1.0f);
            item->create(_world);
            //item->body()->SetBullet(true);
            _scene->addItem(item);
            _ball = item;
        }

        for (int j = 0; j< 10; ++j){
            for (int i = 0; i < 10; ++i){
                QBox2DRectItem* brick = new QBox2DRectItem();
                brick->setPos(-150 + 30*i, 10 + 30*j);
                brick->setShape(QRectF(0, 0, 10, 20));
                brick->setBrush(QColor(128 + qrand() % 128, 128 + qrand() % 128, 128 + qrand() % 128));
                brick->setRestitution(1.0f);
                brick->setDensity(1.0f);
                brick->setBodyType(b2_dynamicBody);
                brick->create(_world);
                _scene->addItem(brick);
                b2RevoluteJointDef jointDef;
                jointDef.Initialize(brick->body(), groundup->body(), brick->body()->GetWorldCenter());
                jointDef.enableMotor = true;
                jointDef.motorSpeed = b2_pi;
                jointDef.maxMotorTorque = 5000.0f;

                _world->CreateJoint(&jointDef);
            }
        }
    }


void ArcanoidWorld::updateKeys(int key, int state) {
        if( state == 1 ) {
            switch( key ) {
            case Qt::Key_W:
                _paddle->body()->ApplyLinearImpulse(b2Vec2(0.0, 1.0f),_paddle->body()->GetWorldCenter());
            break;
            case Qt::Key_S:
                _paddle->body()->ApplyLinearImpulse(b2Vec2(0.0, -1.0f),_paddle->body()->GetWorldCenter());
            break;
            case Qt::Key_A:
                _paddle->body()->ApplyForceToCenter(b2Vec2(-500.0f,0.0f));
            break;
            case Qt::Key_D:
                _paddle->body()->ApplyForceToCenter(b2Vec2(500.0f,0.0f));
            break;
            case Qt::Key_Q:
                _paddle->body()->ApplyTorque(1000);
            break;
            case Qt::Key_E:
                _paddle->body()->ApplyTorque(-1000);
            break;
            }
        }
        else {

        }
    }
