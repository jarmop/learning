import React from 'react';
import Button from '@material-ui/core/Button';

function App() {
  return (
      <div>
        <h1>Material UI Showroom</h1>
        <h2>Button</h2>
        <Button variant="contained" color="primary">
          Hello World
        </Button>
      </div>
  );
}

export default App;

let frame = document.createElement("iframe");
document.body.appendChild(frame);
let win = frame.contentWindow;
//win.console.log = (msg) => win.document.write(msg);
win.console.log = (msg) => alert(msg);

console.log(win);
let val = win.eval('console.log("dfb");');

console.log(val);
