import {vec2 as fvec2, vec3 as fvec3, mat3 as fmat3} from './glround';
import {vec2, vec3, mat3} from 'gl-matrix';
import React from 'react';
import numeral from 'numeral';
import drag from './drag';
import config from './floatingpointconfig';

let nextId = 0;

class Node {
  constructor(name) {
    this._parent = null;
    this._name = name;
    this._id = nextId++;
    this._children = [];
  }

  traverse(fn) {
    fn(this);
    this._children.forEach((c) => { return c.traverse(fn); });
  }
  
  transformationFrom(other) {
    let thisDepth = this.depth();
    let otherDepth = other.depth();

    let invTransformation = mat3.create();
    let transformation = mat3.create();

    let currentInvNode = this;
    let currentNode = other;

    while (thisDepth > otherDepth) {
      mat3.multiply(invTransformation, invTransformation, currentInvNode.inverseTransformation());
      thisDepth--;
      currentInvNode = currentInvNode.parent();
    }

    while (otherDepth > thisDepth) {
      mat3.multiply(transformation, currentNode.transformation(), transformation);
      otherDepth--;
      currentNode = currentNode.parent();
    }

    while (currentNode !== currentInvNode) {
      if (thisDepth < 0) { throw "no common parent."; }
      mat3.multiply(invTransformation, invTransformation, currentInvNode.inverseTransformation());
      mat3.multiply(transformation, currentNode.transformation(), transformation);
      thisDepth--;
      otherDepth--;
      currentNode = currentNode.parent();
      currentInvNode = currentInvNode.parent();
    }

    let finalMatrix = mat3.create(); 
    mat3.multiply(finalMatrix, invTransformation, transformation);

    return finalMatrix;
  }

  fTransformationFrom(other) {
    let thisDepth = this.depth();
    let otherDepth = other.depth();

    let invTransformation = fmat3.create();
    let transformation = fmat3.create();

    let currentInvNode = this;
    let currentNode = other;

    while (thisDepth > otherDepth) {
      fmat3.multiply(invTransformation, invTransformation, currentInvNode.fInverseTransformation());
      thisDepth--;
      currentInvNode = currentInvNode.parent();
    }

    while (otherDepth > thisDepth) {
      fmat3.multiply(transformation, currentNode.fTransformation(), transformation);
      otherDepth--;
      currentNode = currentNode.parent();
    }

    while (currentNode !== currentInvNode) {
      if (thisDepth < 0) { throw "no common parent."; }
      fmat3.multiply(invTransformation, invTransformation, currentInvNode.fInverseTransformation());
      fmat3.multiply(transformation, currentNode.fTransformation(), transformation);
      thisDepth--;
      otherDepth--;
      currentNode = currentNode.parent();
      currentInvNode = currentInvNode.parent();
    }

    let finalMatrix = fmat3.create(); 
    fmat3.multiply(finalMatrix, invTransformation, transformation);

    return finalMatrix;
  }

  parent() {
    return this._parent;
  }

  depth() {
    if (this._parent) {
      return this._parent.depth() + 1;
    } else {
      return 0;
    }
  }


  addChild(node) {
    let oldParent = node._parent;
    if (oldParent) {
      oldParent.removeChild(node);
    }
    this._children.push(node);
    node._parent = this;
  }
  removeChild(node) {
    for (let i = 0; i < this._children.length; i++) {
      if (this._children[i] === node) {
        this._children.splice(i, 1);
        return;
      }
    }
  }
  fTransformation() {
    return fmat3.create();
  }
  fInverseTransformation() {
    return fmat3.create();
  }

  transformation() {
    return mat3.create();
  }
  inverseTransformation() {
    return mat3.create();
  }


  render() {
    let id = this._id;
    return (<polygon key={id} />);
  }

  name() {
    return this._name;
  }

  getNodeByName(name) {
    let foundNode = null;
    this.traverse((node) => {
      if (node.name() === name) {
        foundNode = node;
        return false;
      }
    });
    return foundNode;
  }
}

class Rotation extends Node {
  constructor(name, angle) {
    super(name);
    this._angle = angle;
  }

  transformation() {
    return mat3.fromRotation(mat3.create(), this._angle); 
  }

  inverseTransformation() {
    return mat3.fromRotation(mat3.create(), -this._angle); 
  }

  fTransformation() {
    let m = fmat3.fromRotation(mat3.create(), this._angle); 
    let dummy = fmat3.create();
    m = fmat3.multiply(m, dummy, m);
    return m;
  }

  fInverseTransformation() {
    let m = fmat3.fromRotation(mat3.create(), -this._angle); 
    let dummy = fmat3.create();
    m = fmat3.multiply(m, dummy, m);
    return m;
  }

  set(angle) {
    this._angle = angle;
  }

  angle() {
    return this._angle;
  }
}

class Translation extends Node {
  constructor(name, x, y) {
    super(name);
    this._x = x;
    this._y = y;
  }

  transformation() {
    let v = vec2.fromValues(this._x, this._y);
    let m = mat3.fromTranslation(mat3.create(), v);
    return m;
  }

  inverseTransformation() {
    let v = vec2.fromValues(-this._x, -this._y);
    let m = mat3.fromTranslation(mat3.create(), v);
    return m;
  }

  fTransformation() {
    let v = fvec2.fromValues(this._x, this._y);
    let m = fmat3.fromTranslation(fmat3.create(), v);
    let dummy = fmat3.create();
    m = fmat3.multiply(m, dummy, m);
    return m;
  }

  fInverseTransformation() {
    let v = fvec2.fromValues(-this._x, -this._y);
    let m = fmat3.fromTranslation(fmat3.create(), v);
    let dummy = fmat3.create();
    m = fmat3.multiply(m, dummy, m);
    return m;
  }

