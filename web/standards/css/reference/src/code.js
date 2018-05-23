import React, {Component, Fragment} from 'react';

import './code.css';

function camelCaseToDash(str) {
  str += ''; // make sure str is a string
  return str.replace(/([a-zA-Z])(?=[A-Z])/g, '$1-').toLowerCase();
}

class Code extends Component {
  render() {
    let {cssObject} = this.props;

    return (
        <div className="code-block">
          {Object.keys(cssObject).map(selector =>
              <Fragment key={selector}>
                <span className="selector">{'.' + camelCaseToDash(selector)}</span>
                {' {'}
                <br/>
                {Object.keys(cssObject[selector]).map(property =>
                    <Fragment key={property}>
                      &nbsp;&nbsp;
                      <span className="property">{camelCaseToDash(property)}</span>
                      {': '}
                      <span className="value">{camelCaseToDash(cssObject[selector][property])}</span>
                      {';'}
                      <br/>
                    </Fragment>
                )}
                {'}'}
                <br/>
              </Fragment>
          )}
        </div>
    );
  }
}

export default Code;