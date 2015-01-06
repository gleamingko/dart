/*
 * Copyright (c) 2014, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author(s): Michael X. Grey <mxgrey@gatech.edu>
 *
 * Georgia Tech Graphics Lab and Humanoid Robotics Lab
 *
 * Directed by Prof. C. Karen Liu and Prof. Mike Stilman
 * <karenliu@cc.gatech.edu> <mstilman@cc.gatech.edu>
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#include "dart/dynamics/Entity.h"
#include "dart/dynamics/Frame.h"
#include "dart/dynamics/Shape.h"

#include "dart/renderer/RenderInterface.h"

namespace dart {
namespace dynamics {

//==============================================================================
Entity::Entity(const Frame* _refFrame, const std::string& _name) :
  mParentFrame(NULL),
  mName(_name)
{
  changeParentFrame(_refFrame);
}

//==============================================================================
Entity::~Entity()
{
  changeParentFrame(NULL);
}

//==============================================================================
const std::string& Entity::setName(const std::string &_name)
{
  mName = _name;
  return mName;
}

//==============================================================================
const std::string& Entity::getName() const
{
  return mName;
}

//==============================================================================
void Entity::draw(renderer::RenderInterface *_ri, const Eigen::Vector4d &_color,
                  bool _useDefaultColor, int) const
{
  if(NULL == _ri)
    return;

  _ri->pushMatrix();
  _ri->transform(mParentFrame->getTransform());

  // _ri->pushName(???); TODO(MXG): How should this be handled for entities?
  for(size_t i=0; i < mVizShapes.size(); ++i)
  {
    _ri->pushMatrix();
    mVizShapes[i]->draw(_ri, _color, _useDefaultColor);
    _ri->popMatrix();
  }
  // _ri->popName();
}

//==============================================================================
Frame* Entity::getParentFrame()
{
  return mParentFrame;
}

//==============================================================================
const Frame* Entity::getParentFrame() const
{
  return mParentFrame;
}

//==============================================================================
void Entity::notifyTransformUpdate()
{
  mNeedTransformUpdate = true;
}

//==============================================================================
void Entity::notifyVelocityUpdate()
{
  mNeedVelocityUpdate = true;
}

//==============================================================================
void Entity::notifyAccelerationUpdate()
{
  mNeedAccelerationUpdate = true;
}

//==============================================================================
void Entity::changeParentFrame(const Frame* _newParentFrame)
{
  if(mParentFrame)
  {
    if(mParentFrame->mChildEntities.find(this) !=
       mParentFrame->mChildEntities.end())
      mParentFrame->mChildEntities.erase(this);
  }

  if(NULL==_newParentFrame)
  {
    mParentFrame = NULL;
    return;
  }

  mParentFrame = const_cast<Frame*>(_newParentFrame);
  mParentFrame->mChildEntities.insert(this);
  notifyTransformUpdate();
}

//==============================================================================
Detachable::Detachable(const Frame *_refFrame, const std::string &_name) :
  Entity(_refFrame, _name)
{

}

//==============================================================================
void Detachable::setParentFrame(const Frame* _newParentFrame)
{
  changeParentFrame(_newParentFrame);
}


} // namespace dynamics
} // namespace dart