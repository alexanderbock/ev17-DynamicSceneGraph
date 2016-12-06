import {Node, Circle, Translation, Camera, CoordinateSystem} from './nodes';

let rootNode = new Node('root');


let ourMethod = new Translation('ourMethod', 400, 0);
let naiveMethod = new Translation('naiveMethod', -400, 0);


rootNode.addChild(ourMethod);
rootNode.addChild(naiveMethod);

let ourSunTranslation = new Translation('sunTranslation', 0, 0);
let ourEarthTranslation = new Translation('earthTranslation', 200, 0);
let ourSun = new Circle('sun', 40, 100);
let ourEarth = new Circle('earth', 20, 100);
let ourSystem = new CoordinateSystem('view', 100, 100);

let naiveSunTranslation = new Translation('sunTranslation', 0, 0);
let naiveEarthTranslation = new Translation('earthTranslation', 200, 0);
let naiveSun = new Circle('sun', 40, 100);
let naiveEarth = new Circle('earth', 20, 100);
let naiveSystem = new CoordinateSystem('view', 100, 100);


ourMethod.addChild(ourSunTranslation);

ourSunTranslation.addChild(ourSun);
ourSunTranslation.addChild(ourEarthTranslation);

ourEarthTranslation.addChild(ourEarth);
ourEarthTranslation.addChild(ourSystem);


naiveMethod.addChild(naiveSunTranslation);

naiveSunTranslation.addChild(naiveSun);
naiveSunTranslation.addChild(naiveEarthTranslation);

naiveEarthTranslation.addChild(naiveEarth);
naiveSunTranslation.addChild(naiveSystem);


let ourCamera = new Camera('ourCamera');
ourEarthTranslation.addChild(ourCamera);

let naiveCamera = new Camera('naiveCamera');
naiveSunTranslation.addChild(naiveCamera);





/*
for (let i = -7; i <= 7; i++) for (let j = -7; j <= 7; j++) {
  let trans = new Translation('trans' + (i+7) + ',' + (j+7), i * 50, j * 50);
  let circle = new Circle('circle' + (i+7) + ',' + (j+7), 20, 100);
  rootNode.addChild(trans);
  trans.addChild(circle);
}*/


export default rootNode;