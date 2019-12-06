import React, {useEffect} from 'react';
import Matter, {Engine, World, Bodies, Composite} from 'matter-js';
import decomp from 'poly-decomp';

import Render from './Render';
// import renderCustom from './RenderCustom';
import './App.css';

window.decomp = decomp; // required for concave shapes

const renderMatter = (element) => {
  const engine = Engine.create();

  const render = Render.create({
    element: element,
    engine: engine,
  });

  // const boxA = Bodies.rectangle(400, 200, 80, 80);
  // const boxB = Bodies.rectangle(450,50,80,80);
  // const ground = Bodies.rectangle(400, 610, 810, 60, {isStatic: true});

  let ver = Matter.Vertices.fromPath('35 7 19 17 14 38 14 58 25 79 45 85 65 84 65 66 46 67 34 59 30 44 33 29 45 23 66 23 66 7 53 7');
  const ground = Bodies.fromVertices(300, 300, ver, {isStatic: true});

  // World.add(engine.world, [boxA, boxB, ground]);
  World.add(engine.world, [ground]);

  Engine.run(engine);
  Render.run(render);
  // renderCustom(element, engine);
};

function App() {
  let element = null;
  useEffect(() => {
    renderMatter(element);
  });

  // Use this for Render
  return (
    <div className="App" ref={domObject => element = domObject}>
    </div>
  );

  // Use this for renderCustom
  // return (
  //   <div className="App">
  //     <canvas width="800" height="600" ref={domObject => element = domObject}></canvas>
  //   </div>
  // );
}

export default App;
