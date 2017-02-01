import React from 'react';
import { render } from 'react-dom';
import Scene from './scene';
import Circles from './circles';
import FloatingPointOptions from './floatingpointoptions';
    
const content = ((
  <div>
    <div className="method-container">
      <Scene method="separate"/>
      <p> 
        Method A: Transform object vertices to world space using model matrix.
        Transform world space vertices to view space.
      </p>
    </div>
    <div className="method-container">
      <Scene method="merged"/>
      <p>
        Method B: Merge model and view matrix to one matrix.
        Transform object vertices to view space using model-view matrix.
      </p>
    </div>
    <div className="method-container">
      <Scene method="ours"/>
      <p>
        Method C (ours): Compose transformation matrix through shortest path in scene graph.
        Transorm object vertices to view space using this matrix.
      </p>
    </div>
    <FloatingPointOptions/>
  </div>
));

render(content, document.getElementById('main-container'));