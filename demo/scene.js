import React from 'react';
import sceneGraph from './scenegraph';
import config from './floatingpointconfig';
import fRound from './fRound';

export default class extends React.Component {
  constructor() {
    super();
    this._frame = 0;
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
    let earth = sceneGraph.getNodeByName('earth');
    let sun = sceneGraph.getNodeByName('sun');
    let earthOrbit = sceneGraph.getNodeByName('earthOrbit');
    let cameraEarthOrbitCompensation = sceneGraph.getNodeByName('cameraEarthOrbitCompensation');
    let camera = sceneGraph.getNodeByName('camera'); 
    earthOrbit.set(this._frame / 100);
    cameraEarthOrbitCompensation.set(-this._frame / 100)


    let svgElements = [];
    sceneGraph.traverse((node) => {
      svgElements.push(node.render(camera, cameraEarthOrbitCompensation));
    });

    //config.mantissaBits = (Math.round(this._frame / 50) % 6) + 1;
    //scenegraph.root.getNodeByName('translation').set(Math.sin(this._frame / 50) * 400, 0);


    return (
    <div>
      <svg version="1.1" id="Layer_1" xmlns="http://www.w3.org/2000/svg" xmlnsXlink="http://www.w3.org/1999/xlink" x="0px" y="0px"
                width="50%" height="50%" viewBox="-400 -300 800 600" enableBackground="new 0 0 800 600"
                xmlSpace="preserve">
            {svgElements}
      </svg>
    </div>
    );
  }
};

