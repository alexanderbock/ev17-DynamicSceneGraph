import React from 'react';
import { render } from 'react-dom';
import Scene from './scene'

const content = ((
  <div>
  	<Scene method='separateMatrices'/>
  	<Scene method='mergedMatrices'/>
  	<Scene method='ours'/>
  </div>
));

render(content, document.getElementById('main-container'));
