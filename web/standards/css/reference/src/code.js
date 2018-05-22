import React, {Component} from 'react';
import 'prismjs/themes/prism.css';
import Prism from 'prismjs';
// import jsBeautify from 'js-beautify';
// console.log(jsBeautify.css_beautify(".grid { display: grid; color: black; }"));

const cssObject = {
  '.grid': {
    display: 'grid',
    color: 'black',
  },
  '.cell': {
    display: 'grid',
    color: 'black',
  }
};

let cssString = '';
for (let selector in cssObject) {
  cssString += selector + ' {\n';
  let block = cssObject[selector];
  for (let property in block) {
    let value = block[property];
    cssString += '  ' + property + ': ' + value + ';\n';
  }
  cssString += '}\n';
}

class Code extends Component {
  render() {
    return (
        <pre>
          <code
              className="code"
              dangerouslySetInnerHTML={{
                __html: Prism.highlight(
                    cssString,
                    Prism.languages.css,
                    'css'
                )
              }}
          >
        </code>
        </pre>
    );
  }
}

export default Code;