import React from 'react';
import config from './floatingpointconfig';
import fRound from './fRound';

import {Node, Circle, Translation, Rotation, Camera, CoordinateSystem} from './nodes';

export default class extends React.Component {
  constructor(props) {
    console.log(props);
    super();
    this._method = props.method;


    this.sceneGraph = new Node();
    for (let i = 0; i < 10; i++) for (let j = 0; j < 10; j++) {
      let name = 'x' + i + 'y' + j;
      let translation = new Translation('translation_' + name, i * 50, j * 50);
      let circle = new Circle('circle_' + name, 20, 100);
      this.sceneGraph.addChild(translation);
      translation.addChild(circle);
    }

    let camera = new Camera('camera');
    this.sceneGraph.addChild(camera);
  }

  componentDidMount() {

  }

  componentWillUnmount() {
  }

  render() {
    let method = this._method;

    let camera = this.sceneGraph.getNodeByName('camera');

    let svgElements = [];
    this.sceneGraph.traverse((node) => {
      svgElements.push(node.render(camera, this.sceneGraph, method));
    });

    //config.mantissaBits = (Math.round(this._frame / 50) % 6) + 1;
    //scenegraph.root.getNodeByName('translation').set(Math.sin(this._frame / 50) * 400, 0);


    return (
    
      <svg version="1.1" id="Layer_1" xmlns="http://www.w3.org/2000/svg" xmlnsXlink="http://www.w3.org/1999/xlink" x="0px" y="0px"
                width="33%" height="33%" viewBox="-300 -300 600 600" enableBackground="new 0 0 600 600"
                xmlSpace="preserve">
            {svgElements}
      </svg>
    
    );
  }
};

