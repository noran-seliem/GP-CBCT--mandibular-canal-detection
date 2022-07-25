#include "scene.h"
#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

class SceneManager
{

public:
    Scenez *sceneAxial;
    Scenez *sceneCoronal;
    Scenez *sceneSagittal;
    Scenez *sceneCurve;
    Scenez *scenePanorama;
    Scenez *sceneSerial;
	void SceneReturener(QGraphicsScene * Scene, int HorizonatSilderValue);

};
#endif // !