  set(x, y) {
    this._x = x;
    this._y = y;
  }

  x() {
    return this._x;
  }

  y() {
    return this._y;
  }
}

class Circle extends Node {
  constructor(name, size, segments) {
    super(name);
    this._size = size;
    this._segments = segments;
    
  }

  mouseDown(click) {
    console.log(this);
    drag.start((deltaX, deltaY) => {
      let translation = this.parent();
      let x = translation.x();
      let y = translation.y();
      translation.set(x + deltaX, y + deltaY);
    });
  }

  mouseUp(click) {
    drag.end();
  }

  render(camera, center) {
    //if (this._name === 'sun') debugger;

    let size = this._size;
    let segments = this._segments;

    let verts = [];
    let n = segments || 10;
    for (let i = 0; i <= n; i++) {  
      let angle = i / n * 2 * Math.PI;
      let v = fvec3.fromValues(size * Math.cos(angle), size * Math.sin(angle), 1.0);
      fvec3.add(v, v, fvec3.fromValues(0, 0, 0));
      verts.push(v);
    }

    let viewMatrix = camera.viewMatrix(this);
    let offsetMatrix = center.transformationFrom(camera);

    let transformedVerts = [];
    for (let i = 0; i < verts.length; i++) {
      let transformedVert = fvec3.fromValues(0, 0, 0);
      // In low precision: apply relative camera transformation.
      fvec3.transformMat3(transformedVert, verts[i], viewMatrix);

      // In high precision: apply camera position. (bring it to visualization world space)
      vec3.transformMat3(transformedVert, transformedVert, offsetMatrix);
      transformedVerts.push(transformedVert);
    }

    let vertString = '';
    transformedVerts.forEach((v) => {
      vertString += numeral(v[0]).format('0.0') + ',' + numeral(v[1]).format('0.0') + ' ';
    });




    let id = this._id;

    let domElement = (
      <polygon key={id} onMouseDown={this.mouseDown.bind(this)} onMouseUp={this.mouseUp.bind(this)} className={this._name} points={vertString}/>
    );

    return domElement;
  }
}

class Camera extends Node {
  constructor(name) {
    super(name);
  }

  viewMatrix(node) {
    return this.fTransformationFrom(node);
  }

  render(camera, center) {
    let offsetMatrix = center.transformationFrom(camera);
    let scaleMatrix = mat3.create();
    mat3.scale(scaleMatrix, scaleMatrix, vec2.fromValues(15, 15));

    let p0 = mat3.fromValues(0, 0, 1);
    let p1 = mat3.fromValues(0, -0.5, 1);
    let p2 = mat3.fromValues(-1, -0.5, 1);
    let p3 = mat3.fromValues(-1, 0.5, 1);
    let p4 = mat3.fromValues(0, 0.5, 1);
    let p5 = p0;
    let p6 = mat3.fromValues(0.5, 0.5, 1);
    let p7 = mat3.fromValues(0.5, -0.5, 1);
    let p8 = p0;

    let verts = [p0, p1, p2, p3, p4, p5, p6, p7, p8];
    let transformedVerts = [];
    for (let i = 0; i < verts.length; i++) {
      let transformedVert = fvec3.fromValues(0, 0, 0);
      // In high precision: apply camera position. (bring it to visualization world space)
      vec3.transformMat3(transformedVert, verts[i], scaleMatrix);
      vec3.transformMat3(transformedVert, transformedVert, offsetMatrix);
      transformedVerts.push(transformedVert);
    }

    let vertString = '';
    transformedVerts.forEach((v) => {
      vertString += numeral(v[0]).format('0.0') + ',' + numeral(v[1]).format('0.0') + ' ';
    });

    return (
      <g>
        <polygon points={vertString} className={this._name} />
      </g>
    );

  }
}

class CoordinateSystem extends Node {
  constructor(name, w, h) {
    super(name);
    this._w = w;
    this._h = h;
  } 

  render(camera, center) {
    let viewMatrix = camera.viewMatrix(this);
    let offsetMatrix = center.transformationFrom(camera);

    let vertex1 = fvec3.fromValues(-this._w, 0, 1);
    let vertex2 = fvec3.fromValues(this._w, 0, 1);
    let vertex3 = fvec3.fromValues(0, -this._h, 1);
    let vertex4 = fvec3.fromValues(0, this._h, 1);

    fvec3.transformMat3(vertex1, vertex1, viewMatrix);
    fvec3.transformMat3(vertex2, vertex2, viewMatrix);
    fvec3.transformMat3(vertex3, vertex3, viewMatrix);
    fvec3.transformMat3(vertex4, vertex4, viewMatrix);

    vec3.transformMat3(vertex1, vertex1, offsetMatrix);
    vec3.transformMat3(vertex2, vertex2, offsetMatrix);
    vec3.transformMat3(vertex3, vertex3, offsetMatrix);
    vec3.transformMat3(vertex4, vertex4, offsetMatrix);


    return (<g>
      <line key={"x"} className={this._name} x1={vertex1[0]} y1={vertex1[1]} x2={vertex2[0]} y2={vertex2[1]} style={{stroke:'rgba(255,0,0, 0.5)', strokeWidth: 1}}/>
      <line key={"y"} className={this._name} x1={vertex3[0]} y1={vertex3[1]} x2={vertex4[0]} y2={vertex4[1]} style={{stroke:'rgba(255,0,0, 0.5)', strokeWidth: 1}}/>
    </g>);
  }
}


export {Node, Translation, Rotation, Circle, Camera, CoordinateSystem};