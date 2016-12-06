import React from 'react';
import { render } from 'react-dom';
import Scene from './scene'

const content = ((
  <div>
  	<Scene/>
  </div>
));

render(content, document.getElementById('main-container'));
