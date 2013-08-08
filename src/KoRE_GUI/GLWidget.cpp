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

#include "KoRE_GUI/GLWidget.h"

#include <QGuiApplication>
#include <QKeyEvent>
#include <QTimer>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "KoRE/ShaderProgram.h"
#include "KoRE/SceneManager.h"
#include "KoRE/ResourceManager.h"
#include "KoRE/RenderManager.h"

GLWidget::GLWidget(QWidget *parent) : QGLWidget(parent) {

    QGLFormat cformat;
    cformat.setDepthBufferSize(24);
    cformat.setVersion(4,3);
    cformat.setProfile(QGLFormat::CoreProfile);

    this->setFormat(cformat);
    this->setMinimumSize(640,480);
    resize(640,480);

    QTimer* timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    timer->start(18);
    setFocusPolicy(Qt::FocusPolicy::NoFocus);
}

GLWidget::~GLWidget() {
}

void GLWidget::initializeGL() {
    // initialize glew
    glewExperimental = GL_TRUE;
    if (glewInit()) {
      kore::Log::getInstance()->write("[ERROR] glew could not be initialized");
      QGuiApplication::quit();
    }

    // get GL info
    kore::Log::getInstance()->write(
        "Render Device: %s\n",
        reinterpret_cast<const char*>(glGetString(GL_RENDERER)));
    kore::Log::getInstance()->write(
        "Vendor: %s\n",
        reinterpret_cast<const char*>(glGetString(GL_VENDOR)));
    kore::Log::getInstance()->write(
        "OpenGL version: %s\n",
        reinterpret_cast<const char*>(glGetString(GL_VERSION)));
    kore::Log::getInstance()->write(
        "GLSL version: %s\n",
        reinterpret_cast<const char*>
        (glGetString(GL_SHADING_LANGUAGE_VERSION)));

    // enable culling and depthtest
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_TEXTURE_2D);
    glCullFace(GL_BACK);
}

void GLWidget::resizeGL(int x, int y) {
    //kore::RenderManager::getInstance()->setRenderResolution(glm::ivec2(width(), height()));
    //TODO (dospelt) remove placeholder
    std::vector<kore::SceneNode*> finder;
    kore::SceneManager::getInstance()->getSceneNodesByName("Camera", finder);
    kore::SceneNode* camnode = (finder.size() > 0) ? finder[0] : NULL;
    if (camnode) {
      kore::Camera* cam = static_cast<kore::Camera*>(camnode->getComponent(kore::COMPONENT_CAMERA));
      if (cam) {
        cam->setAspectRatio((width()*1.0f)/height());
      }
    }
    glViewport(0, 0, width(), height());
    kore::RenderManager::getInstance()->setScreenResolution(glm::ivec2(x,y));
    kore::Log::getInstance()->write("[GUI] Render resolution changed to %i x %i\n", width(), height());
}

void GLWidget::paintGL() {
  std::vector<kore::SceneNode*> node;
  kore::SceneManager::getInstance()->getSceneNodesByName("Rob", node);
  for (uint i = 0; i < node.size(); i++) {
    node[i]->rotate(1.0, glm::vec3(0.0, 0.0, 1.0), kore::SPACE_WORLD);
  }
  kore::SceneManager::getInstance()->update();
  // TODO all GL handling is provided by KoRE itself
  glClearColor(0.089, 0.089, 0.089, 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  kore::RenderManager::getInstance()->renderFrame();
}

void GLWidget::keyPressEvent(QKeyEvent * evnt) {
    if (evnt->key() == Qt::Key_Escape) QGuiApplication::quit();
    QGLWidget::keyPressEvent(evnt);
}
