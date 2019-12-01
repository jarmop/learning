import React, {useEffect} from 'react';
import {Engine, Render, World, Bodies} from 'matter-js';
import './App.css';

const renderMatter = (element) => {
  const engine = Engine.create();

  const render = Render.create({
    element: element,
    engine: engine,
  });

  const boxA = Bodies.rectangle(400, 200, 80, 80);
  const boxB = Bodies.rectangle(450,50,80,80);
  const ground = Bodies.rectangle(400, 610, 810, 60, {isStatic: true});

  World.add(engine.world, [boxA, boxB, ground]);

  Engine.run(engine);
  Render.run(render);
};

function App() {
  let element = null;
  useEffect(() => {
    renderMatter(element);
  });

  return (
    <div className="App" ref={domObject => element = domObject}>

    </div>
  );
}

export default App;
