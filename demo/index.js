import React from 'react';
import { render } from 'react-dom';
import Scene from './scene';
import Circles from './circles';
import FloatingPointOptions from './floatingpointoptions';

const content = ((
  <div>
  	<Scene method='separate'/>
  	<Scene method='merged'/>
  	<Scene method='ours'/>
  	<FloatingPointOptions/>
  </div>
));

render(content, document.getElementById('main-container'));


//<Circles method = 'merged'/>