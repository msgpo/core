/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#pragma once

#include "shape.hxx"
#include "shapeattributelayer.hxx"
#include <unordered_map>

class b2Body;
class b2World;

namespace slideshow::internal
{
class ShapeManager;
typedef std::shared_ptr<ShapeManager> ShapeManagerSharedPtr;
}

namespace box2d::utils
{
class box2DBody;
class box2DWorld;
typedef std::shared_ptr<box2DWorld> Box2DWorldSharedPtr;
typedef std::shared_ptr<box2DBody> Box2DBodySharedPtr;
typedef std::unordered_map<css::uno::Reference<css::drawing::XShape>, Box2DBodySharedPtr>
    XShapeToBox2DBodyPtrMap;
typedef std::shared_ptr<XShapeToBox2DBodyPtrMap> XShapeToBox2DBodyPtrMapSharedPtr;

enum box2DBodyType
{
    BOX2D_STATIC_BODY = 0,
    BOX2D_KINEMATIC_BODY,
    BOX2D_DYNAMIC_BODY
};

class box2DWorld
{
private:
    std::unique_ptr<b2World> mpBox2DWorld;
    double mfScaleFactor;
    bool mbShapesInitialized;
    bool mbHasWorldStepper;
    int mnDynamicShapeCount;
    XShapeToBox2DBodyPtrMap mpXShapeToBodyMap;

    b2Body* createStaticFrameAroundSlide(const ::basegfx::B2DVector& rSlideSize);

public:
    box2DWorld(const ::basegfx::B2DVector& rSlideSize);
    ~box2DWorld();

    bool initiateWorld(const ::basegfx::B2DVector& rSlideSize);

    void step(const float fTimeStep = 1.0f / 100.0f, const int nVelocityIterations = 6,
              const int nPositionIterations = 2);

    bool getShapesInitialized();
    bool getWorldInitialized();

    Box2DBodySharedPtr makeShapeDynamic(const slideshow::internal::ShapeSharedPtr pShape);
    Box2DBodySharedPtr makeBodyDynamic(const Box2DBodySharedPtr pBox2DBody);
    Box2DBodySharedPtr makeShapeStatic(const slideshow::internal::ShapeSharedPtr pShape);
    Box2DBodySharedPtr makeBodyStatic(const Box2DBodySharedPtr pBox2DBody);

    Box2DBodySharedPtr createDynamicBodyFromBoundingBox(
        const slideshow::internal::ShapeSharedPtr& rShape,
        const slideshow::internal::ShapeAttributeLayerSharedPtr& rAttrLayer,
        const float fDensity = 1.0f, const float fFriction = 0.3f);

    Box2DBodySharedPtr
    createStaticBodyFromBoundingBox(const slideshow::internal::ShapeSharedPtr& rShape,
                                    const float fDensity = 1.0f, const float fFriction = 0.3f);

    void
    initateAllShapesAsStaticBodies(const slideshow::internal::ShapeManagerSharedPtr pShapeManager);

    bool hasWorldStepper();
    void setHasWorldStepper(bool bHasWorldStepper);
};

class box2DBody
{
private:
    b2Body* mpBox2DBody;

public:
    box2DBody(b2Body* pBox2DBody);

    ~box2DBody();

    ::basegfx::B2DPoint getPosition(const ::basegfx::B2DVector& rSlideSize);
    double getAngle();

    void setType(box2DBodyType eType);
    box2DBodyType getType();
};
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
