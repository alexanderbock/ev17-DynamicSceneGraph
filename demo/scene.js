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
    
    function r() {
      self.forceUpdate();
      self._frame++;
      if (self._running) {
          requestAnimationFrame(r);
      }
    }

    requestAnimationFrame(r);

    window.addEventListener('keypress', (evt) => {
      if (evt.key === ' ') {
        this._running = !this._running;
        if (this._running) {
          r();
        }
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

    let moonOrbit = sceneGraph.getNodeByName('moonOrbit');
    let cameraMoonOrbitCompensation = sceneGraph.getNodeByName('cameraMoonOrbitCompensation');    

    let camera = sceneGraph.getNodeByName('camera'); 
    earthOrbit.set(this._frame / 1000);
    cameraEarthOrbitCompensation.set(-this._frame / 1000)

    moonOrbit.set(this._frame / 1000 * 12);
    cameraMoonOrbitCompensation.set(-this._frame / 1000 * 12)


    let svgElements = [];
    sceneGraph.traverse((node) => {
      const elem = node.render(camera, sun, method);
      svgElements.push(elem);
    });



    //config.mantissaBits = (Math.round(this._frame / 50) % 6) + 1;
    //scenegraph.root.getNodeByName('translation').set(Math.sin(this._frame / 50) * 400, 0);


    return (
    
      <div>
        <svg version="1.1" id="Layer_1" xmlns="http://www.w3.org/2000/svg" xmlnsXlink="http://www.w3.org/1999/xlink" x="0px" y="0px"
                  width="100%" height="100%" viewBox="-350 -350 700 700" enableBackground="new 0 0 700 700"
                  xmlSpace="preserve">
              {svgElements}
        </svg>
      </div>
    );
  }
};

