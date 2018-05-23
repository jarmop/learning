import React, {Component} from 'react';

import './code.css';

function camelCaseToDash(str) {
  return str.replace(/([a-zA-Z])(?=[A-Z])/g, '$1-').toLowerCase();
}

function cssObjectToString(cssObject) {
  let cssString = '';
  for (let selector in cssObject) {
    cssString += '<span class="selector">.' + selector + '</span> {\n';
    let block = cssObject[selector];
    for (let property in block) {
      let value = block[property];
      cssString += '  <span class="property">' + camelCaseToDash(property) + '</span>: <span class="value">' + value + '</span>;\n';
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
          <code dangerouslySetInnerHTML={{__html: cssString}}>
          </code>
        </pre>
    );
  }
}

export default Code;