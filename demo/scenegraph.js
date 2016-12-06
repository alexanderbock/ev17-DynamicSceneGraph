import {Node, Circle, Translation, Camera} from './nodes';

let rootNode = new Node('root');
let sunTranslation = new Translation('sunTranslation', 0, 0);
let earthTranslation = new Translation('earthTranslation', 100, 0);
let sun = new Circle('sun', 40, 100);

let earth = new Circle('earth', 20, 100);

//rootNode.addChild(sunTranslation);
//sunTranslation.addChild(sun);

//rootNode.addChild(earthTranslation);
//earthTranslation.addChild(earth);



let camera = new Camera();
rootNode.addChild(camera);



for (let i = -7; i <= 7; i++) for (let j = -7; j <= 7; j++) {
  let trans = new Translation('trans' + (i+7) + ',' + (j+7), i * 50, j * 50);
  let circle = new Circle('circle' + (i+7) + ',' + (j+7), 20, 100);
  rootNode.addChild(trans);
  trans.addChild(circle);
}


export default {root: rootNode, camera: camera};