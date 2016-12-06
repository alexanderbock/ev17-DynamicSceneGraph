import {Node, Circle, Translation, Rotation, Camera, CoordinateSystem} from './nodes';

let rootNode = new Node('root');

let sunTranslation = new Translation('sunTranslation', 0, 0);
let earthOrbit = new Rotation('earthOrbit', 0);
let cameraEarthOrbitCompensation = new Rotation('cameraEarthOrbitCompensation', 0);
let earthTranslation = new Translation('earthTranslation', 200, 0);
let sun = new Circle('sun', 40, 100);
let earth = new Circle('earth', 20, 100);
//let system = new CoordinateSystem('system', 100, 100);
let cameraTranslation = new Translation('cameraTranslation', -40, 0);
let camera = new Camera('camera');

rootNode.addChild(sunTranslation);

sunTranslation.addChild(sun);
sunTranslation.addChild(earthOrbit);
earthOrbit.addChild(earthTranslation);

earthTranslation.addChild(cameraEarthOrbitCompensation);
cameraEarthOrbitCompensation.addChild(earth);
cameraEarthOrbitCompensation.addChild(cameraTranslation);
//cameraEarthOrbitCompensation.addChild(system);

cameraTranslation.addChild(camera);

/*
for (let i = -7; i <= 7; i++) for (let j = -7; j <= 7; j++) {
  let trans = new Translation('trans' + (i+7) + ',' + (j+7), i * 50, j * 50);
  let circle = new Circle('circle' + (i+7) + ',' + (j+7), 20, 100);
  rootNode.addChild(trans);
  trans.addChild(circle);
}*/


export default rootNode;