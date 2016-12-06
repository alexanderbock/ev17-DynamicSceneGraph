import fRound from './fround';
import {glMatrix, vec2, vec3, vec4, mat2, mat3, mat4} from './glround';
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

class Translation extends Node {
  constructor(name, x, y) {
    super(name);
    this._x = x;
    this._y = y;
  }

  transformation() {
    let v = vec2.fromValues(this._x, this._y);
    let m = mat3.fromTranslation(mat3.create(), v);
    let dummy = mat3.create();
    m = mat3.multiply(m, dummy, m);
    return m;
  }

  inverseTransformation() {
    let v = vec3.fromValues(-this._x, -this._y);
    let m = mat3.fromTranslation(mat3.create(), v);
    let dummy = mat3.create();
    m = mat3.multiply(m, dummy, m);
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

  render(camera) {
    let size = this._size;
    let segments = this._segments;

    let verts = [];
    let n = segments || 10;
    for (let i = 0; i <= n; i++) {  
      let angle = i / n * 2 * Math.PI;
      let v = vec3.fromValues(size * Math.cos(angle), size * Math.sin(angle), 1.0);
      vec3.add(v, v, vec3.fromValues(0, 0, 0));
      verts.push(v);
    }

    let viewMatrix = camera.viewMatrix(this);
    let transformedVerts = [];
    for (let i = 0; i < verts.length; i++) {
      let transformedVert = vec3.fromValues(0, 0, 0);
      vec3.transformMat3(transformedVert, verts[i], viewMatrix);
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
    return this.transformationFrom(node);
  }
}


class CoordinateGrid extends Node {
  constructor(name) {
    super(name);
  }

  render(camera) {
    let lines = [];
    let manBits = config.mantissaBits;
    let expBits = config.exponentBits;
    let maxExp = Math.pow(2, expBits - 1);
    let minExp = -maxExp;

    let x = 0;
    for (let e = 6; e < 9; e++) {
      let significand = 1;
      for (let m = 0; m < Math.pow(2, manBits); m++) {
        significand += Math.pow(2, -manBits);

        x = significand * Math.pow(2, e);
        //console.log(significand, -manBits, x, Math.pow(2, e));
        let idPos = "x" + x;
        let idNeg = "x-" + x;
        lines.push(
          <line key={idPos} x1={-x} x2={-x} y1={-400} y2={400} style={{stroke:'rgba(255,0,0, 0.5)', strokeWidth: 1}}/>
        );
        lines.push(
          <line key={idNeg} x1={x} x2={x} y1={-400} y2={400} style={{stroke:'rgba(255,0,0, 0.5)', strokeWidth: 1}}/>
        );
      }
    }

    let y = 0;
    for (let e = 6; e < 9; e++) {
      let significand = 1;
      for (let m = 0; m < Math.pow(2, manBits); m++) {
        significand += Math.pow(2, -manBits);

        y = significand * Math.pow(2, e);
        //console.log(significand, -manBits, x, Math.pow(2, e));
        let idPos = "y" + y;
        let idNeg = "y-" + y;
        lines.push(
          <line key={idPos} y1={-x} y2={-x} x1={-400} x2={400} style={{stroke:'rgba(255,0,0, 0.5)', strokeWidth: 1}}/>
        );
        lines.push(
          <line key={idNeg} y1={x} y2={x} x1={-400} x2={400} style={{stroke:'rgba(255,0,0, 0.5)', strokeWidth: 1}}/>
        );
      }
    }


    
    let domElement = (
      <g className={this._name}>
        {lines}
      </g>
    );

    return domElement;
  }
}

class CoordinateSystem extends Node {
  constructor(name, w, h) {
    super(name);
    this._w = w;
    this._h = h;
  } 

  render(camera) {
    let viewMatrix = camera.viewMatrix(this);
    let vertex1 = vec3.fromValues(-this._w, 0, 1);
    let vertex2 = vec3.fromValues(this._w, 0, 1);
    let vertex3 = vec3.fromValues(0, -this._h, 1);
    let vertex4 = vec3.fromValues(0, this._h, 1);

    vec3.transformMat3(vertex1, vertex1, viewMatrix);
    vec3.transformMat3(vertex2, vertex2, viewMatrix);
    vec3.transformMat3(vertex3, vertex3, viewMatrix);
    vec3.transformMat3(vertex4, vertex4, viewMatrix);
    return (<g>
      <line key={"x"} className={this._name} x1={vertex1[0]} y1={vertex1[1]} x2={vertex2[0]} y2={vertex2[1]} style={{stroke:'rgba(255,0,0, 0.5)', strokeWidth: 1}}/>
      <line key={"y"} className={this._name} x1={vertex3[0]} y1={vertex3[1]} x2={vertex4[0]} y2={vertex4[1]} style={{stroke:'rgba(255,0,0, 0.5)', strokeWidth: 1}}/>
    </g>);
  }
}


export {Node, Translation, Circle, Camera, CoordinateGrid, CoordinateSystem};