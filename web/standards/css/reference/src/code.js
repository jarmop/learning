import React, {Component} from 'react';
import 'prismjs/themes/prism.css';
import Prism from 'prismjs';

function camelCaseToDash (str) {
  return str.replace(/([a-zA-Z])(?=[A-Z])/g, '$1-').toLowerCase()
}

function cssObjectToString(cssObject) {
  let cssString = '';
  for (let selector in cssObject) {
    cssString += '.' + selector + ' {\n';
    let block = cssObject[selector];
    for (let property in block) {
      let value = block[property];
      cssString += '  ' + camelCaseToDash(property) + ': ' + value + ';\n';
    }
    cssString += '}\n';
  }

  return cssString;
}

class Code extends Component {
  render() {
    let {cssObject} = this.props;
    let cssString = cssObjectToString(cssObject);

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