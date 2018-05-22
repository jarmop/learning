import React, {Component} from 'react';
import 'prismjs/themes/prism.css';
import Prism from 'prismjs';

const cssString = `
.grid {
  display: grid;
  color: black;
}
`;

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