import React, {useState, useEffect} from 'react';
import Editor from 'react-simple-code-editor';
import { highlight, languages } from 'prismjs/components/prism-core';
import 'prismjs/components/prism-clike';
import 'prismjs/components/prism-javascript';
import './App.css';

let runCode = (code) => {
  console.log('run code');
  //let frame = document.createElement("iframe");
  let frame = document.querySelector('iframe');
  document.body.appendChild(frame);
  let win = frame.contentWindow;
  win.console.log = (msg) => win.document.write(msg);
//win.console.log = (msg) => alert(msg);

//console.log(win);
  //code = document.querySelector('#editor').innerText;
  win.eval(code);
  let val = win.document.querySelector('body').innerText;

  document.querySelector('#output').innerText = val;

//console.log(window.document.querySelector('#output'));
};

function App() {
  const [code, setCode] = useState("console.log('werg');");
  useEffect(() => runCode(code));

  return (
      <div className="App">
        <Editor
          value={code}
          highlight={code => highlight(code, languages.js)}
          onValueChange={setCode}
        />
        <div id="output">fgb</div>
        <iframe style={{display: 'none'}}></iframe>
      </div>
  );
}

export default App;
