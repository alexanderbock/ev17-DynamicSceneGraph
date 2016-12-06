import React from 'react';
import sceneGraph from './scenegraph';
import config from './floatingpointconfig';
import fRound from './fRound';

export default class extends React.Component {
  constructor(props) {
    console.log(props);
    super();
    this._frame = 0;
    this._method = props.method;
  }

  componentDidMount() {
    this._running = true;
    this._frame = 0;
    let self = this;
    requestAnimationFrame(function r() {
      self.forceUpdate();
      self._frame++;
      if (self._running) {
          requestAnimationFrame(r);
      }
    });
  }

  componentWillUnmount() {
    this._running = false;
  }

  render() {
    let method = this._method;
    let earth = sceneGraph.getNodeByName('earth');
    let sun = sceneGraph.getNodeByName('sun');
    let earthOrbit = sceneGraph.getNodeByName('earthOrbit');
    let cameraEarthOrbitCompensation = sceneGraph.getNodeByName('cameraEarthOrbitCompensation');
    let camera = sceneGraph.getNodeByName('camera'); 
    earthOrbit.set(this._frame / 1000);
    cameraEarthOrbitCompensation.set(-this._frame / 1000)


    let svgElements = [];
    sceneGraph.traverse((node) => {
      svgElements.push(node.render(camera, sun, method));
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

