import React, {useState} from 'react';
import Editor from 'react-simple-code-editor';
import Prism from 'prismjs/components/prism-core';
import 'prismjs/components/prism-clike';
import 'prismjs/components/prism-javascript';
import 'prismjs/themes/prism.css';
import './App.css';

const run = (code) => {
  // console.log('run code');
  let frame = document.querySelector('iframe');
  document.body.appendChild(frame);
  let win = frame.contentWindow;
  win.console.log = (msg) => win.document.write(msg);
  win.eval(code);
  let val = win.document.querySelector('body').innerText;

  return val;
};

const initialCode = `for (let i = 0; i<5; i++) {
    console.log('a');
}`;

function App() {
  const [code, setCode] = useState(initialCode);
  const [output, setOutput] = useState('');

  const runCode = (code) => {
    const result = run(code);
    setOutput(result);
  };

  return (
      <div className="App">
        <Editor
          value={code}
          onValueChange={setCode}
          highlight={code => Prism.highlight(code, Prism.languages.js)}
          padding={10}
          style={{
            fontFamily: '"Fira code", "Fira Mono", monospace',
            fontSize: 12,
          }}
        />
        <button onClick={() => runCode(code)}>Run code</button>
        <div id="output">{output}</div>
        <iframe title="output" style={{display: 'none'}}></iframe>
      </div>
  );
}

export default App;
