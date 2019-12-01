import React, {useEffect} from 'react';
import {Engine, Render, World, Bodies, Composite} from 'matter-js';
import './App.css';

const render = (canvas, engine) => {
  var bodies = Composite.allBodies(engine.world);

  window.requestAnimationFrame(() => render(canvas, engine));

  const context = canvas.getContext('2d');
  context.fillStyle = '#fff';
  context.fillRect(0, 0, canvas.width, canvas.height);

  context.beginPath();

  for (var i = 0; i < bodies.length; i += 1) {
    var vertices = bodies[i].vertices;

    context.moveTo(vertices[0].x, vertices[0].y);

    for (var j = 1; j < vertices.length; j += 1) {
      context.lineTo(vertices[j].x, vertices[j].y);
    }

    context.lineTo(vertices[0].x, vertices[0].y);
  }

  context.lineWidth = 1;
  context.strokeStyle = '#999';
  context.stroke();
};

const renderMatter = (canvas) => {
  const engine = Engine.create();

  // const render = Render.create({
  //   element: element,
  //   engine: engine,
  // });

  const boxA = Bodies.rectangle(400, 200, 80, 80);
  const boxB = Bodies.rectangle(450,50,80,80);
  const ground = Bodies.rectangle(400, 610, 810, 60, {isStatic: true});

  World.add(engine.world, [boxA, boxB, ground]);

  Engine.run(engine);
  // Render.run(render);
  render(canvas, engine);
};

function App() {
  let canvas = null;
  useEffect(() => {
    renderMatter(canvas);
  });

  return (
    <div className="App">
      <canvas width="800" height="600" ref={domObject => canvas = domObject}></canvas>
    </div>
  );
}

export default App;
