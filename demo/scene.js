import React from 'react';
import sceneGraph from './scenegraph';
import config from './floatingpointconfig';

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
    let svgElements = [];

    let ourMethod = sceneGraph.getNodeByName('ourMethod');
    let ourCamera = sceneGraph.getNodeByName('ourCamera');
    let naiveMethod = sceneGraph.getNodeByName('naiveMethod');
    let naiveCamera = sceneGraph.getNodeByName('naiveCamera');

    ourMethod.traverse((node) => {
      svgElements.push(node.render(ourCamera));
    });

    naiveMethod.traverse((node) => {
      svgElements.push(node.render(naiveCamera));
    });

    //config.mantissaBits = (Math.round(this._frame / 50) % 6) + 1;
    //scenegraph.root.getNodeByName('translation').set(Math.sin(this._frame / 50) * 400, 0);


    return (
    <svg version="1.1" id="Layer_1" xmlns="http://www.w3.org/2000/svg" xmlnsXlink="http://www.w3.org/1999/xlink" x="0px" y="0px"
              width="100%" height="100%" viewBox="-800 -600 1600 1200" enableBackground="new 0 0 800 600"
              xmlSpace="preserve">
          {svgElements}
    </svg>
    );
  }
};

