import React from 'react';
import {css, cx} from 'emotion';

// import './App.css';

const color = 'white';

const styles = `
    padding: 32px;
    background-color: hotpink;
    font-size: 24px;
    border-radius: 4px;
    &:hover {
      color: ${color};
    }
`;

const className = css(styles);



// console.log(className);

function App() {
  return (
      <div
          className={className}
      >
        Hover to change color.
      </div>
  );
  // return (
  //   <div className="App">
  //     <header className="App-header">
  //       <p>
  //         Edit <code>src/App.js</code> and save to reload.
  //       </p>
  //       <a
  //         className="App-link"
  //         href="https://reactjs.org"
  //         target="_blank"
  //         rel="noopener noreferrer"
  //       >
  //         Learn React
  //       </a>
  //     </header>
  //   </div>
  // );
}

export default App;
