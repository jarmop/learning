import {Composite} from 'matter-js';

const renderCustom = (canvas, engine) => {
  var bodies = Composite.allBodies(engine.world);

  window.requestAnimationFrame(() => renderCustom(canvas, engine));

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

export default renderCustom;