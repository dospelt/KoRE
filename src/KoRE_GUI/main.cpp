/*
  Copyright (c) 2012 The KoRE Project

  This file is part of KoRE.

  KoRE is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

  KoRE is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with KoRE.  If not, see <http://www.gnu.org/licenses/>.
*/

/************************************************************************/
/* \author Dominik Ospelt                                               */
/************************************************************************/

#include <GL/glew.h>
#include <QApplication>
#include <QMainWindow>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QCommonStyle>

#include "KoRE/SceneManager.h"
#include "KoRE/ResourceManager.h"
#include "KoRE/RenderManager.h"

#include "KoRE/Components/TexturesComponent.h"

#include "KoRE_GUI/SceneViewer.h"
#include "KoRE_GUI/RenderViewer.h"
#include "KoRE_GUI/ResourceViewer.h"
#include "KoRE_GUI/OperationFlow.h"
#include "KoRE_GUI/GLWidget.h"
#include "KoRE_GUI/MainWidget.h"

int main(int argc, char *argv[]) {
  // initialize Qt
  QApplication app(argc, argv);

  //MainWidget* editor = new MainWidget();
  //editor->show();

  // need of GL-Context
  GLWidget* win = new GLWidget;
  win->show();

  // now  other widgets
  koregui::RenderViewer* rview = new koregui::RenderViewer;
  koregui::ResourceViewer* resview = new koregui::ResourceViewer;
  koregui::OperationFlow* oview = new koregui::OperationFlow;
  koregui::SceneViewer* sview = new koregui::SceneViewer(rview);

  QWidget* editor = new QWidget;
  QTabWidget* tab = new QTabWidget;
  tab->addTab(rview, "RenderView");
  tab->addTab(sview, "SceneViewer");


  QHBoxLayout* hbox = new QHBoxLayout;
  hbox->setMargin(0);
  hbox->setSpacing(0);
  hbox->addWidget(tab);
  hbox->addWidget(win);
 
  QVBoxLayout* vbox = new QVBoxLayout;
  vbox->setMargin(0);
  vbox->setSpacing(0);
  vbox->addLayout(hbox);
  vbox->addWidget(oview);

  editor->setLayout(vbox);
  editor->show();

  //win->show();
  //sview->show();
  //rview->show();
  //oview->show();

  // demo startup loading
  kore::ResourceManager::getInstance()->loadScene("./assets/meshes/Robot.dae");
  kore::ResourceManager::getInstance()->loadScene("./assets/meshes/FullScreenTriangle.dae");
  std::vector<kore::SceneNode*> finder;
  kore::SceneManager::getInstance()->getSceneNodesByName("Rob", finder);
  kore::SceneNode* cube = (finder.size() > 0) ? finder[0] : NULL;
  finder.clear();
  kore::SceneManager::getInstance()->getSceneNodesByName("Flat", finder);
  kore::SceneNode* flat = (finder.size() > 0) ? finder[0] : NULL;
  kore::Texture* tex = kore::ResourceManager::getInstance()->loadTexture("./assets/textures/checkerboard.png");
  kore::Texture* tex1 = kore::ResourceManager::getInstance()->loadTexture("./assets/textures/RobColor.png");
  kore::Texture* tex2 = kore::ResourceManager::getInstance()->loadTexture("./assets/textures/RobNormal.png");
  kore::TexturesComponent* pTexComponent = new kore::TexturesComponent;
  if (tex1) pTexComponent->addTexture(tex1);
  if (tex2) pTexComponent->addTexture(tex2);
  if (cube) cube->addComponent(pTexComponent);
  kore::TexturesComponent* pTexComponent1 = new kore::TexturesComponent;
  if (tex1) pTexComponent1->addTexture(tex1);
  if (tex2) pTexComponent1->addTexture(tex2);
  if (flat) flat->addComponent(pTexComponent1);
  kore::SceneManager::getInstance()->update();

  // set scene to show in sceneviewer
  sview->showScene(kore::SceneManager::getInstance()->getRootNode());

  return app.exec();
}
